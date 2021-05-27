#include "types.h"
#include "memlayout.h"
#include "vm.h"
#include "pm.h"
#include "usrmm.h"
#include "file.h"
#include "proc.h"
#include "string.h"

uint64
_mmap(uint64 start, int len, int prot, int flags, int fd, int off){
  if(off % PGSIZE){
    // TODO: exception
    return -1;
  }

  struct proc *p = myproc();

  int sz = PGROUNDUP(len);
  if(!newseg(p->pagetable, p->segment, MMAP, MAXUVA - sz, sz, prot)){
    // TODO: exception
    return -1;
  }


  struct file * f = fd2file(fd, 0);
  uint64 * ph_addr = NULL;
  if(f)
    ph_addr = f->mmap_ph_addr;
  
  if(!ph_addr){
    // TODO: 分配物理页面
  }

  // 添加映射
  mappages(p->pagetable, MAXUVA - sz, sz, ph_addr, prot);

  // TODO: 将文件中的内容拷贝到映射处

  return MAXUVA - sz;
}