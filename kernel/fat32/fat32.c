#include <type.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

fat_t fat;
uchar buf[NORMAL_PAGE_SIZE];
uchar filebuf[NORMAL_PAGE_SIZE];
ientry_t cwd;
uchar cwd_path[MAX_PATHLEN];
fd_t fd[NUM_FD];

uchar stdout_buf[NORMAL_PAGE_SIZE];
uchar stdin_buf[NORMAL_PAGE_SIZE];

#define ACCEPT_NUM  2
uchar accept_table[2][10] = {{"GETPID"}, {"UNAME"}};

uint8_t fat32_init()
{
    /* read FAT32 info */
    uint8_t *b = kalloc();
    // read superblock
    sd_read_sector(b, 0, 4);
    // store basic messages
    if (memcmp((char const*)(b + 82), "FAT32", 5))
        printk("not FAT32 volume");
    fat.bpb.byts_per_sec = *(uint8 *)(b + 11) + (*(uint8 *)(b + 12))*256;
    fat.bpb.sec_per_clus = *(b + 13);
    fat.bpb.rsvd_sec_cnt = *(uint16 *)(b + 14);
    fat.bpb.fat_cnt = *(b + 16);
    fat.bpb.hidd_sec = *(uint32 *)(b + 28);
    fat.bpb.tot_sec = *(uint32 *)(b + 32);
    fat.bpb.fat_sz = *(uint32 *)(b + 36);
    fat.bpb.root_clus = *(uint32 *)(b + 44);

    fat.first_data_sec = fat.bpb.rsvd_sec_cnt + fat.bpb.hidd_sec + fat.bpb.fat_cnt * fat.bpb.fat_sz;
    fat.data_sec_cnt = fat.bpb.tot_sec - fat.first_data_sec;
    fat.data_clus_cnt = fat.data_sec_cnt / fat.bpb.sec_per_clus;
    fat.byts_per_clus = fat.bpb.sec_per_clus * fat.bpb.byts_per_sec;

    printk("> [FAT32 init] byts_per_sec: %d\n", fat.bpb.byts_per_sec);
    printk("> [FAT32 init] root_clus: %d\n", fat.bpb.root_clus);
    printk("> [FAT32 init] sec_per_clus: %d\n", fat.bpb.sec_per_clus);
    printk("> [FAT32 init] fat_cnt: %d\n", fat.bpb.fat_cnt);
    printk("> [FAT32 init] fat_sz: %d\n", fat.bpb.fat_sz);
    printk("> [FAT32 init] fat_rsvd: %d\n", fat.bpb.rsvd_sec_cnt);
    printk("> [FAT32 init] fat_hid: %d\n", fat.bpb.hidd_sec);

    /* read root dir and store it for now*/
    cwd = fat.bpb.root_clus;
    strcpy(cwd_path, "/");
    uint32_t root_sec = first_sec_of_clus(fat.bpb.root_clus);
    sd_read_sector(buf, root_sec, READ_PAGE_CNT);
    // printk("root_sec: %d\n\r", root_sec);
    // fat32_read(NULL);
    // printk("success");
    kfree(b);

    /* init fs */
    fd[0].used = 1; fd[0].privilege = O_WRONLY; 
    fd[1].used = 1; fd[1].privilege = O_WRONLY; 
    for (int i = 2; i < NUM_FD; ++i)
        fd[i].used = 0; // remember to set up other props when use it

    return 0;
}

/* read file from sd card and put it into readyqueue*/
/*  1: continue to read
   -1: busy
    0: end
    */ 
