
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

/* Utility for converting number macros to string ones. */
#define xstr(s) str(s)
#define str(s) #s

#include <targoviste.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* *** Internal functions block starts *** */

/*
 * Function:  readString
 * --------------------
 * This function is reading string from specified stream with specified size and
 * delimiter. This function is returning pointer to string allocated on heap. Please
 * remember to free pointer. Warning: does not consume the delimiter.
 *
 *  f: File handle
 *  maxSize: Max string size (to get rid of BO attacks)
 *  delim: Delimiter
 *
 *  returns: string that was read from stream.
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
 * Function:  reado
 * --------------------
 * This function is reading number from specified stream. Does
 * consume null terminator. This is wrapper over readString.
 * Function is returning string pointer allocated on heap like
 * readString, so please free it after it's no longer used.
 *
 *  f: File handle
 *
 *  returns: number that was read from stream.
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


/* *** Internal functions block ends *** */

/* *** External functions block starts *** */

/*
 * Function:  writeArchive
 * --------------------
 * This function is writing targoviste_archive structure to
 * specified filename. 
 * 
 *  archive: instance of targoviste_archive structure that
 *           needs to be written.
 *  file: file to write to.
 *
 *  returns: 1 on failure, 0 on success.
 */
int writeArchive(targoviste_archive archive, char * file) {
    FILE * f = fopen(file, "wb");
    int i = 0, totalSize = 0;
    if(!f) return 1;
    
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

/*
 * Function:  readArchive
 * --------------------
 * This function is reading whole targoviste_archive structure from
 * specified filename. 
 *
 *  file: file to write to.
 *  error: pointer to integer variable, which will be changed in
 *         case of error to either:
 *            0 (everything fine)
 *            1 (file could not be opened)
 *            2 (out of memory)
 *            3 (invalid header)
 *            4 (out of memory)
 *
 *  returns: archive read.
 */
targoviste_archive readArchive(char * file, int *error) {
    FILE * f;
    targoviste_archive archive;
    int size, i, j, hst, * startOffsets;
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
    
    hst = ftell(f);
    
    for(j = 0; j < size; j++) {
        #ifndef CAST_MALLOC
            fbuf = malloc(archive.files[j].size);
        #else
            fbuf = (char *) malloc(archive.files[j].size);
        #endif
        if(!fbuf)
            return (fclose(f), *error = 4, archive);
        fseek(f, hst+startOffsets[j], SEEK_SET);
        fread(fbuf, archive.files[j].size, 1, f);
        archive.files[j].buffer = fbuf;
    }
    
    fclose(f);
    *error = 0;
    return archive;
}

/*
 * Function:  freeArchive
 * --------------------
 * This function is reading whole targoviste_archive structure from
 * specified filename. 
 *
 *  file: file to write to.
 *  error: pointer to integer variable, which will be changed in
 *         case of error to either:
 *            0 (everything fine)
 *            1 (file could not be opened)
 *            2 (out of memory)
 *            3 (invalid header)
 *            4 (out of memory)
 *
 *  returns: archive read.
 */
void freeArchive(targoviste_archive archive) {
    int i;
    for(i = 0; i < archive.amount; i++)
        free(archive.files[i].buffer),free(archive.files[i].name);
    free(archive.files);
}

/*
 * Function:  loadFileFromArchive
 * --------------------
 * This function is loading single file from specified archive file.
 *
 *  file: target file
 *  filename: name of archive
 *
 *  returns: integer variable, which will be changed in
 *           case of error to either:
 *             0 (everything fine)
 *             1 (file could not be opened)
 *             2 (out of memory)
 *             3 (invalid header)
 *             4 (out of memory)
 */
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
