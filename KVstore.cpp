#include<bits/stdc++.h>
using namespace std;
#define ll unsigned long long int
#include<fstream>
#include "bTree.h"
 #include <sys/mman.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdint>

#define DB_SIG = "aniketDB"

#define PERM 0644

struct KV_struct{
    string path;
    int fd;
    btree::BTree tree;
    struct mmap{
        int file_size; //file size can be larger than db size
        int total_size;// mmap size can be larger than file size
        vector<void*> chunks; //multiple mmaps can be non continous blcok of memory as well
    };
    struct page{
        ll flushed; //database size in number of pages
        vector<void*> temp; //newly allocated pages
    };
    mmap *mmp1;
    page *pg1;
};

//we need to extend mmap by adding new mappings

void ExtendMMAP(KV_struct *db , int npages){
    size_t fsize = npages * BTREE_PAGE_SIZE;
    if (db->mmp1->total_size >= fsize){
        return;    
    }
    // void *mmap(void *addr, size_t len, int prot, int flags,
    //            int fildes, off_t off);
    // db->fp.fd();
    // int fd = open(db->fp,O_CREAT|O_RDWR | O_TRUNC,PERM); // opening or creating a file
    // void *addr = mmap(NULL, fsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);//mapping
    int totalSize = db->mmp1->total_size;
    void *addr = mmap(nullptr, fsize - totalSize, PROT_READ | PROT_WRITE, MAP_SHARED, db->fd, totalSize);
    if (addr == MAP_FAILED)
    {
        cout << "mmap failed during extension" << endl;
        return;
    }
    db->mmp1->total_size += db->mp1->total_size;
    db->mmp1->chunks.push_back(addr);
}

//now adding into mmap can be grown exponentially

// void

    // creating master paage
    // master page store reference to the root node

    // the masterpage format.
    // it contains the pointer to the root and other important bits.
    //|sig |btree_root |page_used |
    //|16B | 8B | 8B |

    //In db master page contain meta data not user data
    void masterLoad(KV_struct *db){
       if(db->mmp1->file_size==0){
        //empty file so master page needs to be created on first write
        db->pg1->flushed=1;//reserved for master page
        return nullptr;
       }
       data = db->mmp1->chunks[0];
       uint64_t root;
       memccpy(&root, &data[16], sizeof(uint64_t));
       uint64_t used;
       memccpy(&used,&data[24],sizeof(uint64_t);
       if(memcmp(DB_SIG,data[16],16)!=0){
        return "BAD signature";
        exit 1;
       }
       bad = !(used>=1 && used<=(uint64_t)(db->mmp1->file_size/BTREE_PAGE_SIZE));
       bad = bad || !(root>=0 && root<=used);
       if(bad){
        return "BAD MASTER PAGE";
        exit 1;
       }
       db->btree->root=root;
       db->pg1->flushed=used;
       return nullptr;
    }

    //updating master page it should be atomic
void updateMasterPage(KV_struct *db){
    try{
        vector<void *> data;
        memccpy(&data[16], DB_SIG);
        memcpy(&data[16], &db->tree->root, sizeof(db->tree->root));
        memcpy(&data[24], &db->pg1->flushed, sizeof(db->pg1->flushed));
        //NOTE:Updatingthe page via mmap isnot atomic.
// Use the `pwrite()` syscallinstead
        pwrite(db->fd, data.data, data.size(), 0);
    }
    catch(exception e){
        throw e;
    }
}

//allocating disk pages

//simply now add new page at the end of db & new pages are added continously in memory until copied to file later

