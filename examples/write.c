
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

#include <targoviste.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * str1 = "This is example file 1 content.";
char * str2 = "This is example file 2 content.";

int main() {
    targoviste_archive a;
    a.amount = 2;
    a.files = malloc(sizeof(targoviste_file)*2);
    memset(a.files, 0, sizeof(targoviste_file)*2);
    a.files[0].name = "file.txt";
    a.files[0].buffer = str1;
    a.files[0].size = strlen(str1);
    
    a.files[1].name = "file2.txt";
    a.files[1].buffer = str2;
    a.files[1].size = strlen(str2);
    
    writeArchive(a, "archive.tgx");
}
