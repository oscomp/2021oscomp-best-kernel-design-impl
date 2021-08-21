#ifndef INCLUDE_FAT32_MMAP_H
#define INCLUDE_FAT32_MMAP_H

#include <type.h>
#include <os/filemap.h>

void get_file_map_info(fat32_file_map_info_t *file_map_info, fd_num_t fd, size_t len, size_t off);
void file_map_read_in(fat32_file_map_info_t *file_map_info);
uint8_t file_map_write_back(fat32_file_map_info_t *file_map_info);

#endif