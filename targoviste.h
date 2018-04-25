
#ifndef __TARGOVISTE_H_
#define __TARGOVISTE_H_

#define MAX_FILENAME_LEN 256
#define CAST_MALLOC

typedef struct {
    char * buffer;
    char * name;
    long   size;
} targoviste_file;

typedef struct {
    targoviste_file * files;
    int amount;
} targoviste_archive;

int writeArchive(targoviste_archive archive, char * file);
targoviste_archive readArchive(char * file, int *error);
void cleanArchive(targoviste_archive archive);

#endif
