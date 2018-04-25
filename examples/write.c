
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
