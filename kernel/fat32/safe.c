#include <os/fat32.h>

/* success : read count */
int64 fat32_readmy(uchar *buf, size_t count)
{
    fd_t *fdp = &current_running->myelf_fd;
    size_t mycount = 0;
    assert(count < (1lu << 63)); /* cannot be too large */

    int64_t realcount = min((int64_t)count, (int64_t)(fdp->length) - (int64_t)(fdp->pos));
    if (realcount <= 0)
        return 0;

    uchar *buff = kalloc();

    ientry_t now_clus = get_clus_from_len(fdp->first_clus_num, fdp->pos);
    isec_t now_sec = get_sec_from_clus_and_offset(now_clus, fdp->pos % CLUSTER_SIZE);
    now_sec -= (now_sec - first_sec_of_clus(now_clus)) % READ_BUF_CNT; /* bufsize aligned */

    while (mycount < realcount){
        size_t pos_offset_in_buf = fdp->pos % BUFSIZE;
        size_t readsize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
        sd_read(buff, now_sec);
        memcpy(buf, buff + pos_offset_in_buf, readsize);
        buf += readsize;
        mycount += readsize;
        fdp->pos += readsize;
        if (fdp->pos % CLUSTER_SIZE == 0){
            now_clus = get_next_cluster(now_clus);
            now_sec = first_sec_of_clus(now_clus);
        }
        else{
            now_sec += READ_BUF_CNT ; // readsize / BUFSIZE == READ_BUF_CNT until last read
        }
    }

    kfree(buff);
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
