#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void list_directory(const char *path) {
    struct dirent *entry;
    struct stat statbuf;

    DIR *dp = opendir(path);
    if (dp == NULL) {
        perror("opendir failed");
        return;
    }

    printf("Directory: %s\n", path);

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char full_path[1024];
        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (lstat(full_path, &statbuf) == -1) {
            perror("lstat failed");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            printf("  [DIR]  %s\n", entry->d_name);
            list_directory(full_path);
        } else if (S_ISREG(statbuf.st_mode)) { 
            printf("  FILE %s\n", entry->d_name);
        } else {
            printf("  [OTHER] %s\n", entry->d_name);
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return 1;
    }

    list_directory(argv[1]);

    return 0;
}