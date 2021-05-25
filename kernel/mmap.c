#include "types.h"
#include "memlayout.h"
#include "vm.h"
#include "usrmm.h"
#include "file.h"
#include "proc.h"

uint64
_mmap(uint64 start, int len, int prot, int flags, int fd, int off){
  struct proc *p = myproc();

  newseg(p->pagetable, p->segment, MMAP, MAXUVA, PGSIZE, prot);

  if(flags && fd2file(fd, 0)->mmap_ph_addr){
    // 已经有其他进程映射过并且允许共享，直接将物理地址与虚拟地址关联
    // TODO: 关联物理地址与虚拟地址
  }
  else{
    // TODO: 分配物理页面
  }

  // TODO: 将文件中的内容拷贝到映射处
}