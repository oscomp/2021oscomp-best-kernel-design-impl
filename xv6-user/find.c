#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "kernel/include/fcntl.h"
#include "xv6-user/user.h"

static char path[512];

void find(char *filename)
{
    int fd;
    struct kstat st;
    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    if (!(st.mode & ST_MODE_DIR)) {
        close(fd);
        return;
    }
    int len = strlen(path);
    if (len + 255 + 2 > sizeof(path)) {
        fprintf(2, "find: path too long\n");
        close(fd);
        return;
    }
    char *p = path + len - 1;
    if (*p != '/') {
        *++p = '/';
    }
    p++;
    char buf[128];
    int ret;
    while ((ret = getdents(fd, (struct dirent *)buf, sizeof(buf))) > 0) {
        struct dirent *entry;
        for (int i = 0; i < ret; i += entry->reclen) {
            entry = (struct dirent *)(buf + i);
            strcpy(p, entry->name);
            if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0) {
                continue;
            }
            if (strcmp(p, filename) == 0) {
                fprintf(1, "%s\n", path);
            }
            if (entry->type == T_DIR) {
                find(filename);
            }
        }
    }
    close(fd);
    return;
}


int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(2, "Usage: find DIR FILENAME\n");
        exit(0);
    } else {
        strcpy(path, argv[1]);
        find(argv[2]);
    }
    exit(0);
}
