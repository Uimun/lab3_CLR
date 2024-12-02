#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>

void print_usage() {
    printf("Usage: ls [options] [directory]\n");
    printf("Options:\n");
    printf("  -l : Long listing format\n");
    printf("  -a : Show all files, including hidden ones\n");
}

void list_directory(const char *path, int long_format, int show_all) {
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error opening directory");
        return;
    }

    struct dirent *entry;
    struct stat statbuf;
    while ((entry = readdir(dir)) != NULL) {
        if (!show_all && entry->d_name[0] == '.') {
            continue; // Skip hidden files (those starting with '.')
        }

        if (long_format) {
            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);
            if (stat(full_path, &statbuf) == -1) {
                perror("stat failed");
                continue;
            }

            printf((S_ISDIR(statbuf.st_mode)) ? "d" : "-");
            printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
            printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
            printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
            printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
            printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
            printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
            printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
            printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
            printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

            printf(" %ld ", statbuf.st_nlink);
            printf("%-8s ", getpwuid(statbuf.st_uid)->pw_name); // User name
            printf("%-8s ", getgrgid(statbuf.st_gid)->gr_name); // Group name
            printf("%8ld ", statbuf.st_size);
            printf("%s\n", entry->d_name);
        } else {
            printf("%s  ", entry->d_name);
        }
    }

    if (!long_format) {
        printf("\n");
    }

    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // No arguments, list current directory
        list_directory(".", 0, 0);
    } else {
        int long_format = 0;
        int show_all = 0;
        int start_index = 1;

        // Parse options
        if (argc >= 2 && argv[1][0] == '-') {
            int i;
            for (i = 1; i < argc; i++) {
                if (strcmp(argv[i], "-l") == 0) {
                    long_format = 1;
                } else if (strcmp(argv[i], "-a") == 0) {
                    show_all = 1;
                } else {
                    print_usage();
                    return EXIT_FAILURE;
                }
            }
            start_index = i;
        }

        if (start_index < argc) {
            for (int i = start_index; i < argc; i++) {
                printf("%s:\n", argv[i]);
                list_directory(argv[i], long_format, show_all);
                printf("\n");
            }
        } else {
            // No directory argument, list current directory
            list_directory(".", long_format, show_all);
        }
    }

    return EXIT_SUCCESS;
}
