#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

void print_usage() {
    printf("Usage: mv <source> <destination>\n");
}

int move_file(const char *source, const char *destination) {
    // rename() 함수는 파일이나 디렉토리의 이름을 바꾸거나 위치를 변경하는 데 사용됩니다.
    if (rename(source, destination) == -1) {
        perror("Error moving file");
        return -1;
    }
    printf("Moved: %s -> %s\n", source, destination);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        print_usage();
        return EXIT_FAILURE;
    }

    const char *source = argv[1];
    const char *destination = argv[2];

    // 파일 이동 또는 이름 변경
    if (move_file(source, destination) == -1) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
