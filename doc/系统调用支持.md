## 系统调用支持

现在我们需要支持的最重要的系统调用有：rt_sigaction，setitimer, pselect(select), pipe2(with flags), getrusage, readlinkat。这些是为了获得测试数据所需要的时间、数据以及进程相关的syscall，但是各个测试对相应的测试对象设计的系统调用有着额外的要求。

#### Clone

Clone是一个较为复杂的系统调用，可以指定用户栈以及跳转函数，具体的ABI调用规则如下。

> **syscall(SYS_clone, flags, stack, ptid, tls, ctid)**

我们需要支持两个参数：flags和stack。stack指定了子进程的用户栈，而flags则暂定为SIGCHLD=17。

用户程序对clone进行了包装，应用了两个函数clone和fork。

##### fork

> syscall(SYS_clone, SIGCHLD, 0)

fork直接传递0为stack，表示创建的时候需要内核动态分配。

##### __clone

> __clone(fn, stack, flags, NULL, NULL, NULL);

这里的clone是用户态的clone，使用汇编语言进行了包装，其中fn是子程序执行的函数，它将会放在用户栈中，供创建的子进程调用，当子进程调用完毕之后，就以函数的范围值调用exit进行退出。相比__clone，fork则直接调用了系统调用，不需要在用户标准库进行包装。

##### 实现

做折中，我们依然分配已有的用户栈，只不过不使用，将其栈指针改为对应的指定内核栈，这样我们只需对syscall的clone进行适配即可。

#### Nanosleep

> int nanosleep(const struct timespec *req, struct timespec *rem)

req为内有秒和微秒的结构体指针，用来指示需要睡眠的时间。rem则需要内核填写，表示唤醒时与需要睡眠额时间相比还差多少，通常为零，系统调用通常返回0。如果出现了中途的中断，那么rem不会为零，系统调用也会返回-1。

#### set_tid_address & gettid

什么是tid，什么是pid？

![在这里插入图片描述](系统调用支持.assets/20200904185133812.png)

这里面，用户和内核的看法不一样。对用户来说，进程创建的不同线程有着一样的pid和不一样的tid，而内核看来pid都不同，因为他们彼此属于不同的程序。但是为了标识，内核有一个tgid来判定是否隶属于同一进程。

对应关系如下：

|      | 进程 | 线程 |
| :--: | :--: | :--: |
| 内核 | pid  | tid  |
| 用户 | tgid | pid  |

那么gettid实际上是获得得内核的pid。

那什么是set_tid_address 呢？这实际上是PCB中的两个属性之一：set_child_tid和clear_child_tid。

- set_child_tid：clone时指定CLONE_CHILD_SETTID，那么就会将收到的set_child_tid（clone中的ctid）赋值给对应的属性，在**执行新创建的线程之前**，先将tid（内核的pid）赋值进set_child_tid指向的地址中。
- clear_child_tid：clone时指定CLONE_CHILD_CLEARTID，那么就会将收到的clear_child_tid（clone中的ctid）赋值给对应的属性，**新创建的线程退出后**，先将0赋值进clear_child_tid指向的地址中。
  - 不仅如此，还要执行系统调用futex(clear_child_tid, FUTEX_WAKE, 1, NULL, NULL, 0);。但是我先不做。
- long set_tid_address(int *tidptr)： 该方法实际上就是设置clear_child_tid。
- 注意这两个属性，其各自默认为0，为0时什么操作也不做。

因此我们需要添加该属性，更改clone，添加set_tid_address方法，修改exit。

注意，赋值进clear_child_tid指向的地址，指的是**子进程**对应的地址。



#### execve

execve调用：**int execve(const char *filename, char *const argv[], char *const envp[]);**

main函数接收：**int main(int argc, char\** argv)**

虽然我们已经实现了该syscall，但是我们在之后发现我们在实现该系统调用时，有着很大的一部分没有考虑周全。execve最重要的，是执行新程序时同时要传递的参数问题。其参数都存放在栈中，其布局如下

```
position            content                         size(bytes) + comment
-------------------------------------------------------------------------------------------------

stack pointer ->    [argc = number of args]         8
                    [argv[0](pointer)]              8
                    [argv[1](pointer)]              8
                    [argv[...](pointer)]            8 * x
                    [argv[n-1](pointer)]            8
                    [argv[n](pointer)]              8 (=NULL)


                    [envp[0](pointer)]              8
                    [envp[1](pointer)]              8
                    [envp[..](pointer)]             8 * x
                    [envp[term](pointer)]           8 (=NULL)


                    [auxv[0](Elf64_auxv_t)]         16
                    [auxv[1](Elf64_auxv_t)]         16
                    [auxv[..](Elf64_auxv_t)]        16 * x
                    [auxv[term](Elf64_auxv_t)]      16 (=NULL)

                    [padding]                       >= 0

                    [rand bytes]                    16

                    [String identifying platform]   >= 0

                    [padding for align]             >= 0 (sp - (get_random_int() % 8192)) & (~0xf)

                    [argument ASCIIZ strings]       >= 0
                    [environment ASCIIZ str]        >= 0

```

