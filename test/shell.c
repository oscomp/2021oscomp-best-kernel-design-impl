#include <stdio.h>
#include <type.h>
#include <qemu.h>

int main()
{

    char *argv[7], *envp[4];

    int argc = 3;

    char str1[20] = "./busybox";
    char str2[20] = "sh";
    char str3[20] = "busybox_testcode.sh";
    // char str2[20] = "echo";
    // char str3[20] = "1";
    char str4[20] = ">";
    char str5[20] = "test.txt";
    char str6[20] = "";

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
    // argv[1] = str2; 
    // argv[2] = str3;
    // argv[3] = NULL;

    uint64_t tid;
    if ((tid = sys_clone(0,0,0,0,0)) != 0){
        sys_wait4(-1,0,0);
        // sys_write(1, "shell success", 12);
    }
    else{
        sys_exec(argv[0], argv, envp);
    }
    
    while (1)
        for (int i = 0; i < 10000000; ++i)
        {
            ;
        }

}