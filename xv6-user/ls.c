#include "kernel/include/types.h"
#include "kernel/include/stat.h"
#include "xv6-user/user.h"

char *getname(char *path)
{
	char *p = path + strlen(path) - 1;
	while (p > path && *p == '/')
		p--;

	p[1] = '\0';
	while (p >= path && *p != '/')
		p--;

	return p + 1;
}

void
ls(char *path)
{
	int fd;
	struct kstat st;
	char *types[] = {
		[T_DIR]     "DIR ",
		[T_FILE]    "FILE",
		[T_DEVICE]  "DEV ",
	};
	char sizes[] = "BKMGT";

	if((fd = open(path, 0)) < 0){
		fprintf(2, "ls: cannot open %s\n", path);
		return;
	}

	if(fstat(fd, &st) < 0){
		fprintf(2, "ls: cannot stat %s\n", path);
		close(fd);
		return;
	}

	char buf[260 + 255 + 2];
	if (st.mode & ST_MODE_DIR){
		int len = strlen(path);
		if (len >= 260) {
			fprintf(2, "ls: path \"%s\" overflow\n", path);
			close(fd);
			return;
		}
		strcpy(buf, path);
		buf[len] = '/';
		char *p = buf + len + 1;
		
		char dents[512];
		int ret = 0;
		while ((ret = getdents(fd, (struct dirent *)dents, sizeof(dents))) > 0) {
			for (int i = 0; i < ret;) {
				struct dirent *d = (struct dirent *)(dents + i);
				strcpy(p, d->name);
				if (stat(buf, &st) < 0) {
					fprintf(2, "ls: cannot stat %s\n", d->name);
				} else {
					int szno = 0;
					while (st.size >= 1024) {
						st.size /= 1024;
						szno++;
					}
					printf("%x\t%d\t%s\t%l%c\t%s\n", st.dev, st.ino, types[d->type], st.size, sizes[szno], d->name);
				}
				i += d->reclen;
			}
		}
	} else {
		int szno = 0;
		while (st.size >= 1024) {
			st.size /= 1024;
			szno++;
		}
		int t = st.rdev ? T_DEVICE : T_FILE;
		printf("%x\t%d\t%s\t%l%c\t%s\n",
				st.dev, st.ino, types[t], st.size, sizes[szno], getname(path));
	}
	close(fd);
}

int
main(int argc, char *argv[])
{
	int i;

	if(argc < 2){
		ls(".");
		exit(0);
	}
	for(i=1; i<argc; i++)
		ls(argv[i]);
	exit(0);
}
