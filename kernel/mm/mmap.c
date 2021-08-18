#include <os/fat32.h>
#include <os/stdio.h>
#include <os/sched.h>
#include <os/mm.h>

/* 成功返回已映射区域的指针，失败返回-1*/
int64 do_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    debug();
    log(0, "pid is %d, tid is %d", current_running->pid, current_running->tid);
    log(0, "mmap start %lx, len %lx, fd %ld %lx %lx", start, len, fd);
    log(0, "flag is %lx, shared? %lx, private?%lx", flags, flags & MAP_SHARED, flags & MAP_PRIVATE);
    log(0, "prot is %d", prot);
    if (len <= 0)
        return SYSCALL_FAILED;
    /* just for page allocate */
    if (fd == MMAP_ALLOC_PAGE_FD){
        if (flags & MAP_ANONYMOUS == 0)
            return SYSCALL_FAILED;
        if (!start){
            uintptr_t probe = USER_STACK_ADDR - NORMAL_PAGE_SIZE;
            uintptr_t user_addr_top = get_user_addr_top(current_running);
            // while (probe >= user_addr_top && probe_kva_of(probe, current_running->pgdir) != 0)
            //     probe -= NORMAL_PAGE_SIZE;
            start = PAGE_ALIGN(user_addr_top - len);
            set_user_addr_top(current_running, start);
        }
        for (uint64_t i = 0; i < len; i += NORMAL_PAGE_SIZE)
        {
            alloc_page_helper(start + i, current_running->pgdir, _PAGE_ALL_MOD);
            do_mprotect(start + i, NORMAL_PAGE_SIZE, prot);
        }
    }
    else{
        int32_t fd_index;
        if ((fd_index = get_fd_index(fd, current_running)) == -1)
            return -1;

        if (!start){
            start = PAGE_ALIGN(get_user_addr_top(current_running) - len);
            set_user_addr_top(current_running, start);
        }

        if (fat32_lseek(fd, off, SEEK_SET) == -1)
            return -1;

        for (uint64_t i = 0; i < len; i += NORMAL_PAGE_SIZE)
        {
            alloc_page_helper(start + i, current_running->pgdir, _PAGE_ALL_MOD);
            do_mprotect(start + i, NORMAL_PAGE_SIZE, prot);
            fat32_read(fd, start + i, min(NORMAL_PAGE_SIZE, len - i));
        }
         
        current_running->fd[fd_index].mmap.start = start;
        current_running->fd[fd_index].mmap.len =len;
        current_running->fd[fd_index].mmap.flags = flags;
        current_running->fd[fd_index].mmap.off = off;
    }

    log(0, "return is %lx", start);
    return start;
}

/* 成功返回0，失败返回-1*/
int64 do_munmap(void *start, size_t len)
{
    debug();
    uint64_t start_uva = start;
    if (((start_uva % NORMAL_PAGE_SIZE) != 0) || ((len % NORMAL_PAGE_SIZE) != 0))
        assert(0);
    log(0, "munmap start %lx, len %lx", start, len);
#ifndef K210
    return 0;
#endif
    int i;
    for (i = 0; i < NUM_FD; ++i)
    {
        /* used, shared, not anonymous, start point is correct */
        if (current_running->fd[i].used == FD_USED && (current_running->fd[i].mmap.flags & MAP_SHARED)
            && (current_running->fd[i].mmap.flags & MAP_ANONYMOUS == 0) && current_running->fd[i].mmap.start == start){
            if (current_running->fd[i].mmap.len < len)
                return -1;
            fat32_lseek(current_running->fd[i].fd_num, current_running->fd[i].mmap.off, SEEK_SET);
            fat32_write(current_running->fd[i].fd_num, start, len);
            return 0;
        }
    }
    if (i == NUM_FD){
        for (uintptr_t ptr = 0; ptr < len; ptr += NORMAL_PAGE_SIZE){
            assert(probe_kva_of(start + ptr, current_running->pgdir) != 0);
            free_page_helper(start + ptr, current_running->pgdir);
            assert(probe_kva_of(start + ptr, current_running->pgdir) == 0);
        }
    }
    return SYSCALL_SUCCESSED;
}

/* put mmap back to file */
int do_msync(void *addr, size_t length, int flags)
{
    debug();
    return 0;
}