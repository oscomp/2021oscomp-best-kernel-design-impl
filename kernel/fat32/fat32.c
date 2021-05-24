#include <type.h>
#include <stdio.h>
#include <string.h>
#include <os/mm.h>
#include <os/fat32.h>
#include <os/elf.h>
#include <user_programs.h>
#include <os/sched.h>

fat_t fat;
uchar buf[NORMAL_PAGE_SIZE];

int fat32_init()
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

    printk("[FAT32 init]byts_per_sec: %d\n", fat.bpb.byts_per_sec);
    printk("[FAT32 init]root_clus: %d\n", fat.bpb.root_clus);
    printk("[FAT32 init]sec_per_clus: %d\n", fat.bpb.sec_per_clus);
    printk("[FAT32 init]fat_cnt: %d\n", fat.bpb.fat_cnt);
    printk("[FAT32 init]fat_sz: %d\n", fat.bpb.fat_sz);
    printk("[FAT32 init]fat_rsvd: %d\n", fat.bpb.rsvd_sec_cnt);
    printk("[FAT32 init]fat_hid: %d\n", fat.bpb.hidd_sec);

    /* read root dir and store it for now*/
    uint32_t root_sec = first_sec_of_clus(fat.bpb.root_clus);
    sd_read_sector(buf, root_sec, 1);
    // printk("root_sec: %d\n\r", root_sec);
    // fat32_read(NULL);
    // printk("success");
    kfree(b);

    return 0;
}

/* read file from sd card and put it into readyqueue*/
/*  1: continue to read
   -1: busy
    0: end
    */ 
int8 fat32_read(const char *filename)
{
    // busy
    for (int i = 1; i < NUM_MAX_TASK; ++i)
    {
        if (pcb[i].status != TASK_EXITED)
            return -1;
    }

    uchar *_elf_binary;
    uint32_t length;

#ifdef K210

    static dentry_t *p = (dentry_t *)buf;
    // 0x00: time to end
    if (0x00 == p->filename[0]){
        p++;
        return 0;
    }
    // no length or deleted
    if (p->length == 0 || 0xE5 == p->filename[0]) {p++; return 1;}
    if (p->attribute != FILE_ATTRIBUTE_GDIR){ // not rw-able
        // printk("not what I want\n");
        p++; return 1;
    }

    // printk("filename: %s\n\r", p->filename);
    // printk("extname: %s\n", p->extname);
    // printk("attribute: %x\n", p->attribute);
    // printk("HI: %x\n", p->HI_clusnum);
    // printk("LO: %x\n", p->LO_clusnum);
    // printk("Clusnum: %d %x\n", get_cluster_from_dentry(p), get_cluster_from_dentry(p));
    // printk("length: %d\n\r", p->length);

    // readfile
    uint32_t cluster = get_cluster_from_dentry(p);
    uint32_t sec = first_sec_of_clus(cluster); // make sure you know the start addr
    uchar *file = allocproc(); // make sure space is enough
    uchar *temp = file; // for future use
    for (uint32_t i = 0; i < p->length; i += CLUSTER_SIZE)
    {
        // 1. read 1 cluster
        for (int j = 0; j < READ_CLUS_CNT; ++j)
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

    for (int i = 1; i < NUM_MAX_TASK; ++i)
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

uint32_t get_next_cluster(uint32_t cluster)
{
    uchar buf2[fat.bpb.byts_per_sec];
    // printk("cluster is %d, fatsec is %d\n",cluster, fat_sec_of_clus(cluster));
    sd_read_sector(buf2, fat_sec_of_clus(cluster), 1);
    // printk("offset: %x\n", fat_offset_of_clus(cluster));
    return (*(uint32_t*)((char*)buf2 + fat_offset_of_clus(cluster)));
}