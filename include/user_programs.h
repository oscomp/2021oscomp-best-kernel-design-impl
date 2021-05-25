
extern unsigned char _elf___test_shell_elf[];
int _length___test_shell_elf;
extern unsigned char _elf___test_waitpid_elf[];
int _length___test_waitpid_elf;
extern unsigned char _elf___test_clone_elf[];
int _length___test_clone_elf;
typedef struct ElfFile {
  char *file_name;
  unsigned char* file_content;
  int* file_length;
} ElfFile;

#define ELF_FILE_NUM 3
extern ElfFile elf_files[3];
extern int get_elf_file(const char *file_name, unsigned char **binary, int *length);
