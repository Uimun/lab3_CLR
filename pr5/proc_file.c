#include <unistd.h>
#include <fcntl.h>

/* 파일 내에서 위치를 출력한다 */
int printpos (const char *string, int filedes)
{
    off_t pos;
    if ((pos = lseek (filedes, 0, SEEK_CUR)) == -1)
        fatal ("lseek failed");
    printf ("%s:%ld\n", string, pos);
}

int main()
{
    int fd;
    pid_t pid; /* 프로세스 식별번호 */
    char buf[10]; /* 파일 자료를 저장할 버퍼 */
    if ((fd = open ("data", O_RDONLY)) == -1)
        fatal ("open failed");
    read(fd, buf, 10); /* 파일 포인터를 전진시킨다 */
    printpos ("Before fork", fd);

    /* 이제 두 개의 프로세스를 생성한다 */
    switch (pid = fork()) {
        case -1: /* 오류 */
            fatal ("folk failed");
            break;
        case 0: /* 자식 */
            printpos ("Child before read", fd);
            read (fd, buf, 10);
            printpos ("Child after read", fd);
            break;
        default:
            wait ((int *) 0);
            printpos ("Parent after wait", fd);
    }
}
