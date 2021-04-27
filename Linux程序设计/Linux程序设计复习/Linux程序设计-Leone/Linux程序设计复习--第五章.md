# Linux程序设计复习--第五章

> By Leone, Software Engineering

## 文件和文件系统简单概念(背)

* 文件
  * 可以写入或读取或两者兼有的对象。 文件具有某些属性，包括访问权限和类型。
* 文件系统
  * 文件及其某些属性的集合。 它为引用这些文件的文件序列号提供了名称空间。



## 七种文件类型

File types

* regular file(普通文件)
* character special file(字符文件)
* block special file(块文件)
* fifo(管道文件)
* socket(套接字文件)
* symbolic link(链接文件)
* directory(目录文件)



## VFS基本原理(背)

### VFS

VFS（Virtual Filesystem Switch）称为**虚拟文件系统或虚拟文件系统转换**，是一个**内核软件层**，**在具体的文件系统之上抽象的一层**，用来处理与Posix文件系统相关的所有调用，表现为**能够给各种文件系统提供一个通用的接口**，**使上层的应用程序能够使用通用的接口访问不同文件系统**，同时也为不同文件系统的通信提供了媒介。

![VFS示意图](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/VFS示意图.png)

VFS的作用就是采用标准的Unix系统调用读写**位于不同物理介质上的不同文件系统**。VFS是一个可以让open()、read()、write()等系统调用**不用关心底层的存储介质和文件系统类型就可以工作的粘合层**。



### VFS model

**统一文件模型（common file model）**

VFS为了提供**对不同底层文件系统的统一接口**，需要有一个高度的抽象和建模，这就是VFS的核心设计——**统一文件模型**。

Unix家族的VFS的文件模型定义了**四种对象**，这四种对象构建起了统一文件模型。

- superblock：存储**文件系统基本的元数据**。如文件系统类型、大小、状态，以及其他元数据相关的信息（元元数据）
- index node（inode）：**保存一个文件相关的元数据**。包括文件的所有者（用户、组）、访问时间、文件类型等，**但不包括这个文件的名称**。文件和目录均有具体的inode对应
- directory entry（dentry）：**保存了文件（目录）名称和具体的inode的对应关系**，用来粘合二者，同时可以实现目录与其包含的文件之间的映射关系。另外也作为缓存的对象，缓存最近最常访问的文件或目录，提示系统性能
- file：一组逻辑上相关联的数据，被一个进程打开并关联使用(**文件对象是打开一个具体文件之后创建的一个内存数据结构**，与具体的进程和用户相联系。)





## 软硬链接(背)

* Hard link
  * **不同的文件名对应同一个inode**
  * **不能跨越文件系统**
  * 对应系统调用link
* Symbolic link
  * **存储被链接文件的文件名(而不是inode)实现链接**
  * **可跨越文件系统**
  * 对应系统调用symlink

补充：

* 创建命令：硬链接`ln source target`；软链接`ln -s source target`
* 系统调用：硬链接`link(const char *oldpath, const char *newpath)`；软链接`symlink(const char *oldpath, const char *newpath)`



## 系统调用(背)

### 系统调用 vs C库函数

* **都以C函数的形式出现**(在其他语言中，对应于其他语言的函数)
* 系统调用
  * Linux内核的对外接口; 用户程序和内核之间唯一的接口; 提供最小接口
* 库函数
  * 依赖于系统调用; 提供较复杂功能
  * 例：标准I/O库





### Basic I/O System Calls

* File descriptor
* Basic I/O
  * open/creat, close, read, write, lseek
  * dup/dup2
  * fcntl
  * ioctl



#### File Descriptor

* **File descriptor**(文件描述符)
  * A small non-negative **integer**(0/1/2/3/4这样的值，用来指向一个文件对象，各个进程中的文件描述符是独立的，用户态进程与内核交流的工具)
    * int fd;
  * (in **<unistd.h>**) 类unix系统的头文件
    * **STDIN_FILENO (0), STDOUT_FILENO (1), STDERR_FILENO (2)** 
    * 在一个系统下，创建一个进程，预分配的三个文件描述符(system.in是0号文件描述符，system.out是1号，system.error是2号)
