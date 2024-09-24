#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>


void list_files() {
    struct dirent *entry;
    DIR *dp = opendir(".");

    if (dp == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        printf("%s\n", entry->d_name);
    }

    closedir(dp);
}

void copy_file(const char *src, const char *dest) {
    FILE *source = fopen(src, "rb");
    FILE *destination = fopen(dest, "wb");

    if (source == NULL || destination == NULL) {
        perror("fopen");
        if (source) fclose(source);
        return;
    }

    char buffer[1024];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), source)) > 0) {
        fwrite(buffer, 1, bytes, destination);
    }

    fclose(source);
    fclose(destination);
}

void remove_file(const char *filename) {
    if (remove(filename) == 0) {
        printf("Deleted successfully: %s\n", filename);
    } else {
        perror("remove");
    }
}

int main() {
    char command[256];
    char arg1[128], arg2[128];

    while (1) {
        printf("simple_shell> ");
        fgets(command, sizeof(command), stdin);

        // Remove trailing newline
        command[strcspn(command, "\n")] = 0;

        // Parse command
        if (sscanf(command, "ls") == 0) {
            list_files();
        } else if (sscanf(command, "cp %s %s", arg1, arg2) == 2) {
            copy_file(arg1, arg2);
        } else if (sscanf(command, "rm %s", arg1) == 1) {
            remove_file(arg1);
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
    
    
}




