#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFSIZE 512 /* 읽혀질 덩어리의 크기 */

int main(int argc, char *argv[])
{
    char buffer[BUFSIZE];
    int filedes;
    ssize_t nread;
    long total = 0;

    if (argc < 2) {
        fprintf(stderr, "Usage: file_char_count filename\n");
        exit(1);
    }

    if ( (filedes = open (argv[1], O_RDONLY) ) == -1)
    {
        printf("error in opening %s\n", argv[1]);
        exit(1);
    }

    while ( (nread = read (filedes, buffer, BUFSIZE) ) > 0)
        total += nread; /* total 을 증가시킨다 */

    printf("total chars in %s: %ld\n", argv[1], total);
    exit(0);
}