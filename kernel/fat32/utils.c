#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return BOTTOM dentry point if success, NULL if fail */
/* ignore if search "." or ".." in root dir */
/* pos赋值为最后一条目录 */
dentry_t *search(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos)
{
    // printk_port("p addr: %lx, buf: %lx\n", *pp, buf);
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    // printk_port("5\n");
    dentry_t *p = (dentry_t *)buf;
    uchar *filename = kalloc();

    *ignore = 0;

    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){
            uint8 item_num = q->sequence & 0x0f; // entry num

            /* get filename */
            uint8 isbreak = 0;
            uint16_t unich; // unicode

            while (item_num--){
                uint8 name_cnt = 0;
                for (uint8 i = 0; i < LONG_DENTRY_NAME1_LEN; ++i){
                    // name1
                    unich = q->name1[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;           
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME2_LEN; ++i){
                    // name1
                    unich = q->name2[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;            
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME3_LEN; ++i){
                    // name1
                    unich = q->name3[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);  
                    name_cnt++;             
                }
                p = get_next_dentry(p, buf, &now_clus, &now_sec);
                q = (long_dentry_t *)p;
            }
        }
        // short dentry
        else{
            /* filename */
            uint8 name_cnt = 0;
            for (uint8 i = 0; i < SHORT_DENTRY_FILENAME_LEN; ++i)
            {
                if (p->filename[i] == ' ') break;
                else filename[name_cnt++] = p->filename[i];
            }
            if (p->extname[0] != ' ') filename[name_cnt++] = '.';
            for (uint8 i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            {
                if (p->extname[i] == ' ') break;
                else filename[name_cnt++] = p->extname[i];
            }
            filename[name_cnt++] = 0;
        }

        // printk_port("filename is %s\n", filename);
        // printk_port("name is %s\n", name);
        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            if (pos){
                pos->sec = now_sec;
            }
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            if (pos){
                pos->sec = now_sec;
            }
            kfree(filename);
            return p;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    if (mode == FILE_DIR && (!strcmp(name, ".") || !strcmp(name, "..")) && dir_first_clus == root_first_clus){
        *ignore = 1;
    }

    kfree(filename);
    return NULL;
}

/* search if name.file exists in dir now_clus */
/* make sure buf size is BUFSIZE */
/* return p if success, NULL if fail */
/* ignore if search "." or ".." in root dir */
/* pos赋值为第一条目录 */
dentry_t *search2(const uchar *name, uint32_t dir_first_clus, uchar *buf, search_mode_t mode, uint8 *ignore, struct dir_pos *pos)
{
    // printk_port("p addr: %lx, buf: %lx\n", *pp, buf);
    uint32_t now_clus = dir_first_clus, now_sec = first_sec_of_clus(dir_first_clus);
    sd_read(buf, now_sec);
    // printk_port("5\n");
    dentry_t *p = (dentry_t *)buf;
    uchar *filename = kalloc();

    *ignore = 0;

    dentry_t *top_pt; /* ret */
    isec_t top_sec;
    size_t top_len;

    // dont care deleted dir
    while (!is_zero_dentry(p)){

        while (0xE5 == p->filename[0]){
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
        }

        top_pt = p;
        top_sec = now_sec;

        long_dentry_t *q = (long_dentry_t *)p;
        // if long dentry
        if (q->attribute == 0x0f && (q->sequence & 0x40) == 0x40){
            uint8 item_num = q->sequence & 0x0f; // entry num
            top_len = item_num + 1;
            /* get filename */
            uint8 isbreak = 0;
            uint16_t unich; // unicode

            while (item_num--){
                uint8 name_cnt = 0;
                for (uint8 i = 0; i < LONG_DENTRY_NAME1_LEN; ++i){
                    // name1
                    unich = q->name1[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;           
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME2_LEN; ++i){
                    // name1
                    unich = q->name2[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);   
                    name_cnt++;            
                }
                for (uint8 i = 0; i < LONG_DENTRY_NAME3_LEN; ++i){
                    // name1
                    unich = q->name3[i];
                    if (unich == 0x0000 || unich == 0xffffu){
                        filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = 0;
                        break;
                    }
                    else filename[item_num*LONG_DENTRY_NAME_LEN + name_cnt] = unicode2char(unich);  
                    name_cnt++;             
                }
                p = get_next_dentry(p, buf, &now_clus, &now_sec);
                q = (long_dentry_t *)p;
            }
        }
        // short dentry
        else{
            top_len = 1;
            /* filename */
            uint8 name_cnt = 0;
            for (uint8 i = 0; i < SHORT_DENTRY_FILENAME_LEN; ++i)
            {
                if (p->filename[i] == ' ') break;
                else filename[name_cnt++] = p->filename[i];
            }
            if (p->extname[0] != ' ') filename[name_cnt++] = '.';
            for (uint8 i = 0; i < SHORT_DENTRY_EXTNAME_LEN; ++i)
            {
                if (p->extname[i] == ' ') break;
                else filename[name_cnt++] = p->extname[i];
            }
            filename[name_cnt++] = 0;
        }

        if ((p->attribute & 0x10) != 0 && mode == SEARCH_DIR && !filenamecmp(filename, name)){
            pos->offset = (void*)top_pt - (void*)buf;
            pos->sec = top_sec;
            pos->len = top_len;
            kfree(filename);
            return p;
        }
        else if ((p->attribute & 0x10) == 0 && mode == SEARCH_FILE && !filenamecmp(filename, name)){
            pos->offset = (void*)top_pt - (void*)buf;
            pos->sec = top_sec;
            pos->len = top_len;
            kfree(filename);
            return p;
        }
        else
            p = get_next_dentry(p, buf, &now_clus, &now_sec);
    }
    
    if (mode == FILE_DIR && (!strcmp(name, ".") || !strcmp(name, "..")) && dir_first_clus == root_first_clus){
        *ignore = 1;
    }

    kfree(filename);
    return NULL;
}