实际上，运行的程序要接受四个参数，argc, argv,  环境变量(envp) 和 辅助向量(auxv)。envp就是我们在程序运行中俗称的环境变量，如PATH。auxv是ELF文件一些标识内容，用作辅助。

一个例子：

```
$ ./a.out 

arg_num:1

arg 0:./a.out

environment vector:
LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
XDG_MENU_PREFIX=gnome-
LANG=en_US.UTF-8
GDM_LANG=en_US.UTF-8
DISPLAY=:0
COLORTERM=truecolor
DESKTOP_AUTOSTART_ID=1067f02f661904b013153191243121980500000010550007
USERNAME=cike
XDG_VTNR=2
SSH_AUTH_SOCK=/run/user/1000/keyring/ssh
XDG_SESSION_ID=2
USER=cike
DESKTOP_SESSION=gnome
WAYLAND_DISPLAY=wayland-0
GNOME_TERMINAL_SCREEN=/org/gnome/Terminal/screen/1209a8c3_5cad_4323_b8e8_171909ba8809
PWD=/home/cike/workspace/linker_loader
HOME=/home/cike
XDG_SESSION_TYPE=wayland
XDG_SESSION_DESKTOP=gnome
TERM=xterm-256color
SHELL=/bin/bash
VTE_VERSION=5202
QT_IM_MODULE=fcitx
XMODIFIERS=@im=fcitx
XDG_CURRENT_DESKTOP=GNOME
GNOME_TERMINAL_SERVICE=:1.107
XDG_SEAT=seat0
SHLVL=2
GDMSESSION=gnome
GNOME_DESKTOP_SESSION_ID=this-is-deprecated
LOGNAME=cike
DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus
XDG_RUNTIME_DIR=/run/user/1000
PATH=/usr/local/bin:/usr/bin:/bin:/usr/local/games:/usr/games
SESSION_MANAGER=local/debian:@/tmp/.ICE-unix/1055,unix/debian:/tmp/.ICE-unix/1055
GTK_IM_MODULE=fcitx
OLDPWD=/home/cike/workspace/test
_=./a.out

auxiliary vectors:
auxv type:33, val:0x121d1000
auxv type:16, val:0xbfebfbff
auxv type:6, val:0x1000
auxv type:17, val:0x64
auxv type:3, val:0xdb1e4040
auxv type:4, val:0x38
auxv type:5, val:0x9
auxv type:7, val:0x30912000
auxv type:8, val:0x0
auxv type:9, val:0xdb1e45d0
auxv type:11, val:0x3e8
auxv type:12, val:0x3e8
auxv type:13, val:0x3e8
auxv type:14, val:0x3e8
auxv type:23, val:0x0
auxv type:25, val:0x1200c7c9
auxv type:26, val:0x0
auxv type:31, val:0x1200eff0
auxv type:15, val:0x1200c7d9

address:7ffd1200e5fb, env0_ascii_string:LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.Z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36:
address:7ffd1200e5f3 ~ 7ffd1200e5fa, argument0_ascii_string:./a.out
address:7ffd1200c7e0 ~ 7ffd1200e5f2, here contain 7699 bytes data
address:7ffd1200c7d9 ~ 7ffd1200c7df, String identifying platform:x86_64
address:7ffd1200c7c9 ~ 7ffd1200c7d8, rand_bytes[16]: 193 130 184 246 204 82 221 150 58 249 82 90 18 26 172 195
address:7ffd1200c680 ~ 7ffd1200c7b0, auxivilary vector
address:7ffd1200c548 ~ 7ffd1200c678, environment vector
address:7ffd1200c538 ~ 7ffd1200c540, argv
address:7ffd1200c530, argc

```

我们准备从[argc]实现至[String identifying platform]，具体的实现方法就可以见task.rs的exec函数实现。



#### mmap

事实上我们已经实现了mmap，但是有一些功能和协议我们暂时搁置了，现在我们想要完成对应的补充，但是仅限于busybox所使用的系统调用。

