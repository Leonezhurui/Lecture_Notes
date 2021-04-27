# Linux程序设计复习--第一章

> By Leone, Software Engineering



## 简答：Linux系统

* A free Unix-type operating system developed under the GNU General Public License.
  * Open source
  * Popular
  * Support most of the platforms available

* 根据GNU通用公共许可证开发的免费Unix类型的操作系统。
  * 开源
  * 受欢迎的
  * 支持大多数可用平台



## Linux创始人

1991Linus Torvalds

Linux是使用C语言和汇编语言编写的



## Linux发行版

常用的发行版：Redhat、Debian、Ubuntu、CentOS



## 安装Linux

* 从可启动媒体启动系统
* 所有安装程序都需要执行基本相同的步骤：
  * 选择语言，键盘类型，鼠标类型
  * **创建分区****
  * **设置引导加载程序****
  * 配置网络
  * 配置用户和身份验证
  * 选择软件包组
  * 配置X
  * 安装软件包
  * 创建启动盘





## Linux启动流程

power on -> BIOS -> boot loader -> Linux kernel -> init -> system ready



### BIOS(基本输入输出系统)

* 检查内存和硬件（POST）
* 从非易失性存储器中加载选项
  * 存储器时序
  * 引导设备的顺序
* 检查启动设备
  * 软盘，CD-ROM，硬盘等
* 加载启动设备的MBR并执行



### Boot loader引导加载程序

* **引导加载程序加载并启动Linux内核**
  * 可以将引导参数传递给Linux内核，例如设备信息
  * 可以选择加载初始根磁盘
  * 也可以启动其他操作系统
* 通用引导加载程序：
  * LILO：Linux加载程序
  * GRUB：Grand统一引导加载程序
* 除非使用其他引导加载程序，否则通常在/dev/hda中进行配置。



LILO - Linux Loader

* LILO
  * 根据配置文件配置MBR的程序。
  * 使用lilo命令必须以root用户身份运行。

* lilo命令语法：
  * lilo [-v] [-v] [-C配置文件] [-t]
  * 配置文件：/etc/lilo.conf



GRUB – GRand Unified Bootloader

* GRUB
  * 程序存储在MBR（第一阶段）和/boot/grub（1.5和第二阶段）中
  * 了解文件系统的结构； 无需像LILO一样激活配置
  * **配置文件/boot/grub/grub.conf**
  * 通过grub-install安装在MBR中



## 分区理论

MBR(Master Boot Record)

主分区引导记录；主引导分区是磁盘的第一个分区，一共512字节，其中bootloader占446字节(用于存放**引导代码**)，DPT(Disk Partition Table)占64字节，magic number占2个字节

