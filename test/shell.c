#include <stdio.h>
#include <type.h>
#include <qemu.h>

int main()
{

    char *argv[7], *envp[4];
    int argc;
    char str1[20];
    char str2[20];
    char str3[20];
    char str4[20];
    char str5[20];
    char str6[20];
    uint64_t tid;

    char str_echo[60];
    strcpy(str_echo, "latency measurements\n");
    sys_write(1, str_echo, 21);

    // argc = 3;
    // strcpy(str1, "./busybox");
    // strcpy(str2, "sh");
    // strcpy(str3, "busybox_testcode.sh");
    // argv[0] = str1; 
    // if (argc > 1)
    //     argv[1] = str2;
    // if (argc > 2)
    //     argv[2] = str3;
    // if (argc > 3)
    //     argv[3] = str4;
    // if (argc > 4)
    //     argv[4] = str5;
    // if (argc > 5)
    //     argv[5] = str6;
    // argv[argc] = NULL;
    // envp[0] = NULL;

    // if ((tid = sys_clone(0,0,0,0,0)) != 0){
    //     sys_wait4(-1,0,0);
    //     // sys_exit(0);
    // }
    // else{
    //     sys_exec(argv[0], argv, envp);
    // }

    // argc = 3;
    // strcpy(str1, "./busybox");
    // strcpy(str2, "sh");
    // strcpy(str3, "lmbench_testcode.sh");
    // argv[0] = str1; 
    // if (argc > 1)
    //     argv[1] = str2;
    // if (argc > 2)
    //     argv[2] = str3;
    // if (argc > 3)
    //     argv[3] = str4;
    // if (argc > 4)
    //     argv[4] = str5;
    // if (argc > 5)
    //     argv[5] = str6;
    // argv[argc] = NULL;
    // envp[0] = NULL;

    argc = 5;
    strcpy(str1, "./lmbench_all");
    strcpy(str2, "lat_syscall");
    strcpy(str3, "-P");
    strcpy(str4, "1");
    strcpy(str5, "null");
    argv[0] = str1; 
    if (argc > 1)
        argv[1] = str2;
    if (argc > 2)
        argv[2] = str3;
    if (argc > 3)
        argv[3] = str4;
    if (argc > 4)
        argv[4] = str5;
    if (argc > 5)
        argv[5] = str6;
    argv[argc] = NULL;
    envp[0] = NULL;

    if ((tid = sys_clone(0,0,0,0,0)) != 0){
        sys_wait4(-1,0,0);
        // sys_exit(0);
    }
    else{
        sys_exec(argv[0], argv, envp);
    }
    
    strcpy(str_echo, "shell success\n");
    sys_write(1, str_echo, 14);
        
    while (1)
        for (int i = 0; i < 10000000; ++i)
        {
            ;
        }

}