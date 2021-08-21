#ifndef IOREMAP_H
#define IOREMAP_H

// using this as IO address space (at most using 1 GB, so that it can be store in one pgdir entry)
extern void * ioremap(unsigned long phys_addr, unsigned long size);
extern void iounmap(void *io_addr);

#endif // IOREMAP_H