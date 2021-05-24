
/*********************************************************************************************************
  网络参数文件格式范例 /etc/ifparam.ini
*********************************************************************************************************/

[dm9000]
enable=1
ipaddr=192.168.7.30
netmask=255.255.255.0
gateway=192.168.7.1
default=1
mac=00:11:22:33:44:24

/*********************************************************************************************************
  S3C2440.gdbinit
*********************************************************************************************************/

#connect to J-Link GDB Server
target remote localhost:2331

#reset target
monitor reset

#select endian
monitor endian little

#initialize memory controler
monitor long 0x53000000 = 0x00000000   
monitor long 0x4a000008 = 0xFFFFFFFF   
monitor long 0x4a00001c = 0x00007FFF   
monitor long 0x4c000000 = 0x00FFFFFF   
monitor long 0x4c000014 = 0x00000005   
monitor long 0x4c000004 = 0x0007F021   
monitor long 0x48000000 = 0x12111110   
monitor long 0x48000004 = 0x00000700   
monitor long 0x48000008 = 0x00000700   
monitor long 0x4800000c = 0x00000700   
monitor long 0x48000010 = 0x00000700   
monitor long 0x48000014 = 0x00000700   
monitor long 0x48000018 = 0x00000700   
monitor long 0x4800001c = 0x00018005   
monitor long 0x48000020 = 0x00018005   
monitor long 0x48000024 = 0x009804EC   
monitor long 0x48000028 = 0x000000B2   
monitor long 0x4800002c = 0x00000030   
monitor long 0x48000030 = 0x00000030

#wait for moment
monitor sleep 20

#load the debug image
load

#set cpu to svc mode(on cpu reset)
monitor reg cpsr = 0xd3

#debug in ram 
monitor reg pc   = 0x30000000

#debug begin
