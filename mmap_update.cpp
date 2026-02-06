#include <bits/stdc++.h>
#include <iostream>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define PERM 0644
using namespace std;

int main(int argc, char *argv[])
{
    struct stat filestatus;
    if (argc != 3)
    {
        cout << "usage: mmap_update <file-name> <message>" << endl;
        return 1;
    }

    // file descriptor
    size_t length = strlen(argv[2]) + 1;
    int fd = open(argv[1], O_RDWR , PERM);  // first read the file
    fstat(fd, &filestatus);            // get file size
    size_t fsize = filestatus.st_size; // get the size 9of file in bytes
    cout << "File is --> " << argv[1] << " <--- size" << fsize << endl;
    void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0); // mapping
    cout << "READ is --> "<<(char)*addr << endl;
    mcpy(addr, argv[2], strlen(argv[2]));
    cout << "After update us --> " << (char)*addr << endl;
    msync(addr, fsize, MS_SYNC); // syncing memory to file
    munmap(addr, fsize); // unmapping
    close(fd);           // closing the file

    return EXIT_SUCCESS;
}