
#include <targoviste.h>
#include <stdio.h>

int main() {
	int error = 0, i = 0;
    targoviste_archive a = readArchive("archive.tgx", &error);
    printf("Files in archive.tgx: %d\n", a.amount);
    for(; i < a.amount; i++)
        printf("%s - %d bytes\n", a.files[i].name, a.files[i].size);
}