* General steps of file operation
  * open-read/write-[lseek]-close



**打开现存文件或新建文件**时，系统（内核）会返回一个文件描述符，文件描述符用来指定已打开的文件。这个文件描述符相当于这个已打开文件的标号，文件描述符是非负整数，是**文件的标识，操作这个文件描述符相当于操作这个描述符所指定的文件**。

程序运行起来后（**每个进程）都有一张文件描述符的表**，标准输入、标准输出、标准错误输出设备文件被打开，对应的文件描述符 0、1、2 记录在表中。程序运行起来后这三个文件描述符是**默认打开**的:

```C
 #define STDIN_FILENO  0 //标准输入的文件描述符
 #define STDOUT_FILENO 1 //标准输出的文件描述符
 #define STDERR_FILENO 2 //标准错误的文件描述符上面3个常量定义在`/usr/include/unistd.h`中。
```

在程序运行起来后打开其他文件时，系统会**返回文件描述符表中最小可用的文件描述符**，并将此文件描述符记录在表中。Linux 中一个进程最多只能打开 **NR_OPEN_DEFAULT** （即1024）个文件，故**当文件不再使用时应及时调用 close() 函数关闭文件**。



**系统调用I/O函数**需要的头文件：(记住)

```csharp
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
```



#### open函数

```C
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);//注意这里open的参数是以可变参数的形式出现的。
```

功能：

打开文件，**如果文件不存在则创建**。



参数：

**pathname**: **文件的路径及文件名**。

**flags**: 打开文件的行为标志，如，以只读方式（O_RDONLY，第一个为字母不是零）打开，以读写或新建新文件的方式（O_RDWR|O_CREAT）打开。

![linux系统调用flags](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/linux系统调用flags.png)

**mode**: 这个参数，**只有在文件不存在时有效**，指新建文件时指定文件的权限。(在指定文件权限的时候，使用数字法比较简单，并应该采用0xxxx八进制写法。)

![系统调用权限mode](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/系统调用权限mode.png)

**返回值**：

* 成功：成功返回打开的文件描述符

* 失败：-1



#### close函数

```C
int close(int fd);
```

功能：

关闭已打开的文件

参数：

fd: 文件描述符，open()的返回值

返回值：

成功：0

失败：-1



#### write函数

```C
ssize_t write(int fd, const void *addr, size_t count);
```

功能：

**把指定数目的数据写到文件（fd）**

参数：

* fd: 文件描述符

* addr: **数据首地址**

* count: 写入数据的长度（字节），一般情况下，数据有多少，就往文件里写多少，不能多也不能少

 返回值：

* 成功：**实际写入数据的字节个数**

* 失败：-1


