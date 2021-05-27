#include "types.h"
#include "memlayout.h"
#include "vm.h"
#include "pm.h"
#include "usrmm.h"
#include "file.h"
#include "proc.h"
#include "string.h"

struct mapped*
add_map(struct inode* ind, int off, int flags)
{
	struct mapped *p = ind->maphead;
	while(p){
		if(p->offset == off){
			p->n_ref++;
			return p;
		}
		p = p->next;
	}
  if(flags){
    struct mapped* new_head = (struct mapped*)kmalloc(sizeof(struct mapped));
    if(!new_head){
      panic("kmalloc failed in add_map!\n");
      return NULL;
    }
    new_head->ph_addr = NULL;
    new_head->next = ind->maphead;
    new_head->n_ref = 1;
    ind->maphead = new_head;
    return new_head;
  }
  return NULL;
}

struct mapped*
del_map(struct inode* ind, int off)
{
	struct mapped *p = ind->maphead;
  struct mapped *pre = p;
	while(p){
		if(p->offset == off){
			p->n_ref--;
			if(!p->n_ref){
				if(p == pre)
          ind->maphead = p->next;
        else
          pre->next = p->next;
        kfree(p);
			}
		}
    pre = p;
    p = p->next;
	}
}

uint64
_mmap(uint64 start, int len, int prot, int flags, int fd, int off)
{
  if(off % PGSIZE)
  {
    panic("offset is not multiples of PGSIZE in mmap!\n");
    return -1;
  }

  struct file * f = fd2file(fd, 0);
  if(!f)
  {
    _debug_warn("mmap", "No such file descriptor!\n");
    return -1;
  }

  struct inode *ip = f->ip;
  if(off + len > ip->size)
  {
    _debug_warn("mmap", "length overflows!\n");
    return -1;
  }

  if(((f->readable) ^ (flags & 0x1)) || ((f->writable) ^ ((flags >> 1) & 0x1)))
  {
    _debug_warn("mmap" "unmatched priviledge");
    return -1;
  }


  struct proc *p = myproc();

  int sz = PGROUNDUP(len);

  struct seg* s = getseg(p->segment, STACK);
  uint64 start = VU_MMAP;
  if(!(s->next)){
    if(s->next->addr < VU_MMAP + sz){
      s = s->next;
      while(s->next){
        start = s->addr + s->sz;
        if(s->next->addr - start >= sz)
        break;
        s = s->next;
      }
    }
  }

  if(start + len > MAXUVA)
  {
    _debug_warn("mmap", "Not enough user space in mmap!\n");
    return -1;
  }

  struct seg* new_seg;
  if(new_seg = (struct seg*) kmalloc(sizeof(struct seg)))
  {
    new_seg->type = MMAP;
    new_seg->addr = start;
    new_seg->sz = sz;
    new_seg->flag = (flags >> 1) & 0x7;
    new_seg->next = s->next;
    s->next = new_seg;
  }
  else
  {
    _debug_warn("mmap", "newseg failed in mmap!\n");
    return -1;
  }

  int n = len / PGSIZE;
  if(len % PGSIZE)
    n += 1;
  
  int i;
  for(i = 0; i < n; i++)
  {
    struct mapped *map = add_map(ip, off, flags);
    if(flags)
    {
      if(!map){
        _debug_warn("mmap", "add_map failed in mmap!\n");
        return -1;
      }
    }
    
    uint64 ph_addr;

    if(flags)
    { // share
      if(map->ph_addr)    // not the first one to share
        mappages(p->pagetable, start + i * PGSIZE, PGSIZE, map->ph_addr, prot);
      else                // the first one to share
      {
        ph_addr = allocpage();
        if((!ph_addr) || mappages(p->pagetable, start + i * PGSIZE, PGSIZE, ph_addr, new_seg->flag))
        {
          del_map(ip, off);
          _debug_warn("mmap", "allocpage failed!\n");
          return -1;
        }
        ilock(ip);
          if((ip->fop->read(ip, 1, start + i * PGSIZE, off + i * PGSIZE, PGSIZE)) > 0)
            ;
        iunlock(ip);
      }
    }
    else
    { // not share
      ph_addr = allocpage();
      if((!ph_addr) || mappages(p->pagetable, start + i * PGSIZE, PGSIZE, ph_addr, new_seg->flag))
      {
        del_map(ip, off);
        _debug_warn("mmap", "allocpage failed!\n");
        return -1;
      }
      if(map)
      { // mapped by others
        memmove(ph_addr, map->ph_addr, PGSIZE);
      }
      else
      { // not mapped by others
        ilock(ip);
        if(i == n - 1)
          memset(start + i * PGSIZE, 0, PGSIZE);
        if((ip->fop->read(ip, 1, start + i * PGSIZE, off + i * PGSIZE, PGSIZE)) > 0)
          ;
        iunlock(ip);
      }
    }
  }
  return start;
}