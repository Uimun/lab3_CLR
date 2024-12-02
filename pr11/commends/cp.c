#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>

void print_usage() {
    printf("Usage: cp [-r] <source> <destination>\n");
}

int copy_file(const char *source, const char *destination) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        perror("Source file open failed");
        return -1;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Destination file open failed");
        close(src_fd);
        return -1;
    }

    char buffer[4096];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination file");
            close(src_fd);
            close(dest_fd);
            return -1;
        }
    }

    if (bytes_read == -1) {
        perror("Error reading from source file");
    }

    close(src_fd);
    close(dest_fd);
    return 0;
}

int copy_directory(const char *source, const char *destination) {
    DIR *dir = opendir(source);
    if (dir == NULL) {
        perror("Failed to open source directory");
        return -1;
    }

    struct dirent *entry;
    struct stat statbuf;
    char src_path[1024], dest_path[1024];

    if (mkdir(destination, 0755) == -1) {
        perror("Failed to create destination directory");
        closedir(dir);
        return -1;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(src_path, sizeof(src_path), "%s/%s", source, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", destination, entry->d_name);

        if (stat(src_path, &statbuf) == -1) {
            perror("Failed to stat file");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            // Recursively copy directories
            if (copy_directory(src_path, dest_path) == -1) {
                perror("Failed to copy directory");
                closedir(dir);
                return -1;
            }
        } else {
            // Copy files
            if (copy_file(src_path, dest_path) == -1) {
                closedir(dir);
                return -1;
            }
        }
    }

    closedir(dir);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage();
        return EXIT_FAILURE;
    }

    int recursive = 0;
    int source_index = 1;

    if (argc >= 4 && strcmp(argv[1], "-r") == 0) {
        recursive = 1;
        source_index = 2;
    }

    const char *source = argv[source_index];
    const char *destination = argv[source_index + 1];

    struct stat statbuf;
    if (stat(source, &statbuf) == -1) {
        perror("Source path error");
        return EXIT_FAILURE;
    }

    if (S_ISDIR(statbuf.st_mode)) {
        if (recursive) {
            if (copy_directory(source, destination) == -1) {
                return EXIT_FAILURE;
            }
        } else {
            fprintf(stderr, "Error: Source is a directory, use -r to copy directories.\n");
            return EXIT_FAILURE;
        }
    } else {
        if (copy_file(source, destination) == -1) {
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}