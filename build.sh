
#!/bin/sh

gcc -I. -L. -c targoviste.c -o targoviste.o
ar rcs libtargoviste.a targoviste.o

gcc -I. -L. examples/read.c -o examples/read -ltargoviste
gcc -I. -L. examples/write.c -o examples/write -ltargoviste

echo "Build done!"

exit 0