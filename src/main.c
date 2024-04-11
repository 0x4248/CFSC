/* CFSC
 * C File Size Calculator
 * Github://www.github.com/0x4248/CFSC
 * Licence: GNU General public licence v3.0
 * By: 0x4248
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <threads.h>
#include <unistd.h>

#define MAX_FILES 10000

typedef struct {
    char *name;
    int size;
} file_t;

void spinner(void *arg) {
    char spinner[] = {'|', '/', '-', '\\'};
    int i = 0;
    while (1) {
        printf("\x0D%s %c", (char *)arg, spinner[i]);
        fflush(stdout);
        i = (i + 1) % 4;
        usleep(250000);
    }
}

void spinner_cleanup() {
    printf("\x0D");
    printf("\x1B[2K");
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    file_t files[MAX_FILES];
    int i, j, nfiles = 0;
    char *path;
    if (argc == 1) {
        path = ".";
    } else {
        path = argv[1];
    }

    if (argc > 1) {
        path = argv[1];
    }

    thrd_t spinner_thread;
    thrd_create(&spinner_thread, spinner, "Calculating");

    if ((dir = opendir(path)) == NULL) {
        perror("opendir");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') {
            continue;
        }

        char *fullpath = malloc(strlen(path) + strlen(entry->d_name) + 2);
        sprintf(fullpath, "%s/%s", path, entry->d_name);

        if (stat(fullpath, &statbuf) == -1) {
            perror("Could not stat file");
            exit(1);
        }

        if (S_ISREG(statbuf.st_mode)) {
            files[nfiles].name = strdup(entry->d_name);
            files[nfiles].size = statbuf.st_size;
            nfiles++;
        }
    }
    thrd_detach(spinner_thread);
    spinner_cleanup();
    closedir(dir);

    for (i = 0; i < nfiles; i++) {
        for (j = i + 1; j < nfiles; j++) {
            if (files[i].size < files[j].size) {
                file_t tmp = files[i];
                files[i] = files[j];
                files[j] = tmp;
            }
        }
    }

    for (i = 0; i < nfiles; i++) {
        if (strlen(files[i].name) > 7) {
            printf("%s\t%d\n", files[i].name, files[i].size);
        } else {
            printf("%s\t\t%d\n", files[i].name, files[i].size);
        }
    }
    return 0;
}
