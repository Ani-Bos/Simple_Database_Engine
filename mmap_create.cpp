#include<bits/stdc++.h>
#include<iostream>
#include<sys/mman.h>
#include<sys/stat.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>

#define PERM 0644
using namespace std;
//argv[1]==filename argv[2]==data message to write
int main(int argc , char *argv[]){
    if(argc!=3){
        cout << "usage: mmap_create <file-name> <message>" << endl;
        return 1;
    }
    //include the '\0' character as well so +1
    size_t fsize = strlen(argv[2]) + 1;
    //file descriptor
    int fd = open(argv[1], O_CREAT | O_RDWR | O_TRUNC, PERM) ;// first read the file
    cout << "File is --> " << argv[1] << " <--- size" <<fsize << endl;

    lseek(fd, fsize - 1, SEEK_SET); //step2 set file size

    write(fd, "\n", 1);
    void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//mapping
    //mmap(0)--kernel choosed the addres
    //mmap(fsize)--size of mapping , MAP_SHARED: writes go back to file

    //not writting to file directly writing to process virtual memory then kernel
    //later mark pages dirty and flushes ans sync it to disk
    memcpy(addr, argv[2], strlen(argv[2]));
    msync(addr, fsize, MS_SYNC);//syncing memory to file
    munmap(addr, fsize); //unmapping
    close(fd);//closing the file

    return 0;
}