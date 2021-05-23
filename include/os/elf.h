#ifndef _ELF_H
#define _ELF_H

#include <os/string.h>
#include <pgtable.h>
#include <type.h>

/* 64-bit ELF base types. */
typedef uint64_t Elf64_Addr;
typedef uint16_t Elf64_Half;
typedef int16_t Elf64_SHalf;
typedef uint64_t Elf64_Off;
typedef int32_t Elf64_Sword;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef int64_t Elf64_Sxword;
typedef uint16_t Elf64_Section;
typedef Elf64_Half Elf64_Versym;

#define EI_MAG0 0 /* e_ident[] indexes */
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_PAD 8

#define ELFMAG0 0x7f /* EI_MAG */
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'
#define ELFMAG "\177ELF"
#define SELFMAG 4

#define ELFCLASSNONE 0 /* EI_CLASS */
#define ELFCLASS32 1
#define ELFCLASS64 2
#define ELFCLASSNUM 3

#define ELFDATANONE 0 /* e_ident[EI_DATA] */
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_NONE 0 /* e_version, EI_VERSION */
#define EV_CURRENT 1
#define EV_NUM 2

#define ELFOSABI_NONE 0
#define ELFOSABI_LINUX 3

#ifndef ELF_OSABI
#define ELF_OSABI ELFOSABI_NONE
#endif

#define EI_NIDENT 16
typedef struct elf64_hdr
{
    unsigned char e_ident[EI_NIDENT]; /* ELF "magic number" */
    Elf64_Half e_type;
    Elf64_Half e_machine;
    Elf64_Word e_version;
    Elf64_Addr e_entry; /* Entry point virtual address */
    Elf64_Off e_phoff;  /* Program header table file offset */
    Elf64_Off e_shoff;  /* Section header table file offset */
    Elf64_Word e_flags;
    Elf64_Half e_ehsize;
    Elf64_Half e_phentsize;
    Elf64_Half e_phnum;
    Elf64_Half e_shentsize;
    Elf64_Half e_shnum;
    Elf64_Half e_shstrndx;
} Elf64_Ehdr;

/* These constants define the permissions on sections in the
   program header, p_flags. */
#define PF_X (1 << 0)          /* Segment is executable */
#define PF_W (1 << 1)          /* Segment is writable */
#define PF_R (1 << 2)          /* Segment is readable */
#define PF_MASKOS 0x0ff00000   /* OS-specific */
#define PF_MASKPROC 0xf0000000 /* Processor-specific */

/* Legal values for p_type (segment type).  */

#define PT_NULL 0          /* Program header table entry unused */
#define PT_LOAD 1          /* Loadable program segment */
#define PT_DYNAMIC 2       /* Dynamic linking information */
#define PT_INTERP 3        /* Program interpreter */
#define PT_NOTE 4          /* Auxiliary information */
#define PT_SHLIB 5         /* Reserved */
#define PT_PHDR 6          /* Entry for header table itself */
#define PT_TLS 7           /* Thread-local storage segment */
#define PT_NUM 8           /* Number of defined types */
#define PT_LOOS 0x60000000 /* Start of OS-specific */
#define PT_GNU_EH_FRAME                     \
    0x6474e550 /* GCC .eh_frame_hdr segment \
                */
#define PT_GNU_STACK                                             \
    0x6474e551                  /* Indicates stack executability \
                                 */
#define PT_GNU_RELRO 0x6474e552 /* Read-only after relocation */
#define PT_LOSUNW 0x6ffffffa
#define PT_SUNWBSS 0x6ffffffa   /* Sun Specific segment */
#define PT_SUNWSTACK 0x6ffffffb /* Stack segment */
#define PT_HISUNW 0x6fffffff
#define PT_HIOS 0x6fffffff   /* End of OS-specific */
#define PT_LOPROC 0x70000000 /* Start of processor-specific */
#define PT_HIPROC 0x7fffffff /* End of processor-specific */

typedef struct elf64_phdr
{
    Elf64_Word p_type;
    Elf64_Word p_flags;
    Elf64_Off p_offset;   /* Segment file offset */
    Elf64_Addr p_vaddr;   /* Segment virtual address */
    Elf64_Addr p_paddr;   /* Segment physical address */
    Elf64_Xword p_filesz; /* Segment size in file */
    Elf64_Xword p_memsz;  /* Segment size in memory */
    Elf64_Xword p_align;  /* Segment alignment, file & memory */
} Elf64_Phdr;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

static inline int is_elf_format(unsigned char *binary)
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)binary;

    if (ehdr->e_ident[0] == EI_MAG0 &&
        ehdr->e_ident[1] == EI_MAG1 &&
        ehdr->e_ident[2] == EI_MAG2 &&
        ehdr->e_ident[3] == EI_MAG3) {
        return 0;
    }

    return 1;
}

/* prepare_page_for_kva should return a kernel virtual address */
static inline uintptr_t load_elf(
    unsigned char elf_binary[], unsigned length, uintptr_t pgdir,
    uintptr_t (*prepare_page_for_va)(uintptr_t va, uintptr_t pgdir, uint64_t mask))
{
    Elf64_Ehdr *ehdr = (Elf64_Ehdr *)elf_binary;
    Elf64_Phdr *phdr = NULL;
    /* As a loader, we just care about segment,
     * so we just parse program headers.
     */
    unsigned char *ptr_ph_table = NULL;
    Elf64_Half ph_entry_count;
    Elf64_Half ph_entry_size;
    int i = 0;

    // check whether `binary` is a ELF file.
    if (length < 4 || !is_elf_format(elf_binary)) {
        return 0;  // return NULL when error!
    }

    ptr_ph_table   = elf_binary + ehdr->e_phoff;
    ph_entry_count = ehdr->e_phnum;
    ph_entry_size  = ehdr->e_phentsize;

    while (ph_entry_count--) {
        phdr = (Elf64_Phdr *)ptr_ph_table;

        if (phdr->p_type == PT_LOAD) {
            for (i = 0; i < phdr->p_memsz; i += NORMAL_PAGE_SIZE) {
                if (i < phdr->p_filesz) {
                    unsigned char *bytes_of_page =
                        (unsigned char *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    memcpy(
                        bytes_of_page,
                        elf_binary + phdr->p_offset + i,
                        MIN(phdr->p_filesz - i, NORMAL_PAGE_SIZE));
                    if (phdr->p_filesz - i < NORMAL_PAGE_SIZE) {
                        for (int j =
                                 phdr->p_filesz % NORMAL_PAGE_SIZE;
                             j < NORMAL_PAGE_SIZE; ++j) {
                            bytes_of_page[j] = 0;
                        }
                    }
                } else {
                    long *bytes_of_page =
                        (long *)prepare_page_for_va(
                            (uintptr_t)(phdr->p_vaddr + i), pgdir, _PAGE_EXEC|_PAGE_READ|_PAGE_WRITE);
                    for (int j = 0;
                         j < NORMAL_PAGE_SIZE / sizeof(long);
                         ++j) {
                        bytes_of_page[j] = 0;
                    }
                }
            }
        }

        ptr_ph_table += ph_entry_size;
    }

    return ehdr->e_entry;
}

#endif  // _ELF_H