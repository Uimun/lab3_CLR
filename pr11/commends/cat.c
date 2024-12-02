#include <stdio.h>
#include <stdlib.h>

void print_usage() {
    printf("Usage: cat <file1> <file2> ... <fileN>\n");
}

void cat_file(const char *filename) {
    FILE *file = fopen(filename, "r"); 
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    fclose(file); 
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        print_usage(); 
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++) {
        cat_file(argv[i]);
    }

    return EXIT_SUCCESS;
}
