
/*
MIT License

Copyright (c) 2018 Krzysztof Szewczyk

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/* Utility for converting number macros to string macros. */
#define xstr(s) str(s)
#define str(s) #s

#include <targoviste.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* *** Internal functions *** */

/*
 * Function:  readString
 * --------------------
 * Reads a string from the given file stream with the specified size
 * and delimiter. Returns a pointer to a string allocated on the heap.
 * Remember to free the pointer afterwards. The returned string contains
 * the delimiter.
 *
 *  f: the file handle
 *  maxSize: max string size (to prevent BO attacks)
 *  delim: the string delimiter
 *
 *  returns: the resulting string read from the stream
 */
static char * readString(FILE * f, int maxSize, char delim) {
    char * s, c;
    int i;
    
    #ifndef CAST_MALLOC
        s = malloc(maxSize+1);
    #else
        s = (char *) malloc(maxSize+1);
    #endif
    
    if(!s) return NULL;
    
    for(i = 0; i < maxSize; i++) {
        c = fgetc(f);
        if(c == delim) return (s[i] = 0, ungetc(c, f), s);
        else s[i] = c;
    }
    
    return s;
}

/*
 * Function:  readi
 * --------------------
 * Reads an integer from the given file stream.
 * This function uses readString().
 *
 *  f: the file handle
 *
 *  returns: the integer read from the stream
 */
static int readi(FILE * f) {
    char * s;
    int num;
    s = readString(f, 16, 0);
    fgetc(f);
    num = atoi(s);
    free(s);
    return num;
}

/* *** End: Internal functions *** */

/* *** External functions *** */

int writeArchive(targoviste_archive archive, char * file) {
    FILE * f;
    int i = 0, totalSize = 0;
    if(!(f = fopen(file, "wb"))) return 1;
    
    fprintf(f, "TAR%d%c", archive.amount, 0);
    
    for(i = 0; i < archive.amount; i++, totalSize += archive.files[i].size) {
        fwrite(archive.files[i].name, 1, strlen(archive.files[i].name)+1, f);
        fprintf(f, "%d%c%d%c", totalSize, 0, archive.files[i].size, 0);
    }
    
    for(i = 0; i < archive.amount; i++)
        fwrite(archive.files[i].buffer, 1, archive.files[i].size, f);
    
    fclose(f);
    return 0;
}

targoviste_archive readArchive(char * file, int * error) {
    FILE * f;
    targoviste_archive archive;
    int size, i, j, header_end, * startOffsets;
    char * nbuf, * fbuf;
    
    f = fopen(file, "rb");
    if(!f)
        return (*error = 1, archive);
    
    if(fgetc(f)!='T' || fgetc(f)!='A' || fgetc(f)!='R')
        return (*error = 3, archive);
    
    size = readi(f);
    archive.amount = size;

    #ifndef CAST_MALLOC
        archive.files = calloc(sizeof(targoviste_file), size);
        startOffsets = calloc(sizeof(int), size);
    #else
        archive.files = (targoviste_file *) calloc(sizeof(targoviste_file), size);
        startOffsets = (int *) calloc(sizeof(int), size);
    #endif
    
    for(i = 0; i < size; i++) {
        #ifndef CAST_MALLOC
            nbuf = malloc(MAX_FILENAME_LEN);
        #else
            nbuf = (char *) malloc(MAX_FILENAME_LEN);
        #endif
        if(!nbuf)
            return (fclose(f), *error = 2, archive);
        nbuf = readString(f, MAX_FILENAME_LEN, 0);
        fgetc(f);
        archive.files[i].name = nbuf;
        startOffsets[i] = readi(f)+3;
        archive.files[i].size = readi(f);
    }
    
    header_end = ftell(f);
    
    for(j = 0; j < size; j++) {
        #ifndef CAST_MALLOC
            fbuf = malloc(archive.files[j].size);
        #else
            fbuf = (char *) malloc(archive.files[j].size);
        #endif
        if(!fbuf)
            return (fclose(f), *error = 4, archive);
        fseek(f, header_end+startOffsets[j], SEEK_SET);
        fread(fbuf, archive.files[j].size, 1, f);
        archive.files[j].buffer = fbuf;
    }
    
    fclose(f);
    *error = 0;
    return archive;
}

void freeArchive(targoviste_archive * archive) {
    int i;
    for(i = 0; i < archive->amount; i++)
        free(archive->files[i].buffer),free(archive->files[i].name);
    free(archive->files);
}


int loadFileFromArchive(targoviste_file * file, char * filename) {
    FILE * f;
    int i, size, startOffset;
    
    char * filenameBuffer;
    char * contentBuffer;
    
    f = fopen(filename, "rb");
    if(!f)
        return 1;
    
    if(fgetc(f)!='T' || fgetc(f)!='A' || fgetc(f)!='R')
        return 3;
    
    readi(f);

    for(i = 0; i < size; i++) {
        #ifndef CAST_MALLOC
            filenameBuffer = malloc(MAX_FILENAME_LEN);
        #else
            filenameBuffer = (char *) malloc(MAX_FILENAME_LEN);
        #endif
        
        if(!filenameBuffer) return 2;
        
        filenameBuffer = readString(f, MAX_FILENAME_LEN, 0);
        fgetc(f);
        
        if(!strcmp(filename, filenameBuffer)) break;
        readi(f); readi(f);
    }
    
    startOffset = readi(f) + 3;
    size = readi(f);
    #ifndef CAST_MALLOC
        contentBuffer = malloc(size);
    #else
        contentBuffer = (char *) malloc(size);
    #endif
    if(!contentBuffer) return 4;
    fseek(f, startOffset, SEEK_SET);
    fread(contentBuffer, size, 1, f);
    file->size = size;
    file->name = filenameBuffer;
    file->buffer = contentBuffer;
    return 0;
}

targoviste_file * listFilesArchive(char * path, int * error, int * pSize) {
    FILE * f;
    int amount, size, i;
    char * filenameBuffer;
    targoviste_file * files;
    
    f = fopen(path, "rb");
    if(!f) return (*error = 1, NULL);
    
    if(fgetc(f)!='T' || fgetc(f)!='A' || fgetc(f)!='R')
        return (*error = 3, NULL);
    
    amount = readi(f);
    
    #ifndef CAST_MALLOC
        files = malloc(sizeof(targoviste_file) * amount);
    #else
        files = (targoviste_file *) malloc(sizeof(targoviste_file) * amount);
    #endif
    
    *pSize = amount;
    
    for(i = 0; i < amount; i++) {
        #ifndef CAST_MALLOC
            filenameBuffer = malloc(MAX_FILENAME_LEN);
        #else
            filenameBuffer = (char *) malloc(MAX_FILENAME_LEN);
        #endif
        
        if(!filenameBuffer) return (*error = 2, NULL);
        
        filenameBuffer = readString(f, MAX_FILENAME_LEN, 0);
        fgetc(f); readi(f); size = readi(f);
        files[i].buffer = NULL;
        files[i].size = size;
        files[i].name = filenameBuffer;
    }
    
    fclose(f);
    return files;
}

/** *** End: External functions *** */