int8 fat32_read_test(const char *filename)
{
    // for (int i = 0; i < 40000000; ++i)
    // {
    //     ;
    // }
    static uint32 cnt = 0;
    printk_port("read %d\n", cnt++);
    if (cnt > 32){
        printk_port("???\n");
        assert(0);
    }
    // busy
    // for (int i = 1; i < NUM_MAX_TASK; ++i)
    // {
    //     if (pcb[i].status != TASK_EXITED)
    //         return -1;
    // }

    uchar *_elf_binary;
    uint32_t length;

#ifdef K210

    static dentry_t *p = (dentry_t *)buf;
    printk_port("filename: aa;%s;\n", p->filename);
    printk_port("extname: ;%s;\n", p->extname);
    printk_port("attribute: ;%x;\n", p->attribute);
    printk_port("length: ;%d;\n", p->length);
    p++;
    return 1;
    // 0x0f: long dentry
    while (p->attribute == 0x0f || 0xE5 == p->filename[0]){
        printk_port("<cause 3>\n");
        p++;
    }
    /* now we are at a real dentry */

    // 0x00: time to end
    uchar *n = (uchar *)p;
    uint8_t index;
    for (index = 0; index < 32; ++index)
    {
        // prints("<n: %d>\n", *n);
        if (*n++ != 0)
            break;
    }
    if (index == 32){
        printk_port("<return>");
        p++;
        return 0;
    }

    printk_port("filename: aa;%s;\n", p->filename);
    printk_port("extname: ;%s;\n", p->extname);
    printk_port("attribute: ;%x;\n", p->attribute);
    printk_port("length: ;%d;\n", p->length);

    // no length or deleted
    if (p->length == 0) {
        printk_port("<cause 1>\n");
        p++; 
        return 1;
    }
    // not rw-able
    if (p->attribute != FILE_ATTRIBUTE_GDIR){ 
        printk_port("<cause 2>\n");
        p++; return 1;
    }

    printk_port("filename: bb;%s;\n", p->filename);
    // printk_port("strings: %s %s\n", accept_table[0], accept_table[1]);
    // printk_port("len: %d %d\n", strlen(accept_table[0]), strlen(accept_table[1]));
    // printk_port("I'm here, now %d\n", memcmp(p->filename, accept_table[1], strlen(accept_table[1])));

    /* debug return */
    uint8 i;
    uint8 found = 0;
    for (i = 0; i < ACCEPT_NUM; ++i)
    {
        uint8 num = (strlen(accept_table[i]) > 6)? 6 : strlen(accept_table[i]);
        if (!memcmp(p->filename, accept_table[i], num)){
            found = 1;
            break;
        }
    }
    if (0 == found){
        printk_port("Not match\n");
        printk_port("filename :%s\n", p->filename);
        // printk_port("table :%s, %s", accept_table[0],accept_table[1]);
        p++;
        return 1;
    }
    // printk_port("filename: ;%s;\n\r", p->filename);

    printk("HI: %x\n", p->HI_clusnum);
    printk("LO: %x\n", p->LO_clusnum);
    // printk("Clusnum: %d %x\n", get_cluster_from_dentry(p), get_cluster_from_dentry(p));

    /* Now we must read file */

    // readfile
    uint32_t cluster = get_cluster_from_dentry(p);
    uint32_t sec = first_sec_of_clus(cluster); // make sure you know the start addr
    uchar *file = allocproc(); // make sure space is enough
    uchar *temp = file; // for future use
    for (uint32_t i = 0; i < p->length; i += CLUSTER_SIZE)
    {
        // 1. read 1 cluster
        for (uint8 j = 0; j < READ_CLUS_CNT; ++j)
        {
            sd_read_sector(temp, sec + j, 1);
            temp += fat.bpb.byts_per_sec; // make sure continous
        }
        // 2. compute sec number of next cluster
        cluster = get_next_cluster(cluster);
        sec = first_sec_of_clus(cluster);
    }

    _elf_binary = file;
    length = p->length;

#else

    get_elf_file(filename,&_elf_binary,&length);

#endif

    for (uint i = 1; i < NUM_MAX_TASK; ++i)
        if (pcb[i].status == TASK_EXITED)
        {
            // init pcb
            pcb_t *pcb_underinit = &pcb[i];
            ptr_t kernel_stack = allocPage() + NORMAL_PAGE_SIZE;
            ptr_t user_stack = USER_STACK_ADDR;

            pcb_underinit->preempt_count = 0;
            pcb_underinit->list.ptr = pcb_underinit;
            pcb_underinit->pid = process_id++;
            pcb_underinit->type = USER_PROCESS;
            pcb_underinit->wait_list.next = &pcb_underinit->wait_list;pcb_underinit->wait_list.prev = &pcb_underinit->wait_list;
            pcb_underinit->status = TASK_READY;
            pcb_underinit->priority = 1;
            pcb_underinit->temp_priority = pcb_underinit->priority;
            pcb_underinit->mode = AUTO_CLEANUP_ON_EXIT;
            pcb_underinit->spawn_num = 0;
            pcb_underinit->cursor_x = 1; pcb_underinit->cursor_y = 1;
            pcb_underinit->mask = current_running->mask;

            // load file to memory
            uintptr_t pgdir = allocPage();
            clear_pgdir(pgdir);
            uint64_t user_stack_page_kva = alloc_page_helper(user_stack - NORMAL_PAGE_SIZE,pgdir,_PAGE_ACCESSED|_PAGE_DIRTY|_PAGE_READ|_PAGE_WRITE|_PAGE_USER);
            uint64_t test_elf = (uintptr_t)load_elf(_elf_binary,length,pgdir,alloc_page_helper);
            share_pgtable(pgdir,pa2kva(PGDIR_PA));

            // copy argc & argv

            // prepare stack
            init_pcb_stack(pgdir, kernel_stack, user_stack, test_elf, 0, user_stack - NORMAL_PAGE_SIZE, pcb_underinit);
            // add to ready_queue
            list_del(&pcb_underinit->list);
            list_add_tail(&pcb_underinit->list,&ready_queue);

            #ifdef K210
            // free resource
            allocfree();
            p++;
            #endif

            return -1;
        }
    assert(0);
}

/* write count bytes from buff to file in fd */
/* return count: success
          -1: fail
        */
size_t fat32_write(uint32 fd, uchar *buff, uint64_t count)
{
    // for (uint32 i = 0; i < 30000000; ++i)
    // {
    //     ;
    // }
    if (count == 0) return 0;
    if (fd == stdout){
        memcpy(stdout_buf, buff, count);
        stdout_buf[count] = 0;
        return printk_port(stdout_buf);
    }

}

/* go to child directory */

uint32_t get_next_cluster(uint32_t cluster)
{
    uchar buf2[fat.bpb.byts_per_sec];
    // printk("cluster is %d, fatsec is %d\n",cluster, fat_sec_of_clus(cluster));
    sd_read_sector(buf2, fat_sec_of_clus(cluster), 1);
    // printk("offset: %x\n", fat_offset_of_clus(cluster));
    return (*(uint32_t*)((char*)buf2 + fat_offset_of_clus(cluster)));
}