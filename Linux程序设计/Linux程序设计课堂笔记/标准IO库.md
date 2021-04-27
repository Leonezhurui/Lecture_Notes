# 标准I/O库

> By Leonezhurui, Software Engineering



## 标准I/O

标准的I/O是由ANSI C标准规定的，不仅可以在UNIX/Linux上使用，还可以在其他地方使用。

标准I/O库对应文件的操作是围绕**流（stream）**进行的。当用标准I/O库打开或创建文件时，便**使一个流与一个文件相关联**。

标准I/O库的函数**对于文件的描述是基于FILE对象指针的**，该对象是一个结构体，包含了I/O库管理流所需要的全部信息：**用于实际I/O的文件描述符**、**指向流缓存的指针**、缓存长度、当前在缓存中的字符数、出错标志等等。



## 标准输入、标准输出、标准出错

和文件描述符STDIN_FILENO、STDOUT_FILENO、STDERR_FILENO类似，标准I/O也有这三项，它们是通过预定义的指针**stdin、stdout、stderr**来引用的。这三个指针**定义于头文件<stdio.h>**。



## 缓冲

缓冲的作用**就是把进行I/O操作的数据先存起来，等到存到一定量时再进行实际的I/O操作**。系统调用read和write是不带缓冲的，所以每次调用都会进行I/O操作。标准I/O是带缓冲的，它会对流自动进行缓冲管理。标准I/O提供以下三种缓冲：

1. **全缓冲**：指的是在填满I/O缓冲区后才进行实际的I/O操作。对于磁盘上的文件通常是由标准I/O实施全缓冲的。在一个流上第一次执行I/O操作时，相关标准I/O函数通常调用malloc开辟使用的缓冲区。
2. **行缓冲**：指的时在输入或输出**遇到换行符**时，标准I/O库执行实际的I/O操作。当流涉及到终端时（如标准输入、标准输出），通常使用行缓冲。
3. **不带缓冲**：指的是标准I/O库不对字符进行缓冲存储，直接调用系统调用。**标准出错stderr通常是不带缓冲的，这样就可以使得错误信息尽快显示出来**。



## I/O C库函数

### 打开流

```C
#include<stdio.h>
FILE *fopen(const char *restrict pathname, const char *restrict type);
FILE *fopen(const cahr *restrict pathname, const char *restrict type, FILE *restrict fp);
FILE *fdopen(int filedes, const char *type);
```



### 关闭流

```C
#include<stdio.h>
int fclose(FILE *fp);
```





### 读写流

流打开之后，可以在**三种不同类型的非格式化I/O**（还有格式化的I/O）中进行选择，对其进行读写操作：

1. **每次一个字符**的I/O。每次读或写一个字符，如果流是带缓冲的，则标准I/O会处理所有缓冲。
2. **每次一行**的I/O。如果想要一次读或写一行，使用fgets或fputs，每次都以一个换行符终止。
3. **直接I/O**。fread和fwrite支持这种类型的I/O。每次I/O操作读或写某种数量的对象，而这个对象具有指定的长度。这两个函数常用于从二进制的文件中每次读或写一个结构。



**每次读入一个字符的I/O**（函数结尾c说明每次一个字符,char）

```C
#include<stdio.h>
int getc(FILE* fp);
int fgetc(FILE *fp);
int getchar(void);//等价于getc(stdin)
```

对应于上面，每个输入函数都有一个输出函数

```C
#include<stdio.h>
int putc(int c, FILE *fp);
int fputc(int c, FILE *fp);
int putchar(int c);//等价于putchar(c,stdout);
```



**每次一行I/O**（函数结尾s说明每次一行，string)

```C
#include<stdio.h>
char *fgets(char *restrict buf, int n, FILE *restrict fp);
char *gets(char *buf);//从标准输入读
```


对应于上面，每个输入函数都有一个输出函数

```C
#include<stdio.h>
int fputs(const char *restrict str, FILE *restrict fp);
int puts(const char *str);
```



**二进制I/O**
使用二进制I/O**常常一次操作一个结构**。如果使用getc或puts读写一个结构，那么**必须循环通过整个结构**，每次循环处理一个字节，非常耗时。如果使用fputs或fgets，那么fputs/fgets遇到null字节就停止，而结构中可能含有NULL字节，不能实现读结构的要求。因此提供了两个函数执行二进制的I/O.

```C
#include<stdio.h>
size_t fread(void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
size_t fwrite(const void *restrict ptr, size_t size, size_t nobj, FILE *restrict fp);
```


二进制I/O的基本问题是：只能用于在同一文件系统上操作。随着网络文件系统系统的兴起，这两个函数的使用就受到限制。