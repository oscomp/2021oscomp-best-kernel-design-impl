#include <stdio.h>
#include <type.h>
#include <qemu.h>

typedef int __attribute__((__mode__(SI))) int32_t;
int main()
{

    char *argv[4], *envp[4];
    char str1[20] = "./busybox";
    // char str2[20] = "sh";
    // char str3[20] = "busybox_testcode.sh";
    char str2[20] = "cal";
    char str3[20] = "1";
    argv[0] = str1; 
    argv[1] = str2;
    argv[2] = NULL;
    argv[3] = NULL;
    envp[0] = NULL;
    // argv[1] = str2; 
    // argv[2] = str3;
    // argv[3] = NULL;

    #ifdef K210
    /* for K210 do test */

    sys_exec(argv[0], argv, envp);

    #else

    /* for qemu singal test*/

    sys_exec(argv[0], argv, NULL);

    #endif

    while (1)
        for (int i = 0; i < 10000000; ++i)
        {
            ;
        }

}