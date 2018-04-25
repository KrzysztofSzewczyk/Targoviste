
#include <targoviste.h>

#include <stdio.h>

int main() {
	int error = 0;
    targoviste_archive a = readArchive("archive.tgx", &error);
    printf("%s", a.files[1].buffer);
}
