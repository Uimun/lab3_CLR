#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define PERMS 0644
char *workfile = "test";

int main(){
    int filedes;

    if ((filedes = open(workfile, O_RDWR | O_CREAT, PERMS)) == -1)
    {
        printf ("Couldn\'t open %s\n", workfile);
        filedes = creat(workfile, PERMS);
        exit(1);
    }

    close(filedes);

    return 0;
}