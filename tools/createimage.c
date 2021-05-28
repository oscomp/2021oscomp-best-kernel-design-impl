#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IMAGE_FILE "./k210.bin"
#define ARGS "[--extended] [--vm] <bootblock> <executable-file> ..."


/* structure to store command line options */
static struct {
    int vm;
    int extended;
} options;

/* prototypes of local functions */
static void create_image(int nfiles, char *files[], int extended);
static void error(char *fmt, ...);
static void read_ehdr(Elf64_Ehdr * ehdr, FILE * fp);
static void read_phdr(Elf64_Phdr * phdr, FILE * fp, int ph,
                      Elf64_Ehdr ehdr);
static void write_segment(Elf64_Ehdr ehdr, Elf64_Phdr phdr, FILE * fp,
                          FILE * img, int *nbytes, int *first, int extended);
static void write_os_size(int nbytes, FILE * img, int extended);

int main(int argc, char **argv)
{
    char *progname = argv[0];

    /* process command line options */
    options.vm = 0;
    options.extended = 0;
    while ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == '-')) {
        char *option = &argv[1][2];

        if (strcmp(option, "vm") == 0) {
            options.vm = 1;
        } else if (strcmp(option, "extended") == 0) {
            options.extended = 1;
        } else {
            error("%s: invalid option\nusage: %s %s\n", progname,
                  progname, ARGS);
        }
        argc--;
        argv++;
    }
    if (options.vm == 1) {
        error("%s: option --vm not implemented\n", progname);
    }
    if (argc < 3) {
         // at least 3 args
        error("usage: %s %s\n", progname, ARGS);
    }
    create_image(argc - 1, argv + 1, options.extended);
    return 0;
}

static void create_image(int nfiles, char *files[], int extended)
{
    int ph, nbytes = 0, first = 1;
    FILE *fp, *img;
    Elf64_Ehdr ehdr;
    Elf64_Phdr phdr;

    /* open the image file */
    img = fopen(files[1],"wb");
    
    /* open input file */
	fp = fopen(*files,"rb");   
    /* read ELF header */
    read_ehdr(&ehdr, fp);

    printf("0x%04lx: %s\n", ehdr.e_entry, *files);

    /* for each program header */
    for (ph = 0; ph < ehdr.e_phnum; ph++) {

        /* read program header */
        read_phdr(&phdr, fp, ph, ehdr);
        if (extended)
        {
        	printf("\tsegment %x\n", ph);
        	printf("\t\toffset 0x%.4lx\t\tvaddr 0x%.4lx\n"
        			"\t\tfilesz 0x%.4lx\t\tmemsz 0x%.4lx\n", phdr.p_offset,phdr.p_vaddr, phdr.p_filesz,phdr.p_memsz);
        }
        /* write segment to the image */
        write_segment(ehdr, phdr, fp, img, &nbytes, &first, extended);
    }
    fclose(fp);
    files++;

    write_os_size(nbytes, img, extended);
    fclose(img);
}

static void read_ehdr(Elf64_Ehdr * ehdr, FILE * fp)
{
    fread(ehdr,sizeof(Elf64_Ehdr),1,fp);
}

static void read_phdr(Elf64_Phdr * phdr, FILE * fp, int ph,
                      Elf64_Ehdr ehdr)
{
   	fseek(fp,ehdr.e_phoff,0);
   	for (int i = 0; i < ph; ++i)
   		fseek(fp,sizeof(Elf64_Phdr),1);
    fread(phdr,sizeof(Elf64_Phdr),1,fp);
}

static void write_segment(Elf64_Ehdr ehdr, Elf64_Phdr phdr, FILE * fp,
                          FILE * img, int *nbytes, int *first, int extended)
{
    char *buffer = (char*)malloc(512);
    int sgmtsize = phdr.p_memsz;

    fseek(fp,phdr.p_offset,0);

    while (sgmtsize > 0)
    {
    	if (sgmtsize <= 512)
    	{
    		fread(buffer,sgmtsize,1,fp);
    		for (int i = sgmtsize; i < 512; ++i)
    			buffer[i] = 0;
    	}
    	else
    		fread(buffer,512,1,fp);

    	fwrite(buffer,512,1,img);
    	*nbytes += 512;
    	sgmtsize -= 512;
    }
    
    if (extended && phdr.p_memsz > 0)
		printf("\t\twriting 0x%.4lx bytes\n\t\tpadding up to 0x%.4x\n\r",phdr.p_memsz,*nbytes);
    if (1 == *first) *first = 0;
}

static void write_os_size(int nbytes, FILE * img, int extended)
{
	short sector_num = nbytes / 512 - 1;
    if (extended)
    	printf("os_size: %d sectors\n", sector_num);
}

/* print an error message and exit */
static void error(char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    if (errno != 0) {
        perror(NULL);
    }
    exit(EXIT_FAILURE);
}
