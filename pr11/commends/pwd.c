#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define PATH_MAX_LENGTH 1024

int main() {
    char cwd[PATH_MAX_LENGTH];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);  // 현재 디렉토리 출력
    } else {
        perror("getcwd failed");  // 에러 처리
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}