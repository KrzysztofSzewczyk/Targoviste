
#include <targoviste.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int writeArchive(targoviste_archive archive, char * file) {
    FILE * f;
    int i = 0;
    int totalSize = 0;

    f = fopen(file, "wb");
    if(!f) return -1;
    
    fprintf(f, "TAR");
    
    fprintf(f, "%d%c", archive.amount, 0);
    
    for(i = 0; i < archive.amount; i++) {
        fwrite(archive.files[i].name, 1, strlen(archive.files[i].name), f);
        fputc(0, f);
        fprintf(f, "%d%c", totalSize, 0);
        fprintf(f, "%d%c", archive.files[i].size, 0);
        totalSize += archive.files[i].size;
    }
    
    for(i = 0; i < archive.amount; i++)
        fwrite(archive.files[i].buffer, 1, archive.files[i].size, f);
    
    fclose(f);
    return 0;
}

static int readi(FILE * f) {
	char s[16];
	fscanf(f, "%[^\0]s", s);
	fgetc(f);
	return atoi(s);
}

targoviste_archive readArchive(char * file, int *error) {
	FILE * f;
	targoviste_archive archive;
	int size, i, j, hst, * startOffsets;
	char * nbuf, * fbuf;
	
	f = fopen(file, "rb");
	if(!f) {
		*error=1;
		return archive;
	}
	
	size = readi(f);
	archive.amount = size;
    
    if(fgetc(f)=='T' && fgetc(f)=='A' && fgetc(f)=='R');
    else {
        *error = 3;
        return archive;
    }
	
	#ifndef CAST_MALLOC
    	archive.files = calloc(sizeof(targoviste_file), size);
	#else
		archive.files = (targoviste_file *) calloc(sizeof(targoviste_file), size);
	#endif
	
	#ifndef CAST_MALLOC
    	startOffsets = calloc(sizeof(int), size);
	#else
		startOffsets = calloc(sizeof(int), size);
	#endif
	
	
	for(i = 0; i < size; i++) {
		#ifndef CAST_MALLOC
			nbuf = malloc(MAX_FILENAME_LEN);
		#else
			nbuf = (char *) malloc(MAX_FILENAME_LEN);
		#endif
		if(!nbuf) {
			*error = 2;
			return archive;
		}
		fscanf(f, "%[^\0]s", nbuf);
		fgetc(f);
		archive.files[i].name = nbuf;
		startOffsets[i] = readi(f);
		archive.files[i].size = readi(f);
	}
	
	hst = ftell(f);
	
	for(j = 0; j < size; j++) {
		#ifndef CAST_MALLOC
			fbuf = malloc(archive.files[j].size);
		#else
			fbuf = (char *) malloc(archive.files[j].size);
		#endif
		if(!fbuf) {
			*error = 3;
			return archive;
		}
		fseek(f, hst+startOffsets[j], SEEK_SET);
		fread(fbuf, archive.files[j].size, 1, f);
		archive.files[j].buffer = fbuf;
	}
	
	fclose(f);
	return archive;
}

void cleanArchive(targoviste_archive archive) {
    int i;
    for(i = 0; i < archive.amount; i++)
        free(archive.files[i].buffer),free(archive.files[i].name);
    free(archive.files);
}