当前每一个mmap都应该要放在memory set中，但是mmap会改变对应一小块地址的使用权限。这使得其所处的map area的权限不统一，与其内部标识不相符，这种情况我们暂时忽略。同时mprotect也是一样。
同时，要注意的是，使用set_pte_flag的任何进程都会造成这种情况。

MAP_FIXED，指的是强制映射指定的区域，如果这段区域已经被映射了，那么就替换掉原来的区域，占为己有。busybox sh只占有了一次。

因此暂时逻辑为，忽略“替换”操作。被占有的为堆空间，我们暂时不考虑对堆的处理。

##### mmap的调用处理逻辑

- 对于指定的内存区域：默认已经映射，根据指定的协议调整访问权限。（注意：我们没有根据指定的标志做fd处理，这是一个实现的空缺）
- 对于指定空内存：内核需要分配一段内存，然后再根据对应的协议和标志进行操作。
  - 创建新map_area
  - 将对应area进行页表映射
  - push进memory set

#### getrusage

int getrusage(int who, struct rusage *usage);

获取进程（包括进程内的所有线程）的资源使用量。

##### rusage

资源使用的结构体如下：

```c
struct rusage {
               struct timeval ru_utime; /* user CPU time used */
               struct timeval ru_stime; /* system CPU time used */
               long   ru_maxrss;        /* maximum resident set size */
               long   ru_ixrss;         /* integral shared memory size */
               long   ru_idrss;         /* integral unshared data size */
               long   ru_isrss;         /* integral unshared stack size */
               long   ru_minflt;        /* page reclaims (soft page faults) */
               long   ru_majflt;        /* page faults (hard page faults) */
               long   ru_nswap;         /* swaps */
               long   ru_inblock;       /* block input operations */
               long   ru_oublock;       /* block output operations */
               long   ru_msgsnd;        /* IPC messages sent */
               long   ru_msgrcv;        /* IPC messages received */
               long   ru_nsignals;      /* signals received */
               long   ru_nvcsw;         /* voluntary context switches */
               long   ru_nivcsw;        /* involuntary context switches */
           };
```

这个资源应该要放进PCB中，跟随着PCB一同流动。我们只实现两者：ru_utime和ru_stime。

为了获得对应的运行时间，我们需要在核管理器中，加入时间量。于是，现在，我们需要管理进程的运行周期，以计算出时间。

实际上，我们只需要在进程切换，也就是核管理器的切换中做这件事情就可以了。我们需要维护两者，我们分别叙述两者时间获得的方法。

user time：

- 当进程trap return时，更新核管理器的user clock
- 当进程trap into时，获得核管理器中程序运行时间，更新PCB时间

kernel time：与调度相关，较为复杂

- 更新核管理器的kernel clock
  - trap into后
  - shed进程切换成功**后**
- 获得核管理器中程序运行时间，更新PCB时间
  - trap return前
  - shed进程切换成功**前**

注意，这些方法都是在核管理器中的，因此不需要锁，只有更新PCB的时候才会需要锁。

##### who

- RUSAGE_SELF：所有线程加上自己的资源统计。
- RUSAGE_CHILDREN：所有终止的子进程加上自己的资源统计。
- RUSAGE_THREAD (since Linux 2.6.26)：仅仅自己。

##### return

0：成功

-1：失败


#### setitimer

set interval timer

int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);

struct itimerval {
               struct timeval it_interval; /* Interval for periodic timer */
               struct timeval it_value;    /* Time until next expiration */
           };

struct timeval {
    time_t      tv_sec;         /* seconds */
    suseconds_t tv_usec;        /* microseconds */
};

设置一个计时器，当然也可以取消一个之前设置的计时器。

##### args

old_value：指针，不为零就需要system将指针所指的位置赋值为当前系统的值。（这和getitimer的功能相同）

new_value：指针。
- 其中的it_value全零代表取消计时器，否则就制定一个计时器。it_value实际上是当前剩余的时间。
- 其中ite_interval全零代表计时器只使用一次，否则，就是以该间隔执行的多次计时。


##### impl

我们在PCB加入一个定时器，定时器内容为ITimeVal结构体，value代表定时器的**终止时间**（而不是剩余时间）。每次切换到这个进程的时候，就会判断一下是否已经到达时间了。

#### rt_sigaction

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);

该函数主要是将信号和函数绑定起来，使得进程在收到对应的信号时，进入对应的函数执行功能（执行的时候使用独立的堆和栈，与原程序无关）。

实际上，最重要的是其和setitimer所牵连的信号机制，setitimer实际上是设定了生成alarm signal的逻辑，而处理信号的逻辑，要在sigaction中进一步阐述。因此，我们正式开发信号机制。具体见Signal.md