![img](https://pic4.zhimg.com/80/v2-039771e933fc23590ca888821fd6903f_1440w.png)

MBR类型的磁盘最多有4个分区；一个分区可能是一个扩展分区；一个扩展分区可能有多个逻辑分区；



GPT/GUID

GPT分区：全称为Globally Unique Identifier Partition Table，也叫做**GUID分区表**(了解)





## 命令行和图形界面(CLI vs GUI)

GUI

1. 学习起来简单易懂；入门门槛较低

2. 代码的执行效率中等；

3. 界面颜色丰富多彩，非常美观；受广大用户欢迎，界面直观地展示可用的功能和不可用的功能

4. 依赖鼠标和键盘；兼容触摸屏等

5. 界面大小和硬件配置对程序的运行效果有较大的影响

6. 适合编写依赖图形的视频游戏，但花费大量的时间编写界面代码+功能交互逻辑代码

   

CLI

1. 学习起来困难；入门门槛较高，对新手不友好

2. 编写程序高效；功能强大

3. 单调的界面颜色或无界面

4. 只需要键盘就能操作，不适合触屏

5. 运行效率高，稳定性好

6. 适合用于管理服务器

   



## Linux安装软件

from a tarball(Linux最好的大包软件)

就是使用一个压缩文件来安装

* tar zxvf application.tar.gz 
* cd application
* ./configure
* make
* su -
* make install



* apt-get command * 
* dpkg
* aptitude
* yum + rpm
* RPM: RPM Package Management 
* rpm –q –a
* rpm –ivh package-name
* rpm –e package-name



### apt-get工作原理

1. 查询源列表

   apt命令会访问/etc/apt/sources.list源列表

2. 查询Packages.gz

   从Packages.gz中获取到所有包的信息，然后apt-get就可以通过它找到所有的包并且自动下载安装了。

3. 下载包或者依赖

   它会**首先检查依赖**,如果不存在则下载依赖包,这个依赖包或许还有依赖(递归下载),在完成了所有依赖包则可以进行下载,安装完成,中间任意一环没有完成则失败退出.
   
   



## 文件组织(/下的)

![主文件夹](/Users/zhurui/Desktop/rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/主文件夹.png)

**Linux根目录解析**

1. `/bin`：包含二进制可执行文件
2. `/sbin`：系统二进制文件，但是这个目录下的linux命令通常**由系统管理员使用，对系统进行维护**。例如`ifconfig/fdisk`。
3. `/etc`：**配置文件**，包含所有程序所需的配置文件
4. `/dev`：包含**设备文件**，这些包括终端设备、USB或连接到系统的任何设备。例如`/dev/tty1`
5. `/proc`：包含**系统进程相关信息**。这是一个虚拟的文件系统，包含有关正在运行的进程的信息；系统资源以文本信息形式存在。
6. `/var`：**变量文件**，这个目录下可以找到内容可能增长的文件。这包括 - 系统日志文件（/var/log）;包和数据库文件（/var/lib）;电子邮件（/var/mail）;打印队列（/var/spool）;锁文件（/var/lock）;多次重新启动需要的临时文件（/var/tmp）;
7. `/tmp`：包含系统和用户创建的**临时文件**，当系统重新启动时，这个目录下的文件都将被删除。
8. `/usr`：包含二进制文件、库文件、文档和二级程序的源代码。
   1. `/usr/bin`中包含用户程序的二进制文件。`/bin`
   2. `/usr/sbin`中包含系统管理员的二进制文件。`/sbin`
   3. `/usr/lib`中包含了`/usr/bin`和`/usr/sbin`用到的库。
   4. `/usr/local`中包含了从源安装的用户程序。
9. `/home`：用home目录来存储他们的个人档案。
10. `/boot`：包含**引导加载程序**相关的文件。内核的initrd、vmlinux、grub文件位于/boot下。
11. `/lib`：系统库。包含**支持位于/bin和/sbin下的二进制文件的库文件**；库文件名为 `ld*或lib*.so.*`
12. `/opt`：可选的附加应用程序。
13. `/mnt`：**挂载目录**，临时挂载目录，系统管理员可以挂载文件系统。
14. `/media`：用于挂载**可移动设备**的临时目录。举例来说，挂载CD-ROM的/media/cdrom，挂载软盘驱动器的/media/floppy;
15. `/srv`：srv代表服务。包含服务器特定服务相关的数据。



## 文件类型

见第五章



## 文件权限

三个层次：User、Group、Others

三种权限：rwx(读写执行)，当然还有之后的SUID、SGID、Sticky Bit

补充：权限设置命令见第二章**chmode命令**，特殊权限见第五章**高级系统调用**。



要懂：ls -l

```shell
-rw-r--r-- 1 root root   92 4月   9 22:35 makefile
lrwxrwxrwx 1 root root    8 4月  20 19:22 linkToMakefile -> makefile
```

第一列的第一位：文件类型(-表示普通文件)

第一列剩余位数：文件权限

第二列：硬链接数(软链接没有用)

第三列：owner所有者

第四列：group所属组

第五列：size，文件大小(字节)

第六列：mtime最近一次修改时间

第七列：文件名

[参考：ls-l详解](https://blog.csdn.net/sjzs5590/article/details/8254527)



## 进程

进程是一个正在执行的程序实例。由执行程序，它的当前值，状态信息以及通过操作系统管理此进程执行情况的资源组成。



开始和停止进程

* 所有进程均由其他进程启动
  * 父母/子女关系
  * **一个例外：init（PID 1）由内核本身启动**
  * 树层次结构
* 可以终止进程的原因有两个：
  * 该过程完成后会自行终止。
  * 该进程被另一个进程的信号终止



## Unix层次结构

![Unix内核和核外程序](/Users/zhurui/Desktop/rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/Unix内核和核外程序.png)

注意：这里的X Window System是**X Window图形用户接口**，是一种计算机软件系统和网络协议，提供了一个基础的**图形用户界面**（GUI）和丰富的**输入设备能力联网**计算机。

![UNIX-overview](/Users/zhurui/Desktop/rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/UNIX-overview.png)

