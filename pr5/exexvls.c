#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    char * const argv[] = {"ls", "-l", (char *) 0};
    printf("executing execv.\n");
    execv("/bin/ls", argv);

    /* 만약 execv 이 복귀하면, 호출은 실패. */
    perror("execv failed to run ls");
    exit(1);
}