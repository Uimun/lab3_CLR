#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2){
        fprintf(stderr, "Usage: rm filename\n");
        exit(1);
    }

    if (remove(argv[1]) != 0) {
        perror("remove");
    }

    return 0;
}