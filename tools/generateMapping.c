#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_BUFF 1000
#define RECORDS_INIT_SIZE 10

typedef struct Records
{
    char*** record;
    int size;
    int capicity;
} Records;

void extend(Records* self)
{
    if (self->capicity == 0) {
        self->capicity = RECORDS_INIT_SIZE;
    } else {
        self->capicity *= 2;
    }
    self->record = (char***)realloc(
        self->record, sizeof(char**) * self->capicity);
    self->record[0] = (char**)realloc(
        self->record[0], sizeof(char*) * 3 * self->capicity);
    for (int i = 0; i < self->capicity; ++i) {
        self->record[i] = self->record[0] + i * 3;
    }
    assert(self->record != NULL);
}

void simplify_filter(char* dest, char* src, const char* pattern)
{
    //int length         = strlen(src);
    int length_pattern = strlen(pattern);
    char* next         = strstr(src, pattern);
    while (*src != '\0') {
        while (src != next && *src != '\0') {
            *dest = *src;
            ++dest;
            ++src;
        }
        if (*src == '\0') break;

        src += length_pattern;
        next = strstr(src, pattern);
    }
    *dest = '\0';
}

int check_duplicate(Records* self, const char* str)
{
    for (int i = 0; i < self->size; ++i) {
        if (strcmp(self->record[i][0], str) == 0) {
            return 0;
        }
    }
    return 1;
}

char* simplify(Records* self, char* varname)
{
    int l     = strlen(varname);
    char* ret = (char*)malloc(sizeof(char) * (l + 1));
    simplify_filter(ret, varname, "_");
    simplify_filter(ret, ret, "test");
    simplify_filter(ret, ret, "elf");

    return ret;
}

void write_to_file(Records* self, FILE* header, FILE* source)
{
    //header = stdout;
    //source = stdout;
    fseek(header, 0, SEEK_END);
    fprintf(header, "typedef struct ElfFile {\n");
    fprintf(header, "  char *file_name;\n");
    fprintf(header, "  unsigned char* file_content;\n");
    fprintf(header, "  int* file_length;\n");
    fprintf(header, "} ElfFile;\n\n");
    fprintf(header, "#define ELF_FILE_NUM %d\n", self->size);
    fprintf(header, "extern ElfFile elf_files[%d];\n", self->size);
    fprintf(header, "extern int get_elf_file(const char *file_name, unsigned char **binary, int *length);\n");

    fseek(source, 0, SEEK_END);
    fprintf(source, "#include <os/string.h>\n");
    fprintf(source, "#include <user_programs.h>\n");
    fprintf(source, "ElfFile elf_files[%d] = {\n", self->size);
    for (int i = 0; i < self->size; ++i) {
        fprintf(
            source,
            "  {.file_name = \"%s\", .file_content = %s, .file_length = &%s}%s\n",
            self->record[i][0], self->record[i][1], self->record[i][2],
            i == self->size - 1 ? "" : ",");
    }
    fprintf(source, "};\n");
    fprintf(source, "int get_elf_file(const char *file_name, unsigned char **binary, int *length)\n");
    fprintf(source, "{\n");
    fprintf(source, "  for (int i = 0; i < %d; ++i) {\n", self->size);
    fprintf(source, "    if (strcmp(elf_files[i].file_name,file_name) == 0) {\n");
    fprintf(source, "      *binary = elf_files[i].file_content;\n");
    fprintf(source, "      *length = *elf_files[i].file_length;\n");
    fprintf(source, "      return 1;\n");
    fprintf(source, "    }\n");
    fprintf(source, "  }\n");
    fprintf(source, "  return 0;\n");
    fprintf(source, "}\n\n");
}

void append_record(Records* self, char* varname)
{
    while (self->size >= self->capicity) {
        extend(self);
    }

    char* length_var_name = (char*) malloc(sizeof(char) * (strlen(varname) + sizeof("_length")));
    strcpy(length_var_name, "_length");
    strcat(length_var_name, &varname[sizeof("_elf") - 1]);
    self->record[self->size][0] = simplify(self, varname);
    self->record[self->size][1] = varname;
    self->record[self->size][2] = length_var_name;
    ++self->size;
}

int start_with(const char* str, const char* start)
{
    while (*start != '\0') {
        if (*start != *str) {
            return 0;
        }
        ++start;
        ++str;
    }
    return 1;
}

char* variable_name(const char* buffer)
{
    // skip `extern unsigned char `
    char* ret       = NULL;
    const char* var = buffer + sizeof(char) * strlen("extern unsigned char ");
    int l           = strlen(var);
    ret             = (char*)malloc(
        sizeof(char) *
        (l + 1));  // strlen(var) + 1(this is for '\0')
    strcpy(ret, var);
    return ret;
}

FILE* open_file_with_suffix(
    const char* file_name, const char* suffix)
{
    char* file_name_buf = (char*)malloc(
        sizeof(char) * (strlen(file_name) + strlen(suffix) + 1));
    strcpy(file_name_buf, file_name);
    strcat(file_name_buf, suffix);
    FILE* ret = fopen(file_name_buf, "r+");
    free(file_name_buf);
    return ret;
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Usage: generateMapping <user_programs>\n");
        return -1;
    }

    FILE* header = open_file_with_suffix(argv[1], ".h");
    if (header == NULL) {
        printf("Can not open %s.h!\n", argv[1]);
        return -1;
    }

    Records records = {NULL, 0, 0};
    char buffer[MAX_LINE_BUFF];
    while (fgets(buffer, MAX_LINE_BUFF, header) != 0) {
        if (start_with(buffer, "extern unsigned char")) {
            buffer[strlen(buffer) - 4] = '\0';
            append_record(&records, variable_name(buffer));
        }
    }

    FILE* source = open_file_with_suffix(argv[1], ".c");
    if (source == NULL) {
        printf("Can not open %s.h!\n", argv[1]);
        return -1;
    }
    write_to_file(&records, header, source);

    fclose(header);
    fclose(source);

    return 0;
}
