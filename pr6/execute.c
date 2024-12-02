#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

// 명령 실행 함수
int execute_command(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        return 1;
    }

    // 자식 프로세스 생성
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return 1;
    }

    if (pid == 0) { // 자식 프로세스
        // 명령 실행
        if (execvp(argv[1], &argv[1]) == -1) {
            perror("execvp failed");
            exit(EXIT_FAILURE);
        }
    } else { // 부모 프로세스
        int status;
        waitpid(pid, &status, 0); // 자식 프로세스 종료 대기
        if (WIFEXITED(status)) {
            printf("Command exited with status %d\n", WEXITSTATUS(status));
        } else {
            printf("Command execution failed.\n");
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    return execute_command(argc, argv);
}
