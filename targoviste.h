
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
 * Writes a targoviste_archive structure to the file
 * with the given filename.
 * 
 *  archive: the instance of targoviste_archive structure to be written
 *  file: the file to write to
 *
 *  returns: 1 on failure, 0 on success
 */
int writeArchive(targoviste_archive archive, char * file);


/*
 * Function:  readArchive
 * --------------------
 * Reads a targoviste_archive structure from the file
 * with the given filename.
 *
 *  file: the file to write to
 *  error: a pointer to an integer variable,
 *      whose value indicates reading errors and can be:
 *            0 (everything fine)
 *            1 (file could not be opened)
 *            2 (out of memory)
 *            3 (invalid header)
 *            4 (out of memory)
 *
 *  returns: the archive read from the file
 */
targoviste_archive readArchive(char * file, int *error);


/*
 * Function:  loadFileFromArchive
 * --------------------
 * Loads a single file with the specified filename
 * from the given targoviste_file.
 *
 *  file: a file pointer that the output file will be assigned to
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
 * Free targoviste_archive structure
 */
void freeArchive(targoviste_archive * archive);

/*
 * Function:  listFilesInArchive
 * --------------------
 * Returns an array of all files in the archive with the given path.
 *
 *  path: path to archive
 *  error: pointer to integer variable, which will be changed in
 *         case of an error to:
 *            0 (everything fine)
 *            1 (file could not be opened)
 *            2 (out of memory)
 *            3 (invalid header)
 *  amount: pointer to integer variable which will equal amount of
 *          files in archive
 *
 *  returns: array of files. The retured files have a buffer value of NULL.
 *              In case of an error, NULL is returned.
 */
targoviste_file * listFilesArchive(char * path, int * error, int * amount);

/*
 * Function:  createArchive
 * --------------------
 * Returns new instance of targoviste_archive.
 */
targoviste_archive createArchive();

/*
 * Function:  createFile
 * --------------------
 * Returns new instance of targoviste_file.
 */
targoviste_file createFile();

/*
 * Function:  addFileToArchive
 * --------------------
 * Adds file to specified archive. Does either:
 * 1. Allocate memory for 1 file (amount = 0)
 * 2. Reallocate memory for n files (amount != 0)
 *
 * returns 0 on success and 1 on error.
 */
void addFileToArchive(targoviste_archive dest, targoviste_file src);

#endif
