# 初步尝试写HoitFs的笔记

## 1.

mount.c中的mount函数会根据相应字符串去创建一个挂载点pmnDev，这个挂载点中有对应的NN_blkd代表mount的设备，然而我们的Norflash不需要操作BLK RAW设备，相当于NN_blkd只负责存储一个设备名，例如/dev/sda1，所以

``bNeedDelete = LW_TRUE``

bNeedDelete为true的设备在调用完HoitFsDevCreate函数后就会将NN_blkd结构体释放。

函数最后会把挂载点加入到内存全局怪哉点链表上。

## 2.

文件系统驱动必须按照s_class.h中的各种class来，其中LW_DEV_ENTRY里面就是各种要实现的函数。

各种函数的参数列表和ramFs保持一致就行，其中ram_volume这种只要开头是dev_hdr的结构体即可。

s_internal.h中有各种IO接口的函数列表。