注意：size_t被定义为unsigned long，而ssize_t(有符号的大小）被定义long。



#### read函数

```C
ssize_t read(int fd, void *addr, size_t count);
```

功能：

把指定数目的数据读到内存（缓冲区）

参数：

* fd: 文件描述符

* addr: 内存首地址

* count: 读取的字节个数

返回值：

* 成功：实际读取到的字节个数

* 失败：-1



#### lseek函数

所有的文件都有一个**当前文件偏移量**(current file offset)，cfo通常是一个非负整数，**用于表明文件开始处到文件当前位置的字节数**。

读写操作通常开始于 cfo，并且使 cfo 增大，**增量为读写的字节数**。文件被打开时，cfo 会被初始化为 0，除非使用了**O_APPEND**(数据添加到文件末尾)。



使用 lseek 函数可以改变文件的 cfo 。

```C
#include <unistd.h>
#include <sys/types.h>
off_t lseek(int fd, off_t offset, int whence);
```

返回值：新的偏移量（成功），-1（失败）



参数 offset 的含义取决于参数 whence：

1. 如果 whence 是 **SEEK_SET**，**文件偏移量将被设置为 offset**。(可以理解为将位置调到文件起始位置)
2. 如果 whence 是 **SEEK_CUR**，**文件偏移量将被设置为 cfo 加上 offset**，offset 可以为正也可以为负。(可以理解为将位置调到文件当前读写位置)
3. 如果 whence 是 **SEEK_END**，**文件偏移量将被设置为文件长度加上 offset**，offset 可以为正也可以为负。(可以理解为将位置调到文件末尾位置)



lseek 的以下用法返回当前的偏移量：

```C
off_t  currpos;
currpos = lseek(fd, 0, SEEK_CUR);
```

lseek 的以下用法返回文件长度：

```C
off_t  currpos;
currpos = lseek(fd, 0, SEEK_END);
```



#### dup函数

```C
#include <unistd.h>
int dup(int oldfd);
```

dup用来复制参数oldfd所指的文件描述符。复制成功时，**返回最小的尚未被使用过的文件描述符**，若有错误则返回-1。

返回的新文件描述符和参数oldfd**指向同一个文件**，这两个描述符共享同一个数据结构，共享所有的锁定、读写指针和各项权限或标志位



调用dup(oldfd)等效于 

```C
fcntl(oldfd, F_DUPFD, 0)
```



#### dup2函数

```C
 #include <unistd.h>
 int dup2(int oldfd, int newfd);
```

`dup2`与`dup`区别是**`dup2`可以用参数`newfd`指定新文件描述符的数值**。

若参数`newfd`已经被程序使用，则系统就会将`newfd`所指的文件关闭。若`newfd`等于`oldfd`，则返回`newfd`，而不关闭`newfd`所指的文件。

`dup2`所复制的文件描述符与原来的文件描述符共享各种文件状态。共享所有的锁定，读写位置和各项权限或`flags`等。



dup2(oldfd, newfd)等效于 

```C
close(oldfd)； 
fcntl(oldfd, F_DUPFD, newfd)； 
```

在shell的重定向功能中，(输入重定向”<”和输出重定向”>”)就是通过调用dup或dup2函数对标准输入和标准输出的操作来实现的。



#### fcntl函数

fcntl系统调用可以用来**对已打开的文件描述符进行各种控制操作以改变已打开文件的的各种属性**。

```C
#include<unistd.h>
#include<fcntl.h>
int fcntl(int fd, int cmd);
int fcntl(int fd, int cmd, long arg);
int fcntl(int fd, int cmd ,struct flock* lock);
```



fcntl函数功能依据cmd的值的不同而不同。参数对应功能如下：

![fcntl函数命令参数](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/fcntl函数命令参数.png)

1. F_DUPFD

   与dup函数功能一样，**复制由fd指向的文件描述符**，调用成功后返回新的文件描述符，与旧的文件描述符共同指向同一个文件。

2. F_GETFD

   读取文件描述符**close-on-exec标志**

3. F_SETFD

   将文件描述符close-on-exec标志设置为第三个参数arg的最后一位

4. F_GETFL

   获取文件打开方式的标志，标志值含义与open调用一致

5. F_SETF

   设置文件打开方式为arg指定方式



### 系统调用I/O实战

文件复制到另一个文件。

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char* argv[]){
        if(argc!=3){
                printf("error parameters!");
                return -1;
        }
        int fd1, fd2;

        if((fd1=open(argv[1], O_RDONLY)) < 0){ //注意这里的括号
                printf("open error!");
                return -1;
        }
        if((fd2=open(argv[2], O_WRONLY|O_CREAT, 0644)) < 0){
                printf("open error!");
                return -1;
        }

        int n;
        do{
                char buf[1024];
                n = read(fd1, buf, sizeof(buf));
                write(fd2, buf, n);
        }while(n > 0);

        close(fd1);
        close(fd2);

        return 0;
}

```

将文件内容输出到屏幕上。

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int main(int argc, char* argv[]){
        if(argc != 2){
                printf("parameter error!\n");
                return -1;
        }
        int fd;
        if((fd=open(argv[1], O_RDONLY)) < 0){
                printf("open error!");
                return -1;
        }
        int len;
        do{
                char buf[1024];
                len = read(fd, buf, sizeof(buf));
                write(STDOUT_FILENO, buf, len);
        }while(len > 0);

        close(fd);
        return 0;
}
```







## C库函数



### buffer操作

缓冲类型：

1. Full buffer满缓存
2. Line buffer行缓存
3. No buffer无缓存



