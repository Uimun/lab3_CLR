#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "Usage: rmdir filename\n");
        exit(1);
    }

    if (rmdir(argv[1]) != 0) {
        perror("rmdir");
    }

    return 0;
}