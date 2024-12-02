#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#define MAX_TEXT 256  // 메시지 최대 크기

// 메시지 구조체 정의
struct message {
    long msg_type;       // 메시지 타입 (다양한 메시지 구분용)
    char text[MAX_TEXT]; // 메시지 내용
};

// 메시지 큐 생성 함수
int create_message_queue() {
    key_t key = ftok("chat_program", 65);  // 고유 키 생성
    int msg_id = msgget(key, 0666 | IPC_CREAT);  // 메시지 큐 생성
    if (msg_id == -1) {
        perror("메시지 큐 생성 실패");
        exit(1);
    }
    return msg_id;
}

// 메시지 보내기
void send_message(int msg_id, char *message) {
    struct message msg;
    msg.msg_type = 1;  // 메시지 타입 설정 (단일 채팅에선 모두 1)
    strncpy(msg.text, message, MAX_TEXT);

    if (msgsnd(msg_id, &msg, sizeof(msg.text), 0) == -1) {
        perror("메시지 보내기 실패");
        exit(1);
    }
    printf("메시지 전송: %s\n", message);
}

// 메시지 받기
void receive_message(int msg_id) {
    struct message msg;
    
    if (msgrcv(msg_id, &msg, sizeof(msg.text), 1, 0) == -1) {
        perror("메시지 받기 실패");
        exit(1);
    }
    printf("수신된 메시지: %s\n", msg.text);
}

// 메시지 큐 삭제
void delete_message_queue(int msg_id) {
    if (msgctl(msg_id, IPC_RMID, NULL) == -1) {
        perror("메시지 큐 삭제 실패");
        exit(1);
    }
}

int main() {
    int msg_id = create_message_queue();  // 메시지 큐 생성

    printf("채팅 프로그램을 시작합니다. 'quit' 입력시 종료됩니다.\n");

    pid_t pid = fork();
    
    if (pid == 0) {  // 자식 프로세스: 메시지 수신
        while (1) {
            receive_message(msg_id);
        }
    } else if (pid > 0) {  // 부모 프로세스: 메시지 전송
        char message[MAX_TEXT];
        while (1) {
            printf("메시지를 입력하세요: ");
            fgets(message, MAX_TEXT, stdin);
            message[strcspn(message, "\n")] = '\0'; // 개행 문자 제거

            if (strcmp(message, "quit") == 0) {
                break;  // 'quit' 입력시 종료
            }

            send_message(msg_id, message);
        }
        kill(pid, SIGKILL);  // 자식 프로세스 종료
        delete_message_queue(msg_id);  // 메시지 큐 삭제
    } else {
        perror("fork 실패");
        exit(1);
    }

    return 0;
}
