#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define DIR_MAX_LENGTH 255
#define COMMEND_MAX_LENGTH 255
#define TOKEN_MAX_COUNT 32

char commenedDir[DIR_MAX_LENGTH];
char dir[DIR_MAX_LENGTH];

char input[COMMEND_MAX_LENGTH];
char *tokens[TOKEN_MAX_COUNT];

void execute_command(char **command) {
     pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) { // 자식 프로세스
        char path[DIR_MAX_LENGTH];
        snprintf(path, sizeof(path), "%s%s", commenedDir, command[0]); // 경로 생성

        execvp(path, tokens);

        exit(1);
    } else { // 부모 프로세스
        int status;

        waitpid(pid, &status, 0); // 자식 프로세스 종료 대기
    }
}

void change_dir(char *newPath){
    chdir(newPath);

    if(getcwd(dir, sizeof(dir)) == NULL){
        perror("getcwd");
        exit(1);
    }
}

int main(int argc, char *argv[]){
    if(getcwd(dir, sizeof(dir)) == NULL){
        perror("getcwd");
        exit(1);
    }

    strcat(commenedDir, dir);
    strcat(commenedDir, "/commends/");

    while (1) {
        printf("%s> ", dir);
        
        if (fgets(input, COMMEND_MAX_LENGTH - 1, stdin) == NULL) {
            perror("fgets");
        }

        input[strcspn(input, "\n")] = '\0';

        // 문자열 토큰화
        int tokenCount = 0;
        char *token = strtok(input, " ");
        while (token != NULL) {
            tokens[tokenCount++] = token;
            token = strtok(NULL, " ");
        }
        tokens[tokenCount] = NULL;

        if(strcmp(tokens[0], "cd") == 0){
            change_dir(tokens[1]);        
        }
        else if(strcmp(tokens[0], "exit") == 0){
            printf("exit shell");
            exit(0);
        }
        if (tokenCount > 0) {
            execute_command(tokens);
        }
    }
}
