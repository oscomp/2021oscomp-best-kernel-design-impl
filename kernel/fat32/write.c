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

    if (current_running->fd[fd_index].dev == STDOUT){    
        return write_ring_buffer(&stdout_buf, buff, count);
    }
    else if (current_running->fd[fd_index].dev == STDERR)
    {
        return write_ring_buffer(&stderr_buf, buff, count);
    }
    else{
        // 如果是管道，就调用pipe_write
        if (current_running->fd[fd_index].piped == FD_PIPED){
            return pipe_write(buff, current_running->fd[fd_index].pip_num, count);
        }

        size_t mycount = 0;
        size_t realcount = count; // write as many as possible
        uchar *tempbuff = kalloc();
        ientry_t now_clus = get_clus_from_len(current_running->fd[fd_index].first_clus_num, current_running->fd[fd_index].pos);
        isec_t now_sec = BUFF_ALIGN( get_sec_from_clus_and_offset(now_clus, current_running->fd[fd_index].pos % CLUSTER_SIZE) );
        log(0, "sec is %d", now_sec);
        ientry_t old_clus = now_clus;

        while (mycount < realcount){
            size_t pos_offset_in_buf = current_running->fd[fd_index].pos % BUFSIZE;
            size_t writesize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
            log(0, "this time pos is %d, writesize is %d", current_running->fd[fd_index].pos, writesize);
            sd_read(tempbuff,now_sec);
            memcpy(tempbuff + pos_offset_in_buf, buff, writesize);
            sd_write(tempbuff,now_sec);
            buff += writesize;
            mycount += writesize;
            current_running->fd[fd_index].pos += writesize;
            if (current_running->fd[fd_index].pos % CLUSTER_SIZE == 0){
                old_clus = now_clus;
                now_clus = get_next_cluster(now_clus);
                if (now_clus == LAST_CLUS_OF_FILE){
                    // new clus should be assigned
                    now_clus = search_empty_clus(tempbuff);
                    write_fat_table(old_clus, now_clus, tempbuff);
                }
                now_sec = first_sec_of_clus(now_clus);
            }
            else
                now_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
        }
        if (current_running->fd[fd_index].pos > current_running->fd[fd_index].length){
            fd_t *fdp = &current_running->fd[fd_index];
            fdp->length = fdp->pos;
            /* update dentry */
            log(0, "dir info: sec :%d, offset: %lx, len: %d", fdp->dir_pos.sec, fdp->dir_pos.offset, fdp->dir_pos.len);
            printk_port("tid:%d, is_piped:%d\n", current_running->tid, fdp->piped);
            sd_read(tempbuff, fdp->dir_pos.sec);
            dentry_t *p = tempbuff + fdp->dir_pos.offset;
            log(0, "filename: %s, new length is %d", p->filename, fdp->length);
            set_dentry_from_fd(p, fdp);
            log(0, "dentry new length is %d", p->length);
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
    int32_t fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0) return -1;

    size_t count = 0;

    if (current_running->fd[fd_index].dev == STDOUT){  
        for (uint32_t i = 0; i < iovcnt; i++){
            log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);      
            count += write_ring_buffer(&stdout_buf, iov->iov_base, iov->iov_len); /* FOR NOW */
            iov++;
        }
        return count;
    }
    else if (current_running->fd[fd_index].dev == STDERR){
        for (uint32_t i = 0; i < iovcnt; i++){  
            log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);     
            count += write_ring_buffer(&stderr_buf, iov->iov_base, iov->iov_len);
            iov++;
        }
        return count;
    }
    else{
        size_t this_count;
        for (uint32_t i = 0; i < iovcnt; i++){
            if ((this_count = fat32_write(fd, iov->iov_base, iov->iov_len)) == SYSCALL_FAILED)
                return SYSCALL_FAILED;
            count += this_count;
            log(0, "count is %d", count);
            iov++;
        }
        return count; 
    }
}