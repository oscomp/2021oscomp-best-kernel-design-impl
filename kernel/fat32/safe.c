#include <os/fat32.h>
#include <user_programs.h>
#include <os/elf.h>

static void fat32_read_myelf()
{
    assert(current_running->myelf_fd.first_clus_num > 0);

    uchar *buff = kalloc();

    ientry_t now_clus = current_running->myelf_fd.first_clus_num;
    isec_t now_sec = first_sec_of_clus(now_clus);
    uintptr_t elf_point = allocproc();
    size_t readcount = 0, length = current_running->myelf_fd.length;

    while (readcount < length){
        size_t readsize = min(BUFSIZE, length - readcount);
        sd_read(buff, now_sec);
        memcpy(elf_point, buff, readsize);
        elf_point += readsize;
        readcount += readsize;
        if (readcount % CLUSTER_SIZE == 0){
            now_clus = get_next_cluster(now_clus);
            now_sec = first_sec_of_clus(now_clus);
        }
        else{
            now_sec += READ_BUF_CNT ; // readsize / BUFSIZE == READ_BUF_CNT until last read
        }
    }
    kfree(buff);
}

static void fat32_set_elf()
{
    if (this_elf_first_clus_num != current_running->myelf_fd.first_clus_num){
        fat32_read_myelf();
        this_elf_first_clus_num = current_running->myelf_fd.first_clus_num;
    }
}

/* success : read count */
int64 fat32_readmy(uchar *buf, size_t count)
{
    #ifndef K210
    uchar *elf_binary = NULL;
    int32_t length = -1;
    get_elf_file("lmbenchall", &elf_binary, &length);
    memcpy(buf, elf_binary + current_running->myelf_fd.pos, count);
    current_running->myelf_fd.length = length;
    return count;
    #endif

    fd_t *fdp = &current_running->myelf_fd;
    assert(count < (1lu << 63)); /* cannot be too large */
    int64_t realcount = min((int64_t)count, (int64_t)(fdp->length) - (int64_t)(fdp->pos));
    // log(0, "realcount :%lx", realcount);
    if (realcount <= 0)
        return 0;

    fat32_set_elf();
    uintptr_t start_point = allocproc() + fdp->pos;
    memcpy(buf, start_point, realcount);

    return realcount;
}

/* seek pos */
/* return pos if success, fail return -1 */
int64_t fat32_lseekmy(size_t off, uint32_t whence)
{
    fd_t *fdp = &current_running->myelf_fd;
    switch(whence){
        case SEEK_SET:
        fdp->pos = off;
        break;
        case SEEK_CUR:
        fdp->pos += off;
        break;
        case SEEK_END:
        fdp->pos = fdp->length + off;
        break;
        default: assert(0);
        break;
    }
    return fdp->pos;
}