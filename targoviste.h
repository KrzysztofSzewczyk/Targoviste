
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
int writeArchive(targoviste_archive archive, char * file);


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
targoviste_archive readArchive(char * file, int *error);


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
int loadFileFromArchive(targoviste_file * file, char * filename);


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
void freeArchive(targoviste_archive archive);

#endif
