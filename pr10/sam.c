#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define SHM_KEY 12345        // 공유 메모리 키
#define SEM_KEY 67890        // 세마포어 키
#define SHM_SIZE 1024        // 공유 메모리 크기 (복사할 데이터의 크기)
#define FILE_SIZE 1024       // 복사할 파일의 크기

// 세마포어 초기화
int init_semaphore() {
    int sem_id = semget(SEM_KEY, 1, 0666 | IPC_CREAT);
    if (sem_id == -1) {
        perror("세마포어 생성 실패");
        exit(1);
    }

    // 세마포어 값 초기화
    semctl(sem_id, 0, SETVAL, 1);  // 세마포어 초기값을 1로 설정 (자원 공유 가능)
    return sem_id;
}

// 세마포어 작업 (P 연산)
void semaphore_wait(int sem_id) {
    struct sembuf sb = {0, -1, 0};  // P 연산
    semop(sem_id, &sb, 1);
}

// 세마포어 작업 (V 연산)
void semaphore_signal(int sem_id) {
    struct sembuf sb = {0, 1, 0};   // V 연산
    semop(sem_id, &sb, 1);
}

// 공유 메모리 할당
int create_shared_memory() {
    int shm_id = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shm_id == -1) {
        perror("공유 메모리 생성 실패");
        exit(1);
    }
    return shm_id;
}

// 공유 메모리 연결
void* attach_shared_memory(int shm_id) {
    void *shm_ptr = shmat(shm_id, NULL, 0);
    if (shm_ptr == (void *) -1) {
        perror("공유 메모리 연결 실패");
        exit(1);
    }
    return shm_ptr;
}

// 공유 메모리 분리
void detach_shared_memory(void *shm_ptr) {
    if (shmdt(shm_ptr) == -1) {
        perror("공유 메모리 분리 실패");
        exit(1);
    }
}

void parent_process(int shm_id, int sem_id, const char *src_file) {
    // 공유 메모리 할당
    void *shm_ptr = attach_shared_memory(shm_id);
    
    // 파일 열기
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror("소스 파일 열기 실패");
        exit(1);
    }

    // 파일에서 데이터를 읽어 공유 메모리에 저장
    int bytes_read = read(src_fd, shm_ptr, SHM_SIZE);
    if (bytes_read == -1) {
        perror("파일 읽기 실패");
        exit(1);
    }
    printf("파일을 읽어서 공유 메모리에 저장했습니다.\n");

    // 세마포어 신호 보내기 (자식 프로세스가 데이터를 쓸 수 있도록)
    semaphore_signal(sem_id);

    close(src_fd);
    detach_shared_memory(shm_ptr);
}

void child_process(int shm_id, int sem_id, const char *dest_file) {
    // 공유 메모리 할당
    void *shm_ptr = attach_shared_memory(shm_id);

    // 세마포어 대기 (부모가 데이터를 읽고 쓸 준비가 될 때까지 대기)
    semaphore_wait(sem_id);

    // 파일 열기
    int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest_fd == -1) {
        perror("목적지 파일 열기 실패");
        exit(1);
    }

    // 공유 메모리에서 데이터를 읽어 파일에 씁니다
    int bytes_written = write(dest_fd, shm_ptr, SHM_SIZE);
    if (bytes_written == -1) {
        perror("파일 쓰기 실패");
        exit(1);
    }
    printf("공유 메모리에서 데이터를 읽어 파일에 씁니다.\n");

    close(dest_fd);
    detach_shared_memory(shm_ptr);
}

int main() {
    // 세마포어 및 공유 메모리 초기화
    int sem_id = init_semaphore();
    int shm_id = create_shared_memory();

    // 부모-자식 프로세스 생성
    pid_t pid = fork();

    if (pid == 0) {  // 자식 프로세스
        child_process(shm_id, sem_id, "dest_file.txt");
    } else if (pid > 0) {  // 부모 프로세스
        parent_process(shm_id, sem_id, "src_file.txt");
        wait(NULL);  // 자식 프로세스 종료 대기
    } else {
        perror("fork 실패");
        exit(1);
    }

    // 세마포어 제거 및 공유 메모리 제거
    semctl(sem_id, 0, IPC_RMID);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}

