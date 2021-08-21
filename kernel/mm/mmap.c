#include <os/fat32.h>
#include <os/stdio.h>
#include <os/sched.h>
#include <os/mm.h>
#include <os/errno.h>

static uint64_t prot_to_flags(int prot)
{
    uint64_t flags = 0lu;
    if (prot & PROT_READ)
        flags |= O_RDONLY;
    if (prot & PROT_WRITE)
        flags |= O_WRONLY;

    return flags;
}
/* set attribute */
/* is dirty is set to 0, off is the start of THIS map */
static void set_page_kva_file_map(uint64_t page_kva, fd_num_t fd, size_t len, size_t off, int prot)
{
    fat32_file_map_info_t file_map_info;
    // 1. get file map info from fat32
    get_file_map_info(&file_map_info, fd, len, off);
    // 2. set file map info
    file_map_info.memory_kva = page_kva;
    file_map_info.flags = prot_to_flags(prot);
    // 3. copy info pageRecyc and set dirty to 0
    set_page_kva_file_map_info(page_kva, &file_map_info);
    // 3. read in
    // fat32_read(fd, page_kva, len);
}

/* 成功返回已映射区域的指针，失败返回-1*/
int64 do_mmap(void *start, size_t len, int prot, int flags, int fd, off_t off)
{
    debug();
    log(0, "pid is %d, tid is %d", current_running->pid, current_running->tid);
    log(0, "mmap start %lx, off %ld, len %ld, fd %ld", start, off, len, fd);
    log(0, "flag is %lx, shared? %lx, private?%lx", flags, flags & MAP_SHARED, flags & MAP_PRIVATE);

    if (len <= 0 || (uintptr_t)start % NORMAL_PAGE_SIZE != 0){
        printk_port("error addr in mmap\n");
        return SYSCALL_FAILED;
    }
    /* just for page allocate */
    if (fd == MMAP_ALLOC_PAGE_FD){
        if (flags & MAP_ANONYMOUS == 0)
            return SYSCALL_FAILED;
        if (!start){
            start = find_free_space_and_set(len);
            // if (flags & MAP_SHARED)
            //     log2(0, "free space is at %lx", start);
        }
        uint64_t mask = prot_to_mask(prot);
        for (uint64_t i = 0; i < len; i += NORMAL_PAGE_SIZE)
        {
            uintptr_t page_kva = alloc_page_helper(start + i, current_running->pgdir, mask);
            if (flags & MAP_SHARED)
                share_page_helper(start + i, page_kva, mask);
        }
    }
    else{
        int32_t fd_index;
        if ((fd_index = get_fd_index(fd, current_running)) == -1){
            printk_port("file doesn't open\n");
            return SYSCALL_FAILED;
        }
        if (current_running->fd[fd_index].length < off + len){
            printk_port("len or off too big. len %lx, off %lx", len, off);
            return SYSCALL_FAILED;
        }
        if (!start){
            start = find_free_space_and_set(len);
            // log2(0, "free space is at %lx, and it is a file mmap, is shared? %d", start, flags & MAP_SHARED);
        }

        /* page is not accessible, handle pagefault will do loading for it */
        for (uint64_t i = 0; i < len; i += NORMAL_PAGE_SIZE)
        {
            uintptr_t page_kva = alloc_page_helper(start + i, current_running->pgdir, 0); /* not accessible */
            set_page_kva_file_map(page_kva, fd, min(NORMAL_PAGE_SIZE, len - i), off + i, prot); /* including read in */
            if (flags & MAP_SHARED)
                share_page_helper(start + i, page_kva, 0); /* not accessible */
        }
    }

    log(0, "return is %lx\n", start);
    return start;
}

/* 成功返回0，失败返回-1*/
int64 do_munmap(void *start, size_t len)
{
    debug();
    if ((((uintptr_t)start % NORMAL_PAGE_SIZE) != 0) || ((len % NORMAL_PAGE_SIZE) != 0))
        return SYSCALL_FAILED;
    log(0, "munmap start %lx, len %lx", start, len);

    /* 1. if some part is invalid, don't free page */
    for (uintptr_t ptr = 0; ptr < len; ptr += NORMAL_PAGE_SIZE){
        if(probe_kva_of(start + ptr, current_running->pgdir) == 0)
            return SYSCALL_FAILED;
    }

    /* 2. if all are valid, free them */
    for (uintptr_t ptr = 0; ptr < len; ptr += NORMAL_PAGE_SIZE){
        free_page_helper(start + ptr, current_running->pgdir);
    }
    return SYSCALL_SUCCESSED;
}

/* put mmap back to file */
int do_msync(void *addr, size_t length, int flags)
{
    debug();
    if (((uintptr_t)addr % NORMAL_PAGE_SIZE) != 0){
        printk_port("msync addr not page aligned, %lx\n", addr);
        return -EINVAL;
    }
    uintptr_t page_kva;
    size_t i;
    /* check if all addr are valid and mapped */
    for (i = 0; i < length; i += NORMAL_PAGE_SIZE){
        if ((page_kva = probe_kva_of(addr + i, current_running->pgdir)) == NULL){
            printk_port("msync addr is not valid, %lx\n", addr+i);
            return -EPERM;
        }
        if (!is_page_kva_file_mapped(page_kva)){
            printk_port("msync addr not mapped, %lx, len %lx\n", addr+i, length);
            return -ENOMEM;
        }
    }

    /* all address are valid and file mapped */
    for (i = 0; i < length; i += NORMAL_PAGE_SIZE)
        file_map_write_back_page_kva( probe_kva_of(addr + i, current_running->pgdir) );

    return 0;
}