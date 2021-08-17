#include <stdio.h>
#include <type.h>
#include <qemu.h>

int main()
{

    char *argv[20], *envp[4];
    int argc;
    char str1[20];
    char str2[20];
    char str3[20];
    char str4[20];
    char str5[20];
    char str6[20];
    char str7[20];
    char str8[20];
    char str9[20];
    char str10[20];
    char str11[20];
    char str12[20];
    char str_echo[60];
    uint64_t tid;

    // strcpy(str_echo, "latency measurements\n");
    // sys_write(1, str_echo, 21);

    // argc = 4;
    // strcpy(str1, "./busybox");
    // strcpy(str2, "mkdir");
    // strcpy(str3, "-p");
    // strcpy(str4, "/var/tmp");

    argc = 3;
    strcpy(str1, "./busybox");
    strcpy(str2, "sh");
    strcpy(str3, "lmbench_testcode.sh");

    // argc = 7;
    // strcpy(str1, "./lmbench_all");
    // strcpy(str2, "lat_ctx");
    // strcpy(str3, "-P");
    // strcpy(str4, "1");
    // strcpy(str5, "-s");
    // strcpy(str6, "32");
    // strcpy(str7, "2");
    // strcpy(str8, "4");
    // strcpy(str9, "8");
    // strcpy(str10,"16");

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
    if (argc > 6)
        argv[6] = str7;
    if (argc > 7)
        argv[7] = str8;
    if (argc > 8)
        argv[8] = str9;
    if (argc > 9)
        argv[9] = str10;
    if (argc > 10)
        argv[10] = str11;
    if (argc > 11)
        argv[11] = str12;
    argv[argc] = NULL;
    envp[0] = NULL;

    if ((tid = sys_clone(0,0,0,0,0)) != 0){
        sys_set_test_timer();
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