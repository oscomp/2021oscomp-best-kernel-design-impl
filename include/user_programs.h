
extern unsigned char _elf___test_shell_elf[];
int _length___test_shell_elf;
extern unsigned char _elf___test_busybox_elf[];
int _length___test_busybox_elf;
extern unsigned char _elf___test_testcode_elf[];
int _length___test_testcode_elf;
extern unsigned char _elf___test_cmd_elf[];
int _length___test_cmd_elf;
typedef struct ElfFile {
  char *file_name;
  unsigned char* file_content;
  int* file_length;
} ElfFile;

#define ELF_FILE_NUM 4
extern ElfFile elf_files[4];
extern int get_elf_file(const char *file_name, unsigned char **binary, int *length);
