### 文件包结构
- busybox_lua_testsuites：外部生成的测试文件，包括lua、busybox、lmbench等程序以及运行脚本和文件等等。
- riscv64：user_C_program所生成的C语言程序（ELF可执行文件）。
- riscv-syscalls-testing：官方评测程序
- user_C_program：该下存放有Rust用户程序所需要的用户库。
 - res：kendryte-k210专用工具链
 - user：C语言程序库以及编译工具，其下的src存放每个独立C语言程序用户的应用程序代码，他们会被编译成不同的用户程序。
- src：该下存放有Rust用户程序所需要的用户库，在文件夹bin之下存放有每个独立的应用程序代码，他们会被编译成不同的用户程序。

注意：riscv、busybox_lua_testsuites以及该目录src下生成的rust程序，将在根目录运行make run的时候自动烧录至文件镜像，如果需要额外添加文件，请放入其中。

### 使用

> make elf

该命令将编译并生成src文件夹下的所有rust程序，并更新到target文件夹。

> make update

该命令将编译并生成user_C_program文件夹下的所有C语言程序，并更新到riscv64文件夹。

其他命令见Makefile。