#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <os/sched.h>
#include <log.h>

uchar stdout_buf[NORMAL_PAGE_SIZE] = {0};
uchar stderr_buf[NORMAL_PAGE_SIZE] = {0};

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
int64 fat32_write(fd_num_t fd, uchar *buff, uint64_t count)
{
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0) return -1;

    // if (count == 0) return 0;
    if (current_running->fd[fd_index].dev == STDOUT){        
        // for (uint i = 0; i < count; ++i){
        //     sbi_console_putchar(*(buff + i));
        // }
        // printk_port("\npid %d is writing\n", current_running->pid);
        memcpy(stdout_buf, buff, count);
        stdout_buf[count] = 0;
        printk_port(stdout_buf);
        return count;
    }
    else{
        if (current_running->fd[fd_index].piped == FD_PIPED)
            return pipe_write(buff, current_running->fd[fd_index].pip_num, count);

        size_t mycount = 0;
        size_t realcount = count; // write as many as possible
        uchar *tempbuff = kalloc();
        ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
        ientry_t old_clus = now_clus;
        isec_t now_sec = first_sec_of_clus(now_clus);

        while (mycount < realcount){
            size_t writesize = min(BUFSIZE, realcount - mycount);
            sd_read(tempbuff,now_sec);
            memcpy(tempbuff + current_running->fd[fd_index].pos, buff, writesize);
            sd_write(tempbuff,now_sec);
            buff += writesize;
            mycount += writesize;
            if (mycount % CLUSTER_SIZE == 0){
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
        current_running->fd[fd_index].pos += realcount;
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
    log(DEBUG, "%d\n",iovcnt);
    log(DEBUG, "%d\n",fd);
    uint8 fd_index = get_fd_index(fd, current_running);
    if (fd_index < 0) return -1;

    size_t count = 0;

    if (current_running->fd[fd_index].dev == STDOUT){  
        for (uint32_t i = 0; i < iovcnt; i++){      
            memcpy(stdout_buf, iov->iov_base, iov->iov_len);
            stdout_buf[iov->iov_len] = 0;
            printk_port(stdout_buf);
            iov++; count += iov->iov_len;
        }
        return count;
    }
    else if (current_running->fd[fd_index].dev == STDERR){
        for (uint32_t i = 0; i < iovcnt; i++){      
            memcpy(stderr_buf, iov->iov_base, iov->iov_len);
            stderr_buf[iov->iov_len] = 0;
            printk_port(stderr_buf);
            iov++; count += iov->iov_len;
        }
        return count;
    }
    else{
        assert(0);
        // if (current_running->fd[fd_index].piped == FD_PIPED)
        //     return pipe_write(buff, current_running->fd[fd_index].pip_num, count);

        // size_t mycount = 0;
        // size_t realcount = count; // write as many as possible
        // uchar *tempbuff = kalloc();
        // ientry_t now_clus = current_running->fd[fd_index].first_clus_num;
        // ientry_t old_clus = now_clus;
        // isec_t now_sec = first_sec_of_clus(now_clus);

        // while (mycount < realcount){
        //     size_t writesize = min(BUFSIZE, realcount - mycount);
        //     sd_read(tempbuff,now_sec);
        //     memcpy(tempbuff + current_running->fd[fd_index].pos, buff, writesize);
        //     sd_write(tempbuff,now_sec);
        //     buff += writesize;
        //     mycount += writesize;
        //     if (mycount % CLUSTER_SIZE == 0){
        //         old_clus = now_clus;
        //         now_clus = get_next_cluster(now_clus);
        //         if (now_clus == 0x0fffffffu){
        //             // new clus should be assigned
        //             now_clus = search_empty_clus(tempbuff);
        //             write_fat_table(old_clus, now_clus, tempbuff);
        //         }
        //         now_sec = first_sec_of_clus(now_clus);
        //     }
        //     else
        //         now_sec += READ_BUF_CNT;  // writesize / BUFSIZE == READ_BUF_CNT until last write
        // }
        // current_running->fd[fd_index].pos += realcount;
        // current_running->fd[fd_index].length = max(current_running->fd[fd_index].pos, current_running->fd[fd_index].length);

        // kfree(tempbuff);

        // return realcount;
    }
}