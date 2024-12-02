#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "Usage: mkdir filename\n");
        exit(1);
    }

    if (mkdir(argv[1], S_IRWXU) != 0) {
        perror("mkdir");
    }

    return 0;
}