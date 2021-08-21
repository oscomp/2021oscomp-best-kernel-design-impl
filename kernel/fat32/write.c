#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <os/sched.h>
#include <log.h>
#include <os/io.h>
#include <screen.h>

/* write truncated bytes to file */
static void write_truncate(fd_t *fdp)
{
    /* now cur_sec is set to be buff aligned position, acoording to fat32_lseek */
    size_t mycount = 0;
    size_t realcount = fdp->pos - fdp->length; // write as many as possible
    size_t temp_pos = fdp->length; /* start from length, temp_pos is in accordance with cur_sec */
    uchar *tempbuff = kalloc();

    while (mycount < realcount){
        size_t pos_offset_in_buf = temp_pos % BUFSIZE;
        size_t writesize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
        /* still need to read-in to reserve non-truncated area, and set 0 to truncated area */
        sd_read(tempbuff, fdp->cur_sec);
        memset(tempbuff + pos_offset_in_buf, 0, writesize);
        sd_write(tempbuff,fdp->cur_sec);
        mycount += writesize;
        temp_pos += writesize;
        if (temp_pos % CLUSTER_SIZE == 0){
            ientry_t old_clus = clus_of_sec(fdp->cur_sec);
            ientry_t new_clus = get_next_cluster(old_clus);
            if (new_clus == LAST_CLUS_OF_FILE){
                // new clus should be assigned
                new_clus = search_empty_clus(tempbuff);
                write_fat_table(old_clus, new_clus, tempbuff);
            }
            fdp->cur_sec = first_sec_of_clus(new_clus);
        }
        else
            fdp->cur_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
    }
    kfree(tempbuff);
    /* no need to update length, fat32_write will do that (no matter whether really write something )*/
}

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
int64 fat32_write(fd_num_t fd, uchar *buff, uint64_t count)
{
    debug();
    log(0, "fd:%d, count:%d", fd, count);
    int32_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;
    log(0, "fd_index:%d, dev:%d", fd_index, current_running->fd[fd_index].dev);

    fd_t *fdp = &current_running->fd[fd_index];

    if (fdp->dev == STDOUT){    
        return write_ring_buffer(&stdout_buf, buff, count);
    }
    else if (fdp->dev == STDERR)
    {
        return write_ring_buffer(&stderr_buf, buff, count);
    }
    else if (fdp->dev == DEV_NULL){
        log(0, "it's NULL");
        memset(buff, 0, count);
        return count;
    } 
    else if (fdp->piped == FD_PIPED){ // 如果是管道，就调用pipe_write
        return pipe_write(buff, fdp->pip_num, count);
    }
    else{

        if (fdp->pos > fdp->length){
            /* now cur_sec is not valid (in accordance with length, not with pos) */
            write_truncate(fdp);
        }

        /* now cur_sec is valid */
        size_t mycount = 0;
        size_t realcount = count; // write as many as possible
        uchar *tempbuff = kalloc();

        while (mycount < realcount){
            size_t pos_offset_in_buf = fdp->pos % BUFSIZE;
            size_t writesize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
            log(0, "this time pos is %d, writesize is %d", fdp->pos, writesize);
            sd_read(tempbuff, fdp->cur_sec);
            memcpy(tempbuff + pos_offset_in_buf, buff, writesize);
            sd_write(tempbuff,fdp->cur_sec);
            buff += writesize;
            mycount += writesize;
            fdp->pos += writesize;
            if (fdp->pos % CLUSTER_SIZE == 0){
                ientry_t old_clus = clus_of_sec(fdp->cur_sec);
                ientry_t new_clus = get_next_cluster(old_clus);
                if (new_clus == LAST_CLUS_OF_FILE){
                    // new clus should be assigned
                    new_clus = search_empty_clus(tempbuff);
                    write_fat_table(old_clus, new_clus, tempbuff);
                }
                fdp->cur_sec = first_sec_of_clus(new_clus);
            }
            else
                fdp->cur_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
        }
        /* update length */
        if (fdp->pos > fdp->length){
            /* update length in file descriptor */
            fdp->length = fdp->pos;
            /* read dentry */
            sd_read(tempbuff, fdp->dir_pos.sec);
            dentry_t *p = tempbuff + fdp->dir_pos.offset;
            /* update length in dentry */
            set_dentry_from_fd(p, fdp);
            /* write dentry back */
            sd_write(tempbuff, fdp->dir_pos.sec);
        }

        kfree(tempbuff);

        return realcount;
    }
}

/* write iov to file in fd */
/* return count: success
          -1: fail
        */
int64 fat32_writev(fd_num_t fd, struct iovec *iov, int iovcnt)
{
    debug();
    log(0, "fd:%d, iovcnt:%d", fd, iovcnt);
    // int32_t fd_index = get_fd_index(fd, current_running);
    // if (fd_index < 0) return -1;

    size_t count = 0;

    // if (current_running->fd[fd_index].dev == STDOUT){  
    //     for (uint32_t i = 0; i < iovcnt; i++){
    //         log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);      
    //         count += write_ring_buffer(&stdout_buf, iov->iov_base, iov->iov_len); /* FOR NOW */
    //         iov++;
    //     }
    //     return count;
    // }
    // else if (current_running->fd[fd_index].dev == STDERR){
    //     for (uint32_t i = 0; i < iovcnt; i++){  
    //         log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);     
    //         count += write_ring_buffer(&stderr_buf, iov->iov_base, iov->iov_len);
    //         iov++;
    //     }
    //     return count;
    // }
    // else{
        ssize_t this_count;
        for (uint32_t i = 0; i < iovcnt; i++){
            if ((this_count = fat32_write(fd, iov->iov_base, iov->iov_len)) == SYSCALL_FAILED)
                return SYSCALL_FAILED;
            count += this_count;
            log(0, "count is %d", count);
            iov++;
        }
        return count; 
    // }
}