#include <os/fat32.h>
#include <os/mm.h>

void get_file_map_info(fat32_file_map_info_t *file_map_info, fd_num_t fd, size_t len, size_t off)
{
    uint8_t fd_index = get_fd_index(fd, current_running);
    file_map_info->sec = get_buf_aligned_sec_from_clus_and_length(current_running->fd[fd_index].first_clus_num, off);
    file_map_info->offset = off % BUFSIZE;
    file_map_info->len = len; /* FOR NOW, maybe larger than 1 page? */
}

/* file must not be closed */
void file_map_read_in(fat32_file_map_info_t *file_map_info)
{
    uchar *temp = kalloc();
    sd_read(temp, file_map_info->sec);
    memcpy(file_map_info->memory_kva, temp + file_map_info->offset, file_map_info->len);
    kfree(temp);
    return ;
}

/* file might be closed */
uint8_t file_map_write_back(fat32_file_map_info_t *file_map_info)
{
    uchar *temp = kalloc();
    sd_read(temp, file_map_info->sec);
    memcpy(temp + file_map_info->offset, file_map_info->memory_kva, file_map_info->len); /* offset is PAGE_ALIGNED */
    sd_write(temp, file_map_info->sec);
    kfree(temp);
    return 0;
}