```C
#include <stdio.h>
void setbuf(FILE *stream, char *buf);
int setvbuf(FILE *stream, char *buf, int mode, size_t size);
```

type：_IOFBF(满缓冲）_IOLBF(行缓冲）_IONBF(无缓冲）

补充：

1. setbuf用于打开或关闭流缓冲机制，参数buf指向一个长度为BUFSIZ（该常量在<stdio.h>中定义）的缓冲区；如果要关闭缓冲，则将buf设置为NULL即可。

2. setvbuf用于精确地设置所需的缓冲类型，mode取值如下：_IOFBF(全缓冲)/_IOLBF(行缓冲)/_IONBF(无缓冲)；如果指定了mode为带缓冲类型，而buf却为NULL，则系统会自动分配BUFSIZ个字节的缓冲区。



### 标准I/O函数



#### open/close

* Open a stream

```C
#include <stdio.h>
FILE *fopen(const char *filename, const char *mode);
```

* Parameter “mode”
  * “r”:Open text file for **reading**.
  * “w”:Truncate file to zero length or create text file for writing.
  * “a”:Open for **appending**.(追加)
  * “r+”:Open for **reading and writing**.
  * “w+”:Open for reading and writing. The file is created if it does not exist, **otherwise it is truncated**.(清空，读写)
  * “a+”:Open for **reading and appending**. The file is created if does not exist.



* Close a stream

```C
#include <stdio.h>
int fclose(FILE *fp);
(Return: 0 if success; -1 if failure) 
```





#### read/write

##### 每次一个字符的I/O

Input of a character

* getc, fgetc, getchar functions

```C
#include <stdio.h>
int getc(FILE *fp);
int fgetc(FILE *fp);
int getchar(void);
(Result: Reads the next character from a stream and returns it as an unsigned char cast to an int, or EOFon end of file or error.) 强制类型转换成int
```

* Three functions:
  * ferror, feof, clearerr
* ungetc function: push a character back to a stream.

fgetc和getc的差别：C库并没有说有啥差别。在大部分的实现中，getc是被写成宏的(在编译之前就被替换掉)，而fgetc则被写成函数。所以，getc会被fgetc快一点；getc更容易出错(由于是宏替换)。



Output of a Character

* putc, fputc, putchar functions

```C
#include <stdio.h>
int putc(int c, FILE *fp);
int fputc(int c, FILE *fp);
int putchar(int c);
(Return: the character if success; -1 if failure) 
```



##### 每次一行的I/O

Input of a Line of String

* fgets, gets functions

```C
#include <stdio.h>
char *fgets(char *s, int size, FILE *stream);
char *gets(char *s); //not recommended.
```

* fgets: reads in at most **size-1**characters from streamand stores them into the buffer pointed by s. **Reading stops after an EOF or a new line**. A **„\0‟**character is stored at the end of the buffer.

最多读size-1个字符，由于最后一个字符得填充`\0`；最多读一行



Output of a Line of String

* fputs, puts functions

```C
#include <stdio.h>
int fputs(const char *s, FILE *stream);
int puts(const char *s);
```





##### 直接I/O(二进制I/O) 

Binary Stream Input/Output

* fread/fwrite functions

```C
#include <stdio.h>
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
size fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
(Return: the number of a items successfully read or written.) 
```



##### 格式化I/O

Formatted I/O

* scanf, fscanf, sscanf functions

```C
#include <stdio.h>
int scanf(const char *format, ...);//从0号文件描述符里读
int fscanf(FILE *stream, const char *format, ...);//从文件中读数据。给一个文件的流指针(包含文件描述符)，每次读一行，当作是字符串，按照format格式做匹配，然后填到后面的变量中。注意后面的变量必须是地址。
int sscanf(const char *str, const char *format, ...);//由内存buffer中取出字符串，进行操作
```

* Use fgets, then parse the string.



Formatted I/O (cont‟d) 

* printf, fprintf, sprintf functions

```C
#include <stdio.h>
int printf(const char *format, ...);
int fprintf(FILE *stream, const char *format, ...);//写到文件中
int sprintf(char *str, const char *format, ...);//拿到这个字符串，写到char* buffer中
```



#### Reposition a stream

* fseek, ftell, rewind functions

```C
#include <stdio.h>
int fseek(FILE *stream, long int offset, int whence);//给一个文件流指针，跳转量，跳转方式(三个值，三个#define SEEK_SET\SEEK_CUR\SEEK_END)
long ftell(FILE *stream);//得到当前跳转位置
void rewind(FILE *stream);//重置，将跳转到文件开头
```

* fgetpos, fsetpos functions ( Introduced in ANSI C) 

```C
#include <stdio.h>
int fgetpos(FILE *fp, fpos_t *pos);
int fsetpos(FILE *fp, const fpos_t *pos);
//fpos_t是自定义类型，适用于文件相当大的时候
```



#### Flush a stream

* **刷新文件流。把流里的数据立刻写入文件**

```C
#include <stdio.h>
int fflush(FILE *stream);//防止数据流在缓存中
```



Stream and File Descriptor

* **确定流使用的底层文件描述符**

```C
#include <stdio.h>
int fileno(FILE *fp);
```

* **根据已打开的文件描述符创建一个流**

```C
#include <stdio.h>
FILE *fdopen(int fildes, const char *mode);
```





## 高级系统调用

### 处理文件权限

stat/fstat/lstat functions

* Get **file status**

```C
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *filename, struct stat *buf);//buf需要实现开好
int fstat(int filedes, struct stat *buf);
int lstat(const char *file_name, struct stat *buf);//stat系统内部定义的结构体
(Return: 0 if success; -1 if failure) 
```

如果使用stat来查看软链接，则会返回软链接所指向的那个原始文件的文件属性；而如果使用lstat，则会返回软链接的文件属性。



struct stat

```C
struct stat {
	mode_t st_mode; /*file type & mode*/
	ino_t st_ino; /*inode number (serial number)*/
	dev_t st_rdev; /*device number (file system)设备号*/ 
	nlink_t st_nlink; /*link count硬链接计数*/
	uid_t st_uid; /*user ID of owner*/
	gid_t st_gid; /*group ID of owner*/
	off_t st_size; /*size of file, in bytes*/
	time_t st_atime; /*time of last access*/
	time_t st_mtime; /*time of last modification*/
	time_t st_ctime; /*time of last file status change*/
	long st_blksize; /*Optimal block size for I/O*/
	long st_blocks; /*number 512-byte blocks allocated*/
};
```



Test macros for file types

* Defined in <sys/stat.h> 

| Macro      | File type                 |
| ---------- | ------------------------- |
| S_ISREG()  | regular file              |
| S_ISDIR()  | directory                 |
| S_ISCHAR() | character special file    |
| S_ISBLK()  | block special file        |
| S_ISFIFO() | fifo                      |
| S_ISLNK()  | symbolic link(特指软链接) |
| S_ISSOCK() | socket                    |

注意：在我的Ubuntu中，表示字符文件的宏是S_ISCHR()，可能是不同系统的问题。



#### SUID、SGID、SBIT

**SUID：**

Set UID

当s这个标志出现在**文件所有者owner的权限x上**时，例如/usr/bin/passwd的权限状态是`”-rwsr-xr-x”`，这样的形式就被称为SetUID。



SUID限制和功能

1. 首先，SUID**仅对二进制程序有效**; 
2. 执行者要对该程序**具有x权限;**
3. 这个权限是在程序的**执行过程中发挥作用的**;
4. **执行者将暂时拥有程序owner的权限**。



例：普通用户zhangsan使用`usr/bin/passwd`和`/bin/cat`命令对于文件`/etc/passwd`(权限000)的操作是不一样的，由于passwd命令的所有者有s权限，所以普通用户暂时获得owner的权限。`su/sudo`命令也是有SUID权限的。



**SGID：**

SGID对普通文件和目录都是有效的，效果和SUID类似。



**SBIT：**

SBIT即Sticky Bit，它出现在其他用户权限的执行位x上，**只能用来修饰一个目录**。当某一个目录拥有SBIT权限时，则任何一个能够在这个目录下建立文件的用户，该用户在这个目录下所建立的文件，**只有该用户自己和root可以删除，其他用户均不可以。**

例：`/tmp`目录的权限是`drwxrwxrwt. `，注意其在others的x权限上是t，说明`/tmp`目录具有SBIT权限。那么普通用户admin就不能删除普通用户zhangsan在`/tmp`目录中创建的`su.java`文件。



File permission

![文件权限1](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/文件权限1.png)



![文件权限3](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/文件权限3.png)





#### chmod/fchmod

* Change permissions of a file

```C
#include <sys/types.h>
#include <sys/stat.h>
int chmod(const char *path, mode_t mode);
int fchmod(int fildes, mode_t mode);//系统调用，文件描述符已经打开
(Return: 0 if success; -1 if failure) 
```



#### umask function

* **为进程设置文件存取权限屏蔽字，并返回以前的值**

```C
#include <sys/types.h>
#include <sys/stat.h>
mode_t umask(mode_t mask);
```

默认的文件权限是644，文件夹权限是755



#### link/symlink

link/unlink functions

* Create a new link to (make a new name for) a file.(创建新的硬链接)

```C
#include <unistd.h>
int link(const char *oldpath, const char *newpath);
(Return: 0 if success; -1 if failure) 
```

* Delete a name and possibly the file it refers to.

```C
#include <unistd.h>
int unlink(const char *pathname);
(Return: 0 if success; -1 if failure) 
```



symlink/readlink

* Create a symbolic link (named newpath which contains the sting “oldpath”) 

```C
#include <unistd.h>
int symlink(const char *oldpath, const char *newpath);
(Return: 0 if success; -1 if failure) 
```

* Read value of a symbolic link

```C
#include <unistd.h>
int readlink(const char *path, char *buf, size_t bufsiz);
(Return: the count of characters placed in the buffer if success; -1 if failure) 
```



### 处理目录



#### mkdir/rmdir functions

* 创建一个空目录

```C
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);
(Return: 0 if success; -1 if failure) 
```

* **删除一个空目录**

```C
#include <unistd.h>
int rmdir(const char *pathname);
(Return: 0 if success; -1 if failure) 
```

mkdir和rmdir和命令行中的命令刚好重名。



#### chdir/fchdir functions

* Change working directory(切换文件夹)

```C
#include <unistd.h>
int chdir(const char *path);
int fchdir(int fd);//如果已经打开了文件(有文件描述符)
(Return: 0 if success; -1 if failure) 
```

* 当前工作目录是进程的属性，所以该函数只影响调用chdir的进程本身



getcwd function

* **获得当前工作目录的绝对路径**

```C
#include <unistd.h>
char *getcwd(char *buf, size_t size);//写到缓存buf中
(返回值: 若成功则为buf，若出错则为NULL) 
```

命令行命令：`pwd`



#### Read a directory

结构体struct DIR和dirent：

```C
//DIR 目录
typedef struct __dirstream DIR;
//dirent 目录项
ino_t d_ino; /* inode number */
char d_name[NAME_MAX + 1]; /* file name */
```



Operations

* 目录的打开、关闭、读、定位

```C
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);//打开文件夹返回的是DIR结构体
int closedir(DIR *dir);

struct dirent *readdir(DIR *dir);//从文件夹读取目录项，返回的是dirent结构体，一般是你用while循环将文件夹的所有文件读出来。

off_t telldir(DIR *dir);//当前在目录中已经遍历了几项了
void seekdir(DIR *dir, off_t offset); //跳过，只有一种跳转方式(从头开始跳)
```





### 代码实战

例：(一般就这样读取文件夹)

```C
DIR *dp;
struct dirent *entry;

if ( (dp = opendir(dir)) == NULL ) 
	err_sys(…);
while ( (entry = readdir(dp)) != NULL ) { //直到entry为空
	lstat(entry->d_name, &statbuf);
	if ( S_ISDIR(statbuf.st_mode) ) 
		…
	else
		…
}
closedir(dp);
```



```C
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("parameter error！");
        return -1;
    }
    DIR* dp;
    struct dirent* entry;
    struct stat state;
    
    printf("%s\n", argv[1]);
    if( (dp = opendir(argv[1])) == NULL){
        printf("error open dir!\n");
        return -1;
    }
    while( (entry = readdir(dp)) != NULL ){
        lstat(entry->d_name, &state);
        if(state.st_mode & S_IFREG){//注意：这样判断不是很好，见下面的代码
            printf("%s\n", entry->d_name);
        }
    }
    return 0;
}
```



```C
switch(buf->st_mode & S_IFMT)//按位&获取文件基本属性，S_IFMT相当于掩码的功能
    {
        case S_IFIFO:
            mode[0] = 'f';
            break;
        case S_IFDIR:
            mode[0] = 'd';
            break;
        case S_IFSOCK:
            mode[0] = 's';
            break;
        case S_IFBLK:
            mode[0] = 'b';
            break;
        case S_IFLNK:
            mode[0] = 'l';
            break;
        case S_IFCHR:
            mode[0] = 'c';
            break;
        default:break;
    }
```





### 文件锁

#### 记录锁

当第一个进程正在读或修改文件的某个部分时，使用记录锁可以阻止其他进程修改同一文件区。

一个更适合的术语可能是**字节范围锁 (byte-range locking)，**因为它锁定的只是文件中的一个区域(也可能是整个文件)。



#### 劝告锁

劝告锁讲究的是一种协同工作，**内核仅负责对文件加锁以及检查文件是否已经上锁等操作**，而不亲自去参与文件锁的控制与协调，而这些都需要程序员首先要检查所要访问的文件之前是否已经被其他进程加锁来实现并发控制，劝告锁不仅可以对文件的任一部分加锁，也可以对整个文件加锁。下面是加锁规则：

![](https://img-blog.csdn.net/20131205195730562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpbG9uZzg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



#### 强制锁

强制锁则是**内核强制使用**的一种文件锁，**每当有进程违反锁规则，内核将会进行阻止**，具体的加锁规则如下：
           （1）若一个文件已经加上共享锁，那么其他进程在对这个文件进行写操作时将会被内核阻止；
           （2）若一个文件已经加上了排他锁，那么其他进程对这个文件的读取与写操作都将被阻止；
            下表总结了进程试图访问已经加有强制锁的文件，进程行为如下：

![](https://img-blog.csdn.net/20131205195937718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2hlbmxpbG9uZzg0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



#### 共享锁

共享锁也称为读锁。

如果一个进程为某个文件的某个区域加了一把共享锁，那么其他进程对该文件的该区域可以加共享锁不能加排他锁。




#### 排他锁

排他锁也称为写锁。

如果一个进程为某个文件的某个区域加了一把排他锁，那么其他进程无法对该文件的该区域加任何锁，包括共享锁和排他锁。





#### fcntl记录锁

* 用于记录锁的fcntl函数原型

```C
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, struct flock *lock);
(返回值: 若成功则依赖于cmd，若出错为-1) 
```



```C
struct flock{
	...
	short l_type; /* Type of lock: F_RDLCK, F_WRLCK, F_UNLCK */
	short l_whence; /* How to interpret l_start: SEEK_SET, SEEK_CUR, SEEK_END */
	off_t l_start; /* Starting offset for lock 加锁起始位置*/
	off_t l_len; /* Number of bytes to lock */
	pid_t l_pid; /* PID of process blocking our lock (F_GETLK only) 加锁进程ID，好处是别的进程想要访问的时候可以知道是哪一个文件锁住的*/
	...
}
```

l_type：F_RDLCK(共享读锁), F_WRLCK(独占性写锁), F_UNLCK(解锁一个区域)





cmd参数的取值

* F_GETLK：获得文件的封锁信息
* F_SETLK：对文件的某个区域**封锁或解除封锁**(不管加锁是否成功都立即返回)
* F_SETLKW：功能同F_SETLK, wait方式。(如果对应的加锁文件被别的进程锁住了，则该进程会被阻塞)



其它封锁命令

* lockf函数

```C
#include <sys/file.h>
int lockf(int fd, int cmd, off_t len); //len表示锁定的字符数
```



注意：功能类似的系统调用不要混用，使用什么方法加锁，就是用什么方式解锁。