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

uchar stdout_buf[NORMAL_PAGE_SIZE] = {0};
uchar stderr_buf[NORMAL_PAGE_SIZE] = {0};

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
int64 fat32_write(fd_num_t fd, uchar *buff, uint64_t count)
{
    debug();
    log(0, "fd:%d, count:%d", fd, count);
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0 || !current_running->fd[fd_index].used)
        return -1;

    if (current_running->fd[fd_index].dev == STDOUT){        
        memcpy(stdout_buf, buff, count);
        stdout_buf[count] = 0;
        printk_port(stdout_buf);
        return count;
    }
    else if (current_running->fd[fd_index].dev == STDERR)
    {
        printk_port("ERROR: ");
        memcpy(stderr_buf, buff, count);
        stderr_buf[count] = 0;
        printk_port(stderr_buf);
        return count;
    }
    else{
        // 如果是管道，就调用pipe_write
        if (current_running->fd[fd_index].piped == FD_PIPED)
            return pipe_write(buff, current_running->fd[fd_index].pip_num, count);

        size_t mycount = 0;
        size_t realcount = count; // write as many as possible
        uchar *tempbuff = kalloc();
        ientry_t now_clus = get_clus_from_len(current_running->fd[fd_index].first_clus_num, current_running->fd[fd_index].pos);
        isec_t now_sec = get_sec_from_clus_and_offset(now_clus, current_running->fd[fd_index].pos % CLUSTER_SIZE);
        now_sec -= (now_sec - first_sec_of_clus(now_clus)) % READ_BUF_CNT; /* bufsize aligned */
        ientry_t old_clus = now_clus;

        while (mycount < realcount){
            size_t pos_offset_in_buf = current_running->fd[fd_index].pos % BUFSIZE;
            size_t writesize = min(BUFSIZE - pos_offset_in_buf, realcount - mycount);
            sd_read(tempbuff,now_sec);
            memcpy(tempbuff + pos_offset_in_buf, buff, writesize);
            sd_write(tempbuff,now_sec);
            buff += writesize;
            mycount += writesize;
            current_running->fd[fd_index].pos += writesize;
            if (current_running->fd[fd_index].pos % CLUSTER_SIZE == 0){
                old_clus = now_clus;
                now_clus = get_next_cluster(now_clus);
                if (now_clus == 0x0fffffffu){
                    // new clus should be assigned
                    now_clus = search_empty_clus(tempbuff);
                    write_fat_table(old_clus, now_clus, tempbuff);
                }
                now_sec = first_sec_of_clus(now_clus);
            }
            else
                now_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
        }
        current_running->fd[fd_index].length = max(current_running->fd[fd_index].pos, current_running->fd[fd_index].length);

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
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0) return -1;

    size_t count = 0;

    if (current_running->fd[fd_index].dev == STDOUT){  
        for (uint32_t i = 0; i < iovcnt; i++){
            log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);      
            memcpy(stdout_buf, iov->iov_base, min(iov->iov_len, NORMAL_PAGE_SIZE - 1));
            stdout_buf[iov->iov_len] = 0;
            printk_port(stdout_buf);
            count += iov->iov_len; /* FOR NOW */
            iov++;
        }
        return count;
    }
    else if (current_running->fd[fd_index].dev == STDERR){
        for (uint32_t i = 0; i < iovcnt; i++){  
            log(0, "iov_base:%lx, iov_len: %d", iov->iov_base, iov->iov_len);     
            printk_port("ERROR:");
            memcpy(stderr_buf, iov->iov_base, min(iov->iov_len, NORMAL_PAGE_SIZE - 1));
            stderr_buf[iov->iov_len] = 0;
            printk_port(stderr_buf);
            printk_port("\n");
            iov++; count += iov->iov_len; /* FOR NOW */
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