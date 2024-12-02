#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void print_usage() {
    printf("Usage: ln [-s] <target> <link_name>\n");
    printf("  -s: Create a symbolic link instead of a hard link\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 4) {
        print_usage();
        return EXIT_FAILURE;
    }

    int symbolic = 0;  // 기본적으로 하드 링크 생성
    char *target;
    char *link_name;

    if (argc == 4 && strcmp(argv[1], "-s") == 0) {
        symbolic = 1;  // 심볼릭 링크 생성
        target = argv[2];
        link_name = argv[3];
    } else if (argc == 3) {
        target = argv[1];
        link_name = argv[2];
    } else {
        print_usage();
        return EXIT_FAILURE;
    }

    if (symbolic) {
        // 심볼릭 링크 생성
        if (symlink(target, link_name) == -1) {
            perror("symlink failed");
            return EXIT_FAILURE;
        }
        printf("Symbolic link created: %s -> %s\n", link_name, target);
    } else {
        // 하드 링크 생성
        if (link(target, link_name) == -1) {
            perror("link failed");
            return EXIT_FAILURE;
        }
        printf("Hard link created: %s -> %s\n", link_name, target);
    }

    return EXIT_SUCCESS;
}
