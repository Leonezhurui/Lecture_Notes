# Linux程序设计复习--第二章

> By Leone, Software Engineering

注意：小标题中的no表示不要掌握，看看就行！

## Linux Commands



### 列出目录内容

#### ls命令

常用参数：-l(详细信息)、-a(所有文件信息)、-i(inode)、-R(递归)、-d(当前目录)

#### dir/vdir命令(no)

dir命令和ls命令类似，vdir命令和ls -l命令类似。

dir命令并不是针对目录的，可以查看所有文件的信息。

```shell
[root@Leoneserver home]# dir su.java 
su.java
[root@Leoneserver home]# vdir su.java 
-rw-r--r-- 1 root root 0 4月  21 10:58 su.java
```



### 创建特殊文件

#### mkdir命令

用于创建目录。

-p选项：递归创建一个目录

```shell
mkdir -p /Linux/su
```



#### ln命令

创建硬链接和软链接

命令格式：`ln (-s) 源文件 目标文件`

```shell
# 命令
ln -s makefile linkToMakefile
ln makefile hardlinkToMakefile
# 结果
-rw-r--r-- 2 root root   92 4月   9 22:35 hardlinkToMakefile
lrwxrwxrwx 1 root root    8 4月  20 19:22 linkToMakefile -> makefile
-rw-r--r-- 2 root root   92 4月   9 22:35 makefile
```



#### mknod命令(no)

用于创建一个设备文件。

mknod 的标准形式为:    

```shell
# mknod 的标准形式为: 
mknod DEVNAME {b | c}  MAJOR  MINOR
```

DEVNAME：设备名称

b｜c：分别表示块设备和字符设备

MAJOR  MINOR：主设备号和次设备号

例：

```shell
mkdir -p  /dev/cobing
mknod /dev/cobing/mydev1 c 128 512
```



#### mkfifo命令(no)

了解管道：

- 管道是一种**通信机制，用于进程间的通信**（也可通过socket进行网络通信），表现出来的形式将前面的每一个进程的输出，直接作为下一个进程的输入
- 管道命令仅能处理stdout，而error则会忽略



mkfifo创建命名一个管道

```shell
mkfifo fifo 
```



### 文件操作

#### cp命令

主要用于复制文件或目录。

选项：

- -r：若给出的源文件是一个目录文件，此时将复制该目录下所有的子目录和文件。(递归复制)

```shell
# 语法
cp [options] source dest
# 例
cp –r test/ newtest
```



#### mv命令

用来为文件或目录改名、或将文件或目录移入其它位置。

```shell
# 语法
mv [options] source dest
# 例
mv su.java su.python #将su.java改名为su.python
mv /usr/runoob/*  . # 将/usr/runoob文件夹下所有文件移到当前文件夹下。
```



#### rm命令

用于删除一个文件或者目录。

选项：

- -f 即使原档案属性设为唯读，亦直接删除，**无需逐一确认**。
- -r 将目录及以下之档案亦逐一删除。(递归删除)

```shell
rm  -rf  /* # 死亡操作 
```



### 修改文件属性

#### chmod

三种方式

第一种方法： 在原来权限的基础上进行加减

```shell
chmod u+x,g+x,o-w file.c
```

第二种方法：取消原来的权限，赋新的权限

```shell
chmod u=rwx,g=rwx,o=r file.c
```

第三种方法：数字赋权限

```shell
chmod 0774 file.c
```

注意：r-4,w-2,x-1



#### 补充：默认文件权限

新创建的文件和目录的默认权限为：

File: -rw-r--r-- 644 

Directory: drwxr-xr-x 755

与umask(权限掩码)有关

![权限和掩码](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/权限和掩码.png)



#### chown、chgrp(no)

chown用于设置文件所有者和文件关联组的命令。

chgrp用于变更文件或目录的所属群组。



#### touch

用于修改文件或者目录的时间属性，包括存取时间和更改时间。若文件不存在，系统会建立一个新的文件。





### 查找文件

#### locate命令(no)

用于查找符合条件的文档，他会去保存文档和目录名称的数据库内，查找合乎范本样式条件的文档或目录。



#### find命令

用来在指定目录下查找文件。任何位于参数之前的字符串都将被视为欲查找的目录名。如果使用该命令时，不设置任何参数，则 find 命令将在当前目录下查找子目录与文件。并且将查找到的子目录和文件全部进行显示。

```shell
# 语法
find   path   -option   [   -print ]   [ -exec   -ok   command ]   {} \;
```

选项：

* -name name, -iname name : 文件名称符合 name 的文件。iname 会忽略大小写
* -type c : 文件类型是 c 的文件。
* -exec  command  {} \;   —–将查到的文件执行command操作，`{} `和` \;`之间有空格
* -ok 和-exec相同，只不过在操作前要询用户

```shell
# 例
find . -name "*.c" # 将当前目录及其子目录下所有文件后缀为 .c 的文件列出来:
find . -type f -perm 644 -exec ls -l {} \;# 查找当前目录中文件属主具有读、写权限，并且文件所属组的用户和其他用户具有读权限的文件：
```

注意：

1. find常和grep，xargs命令混合使用。

2. locate 与 find 不同: find 是去硬盘找，locate 只在 /var/lib/slocate 资料库中找。(locate并不是真正的查找)



### 字符串匹配

#### grep命令

用于查找文件里符合条件的字符串。

常用于管道，和find一起使用



### 查看文件内容

more：会以一页一页的形式显示，更方便使用者逐页阅读，而最基本的指令就是**按空白键（space）就往下一页显示**，**按 b 键就会往回（back）一页显示**

less：于more类似，less 可以随意浏览文件，支持翻页和**搜索**，支持**向上翻页和向下翻页**。(less比more更强大)

head：默认显示开头10行

tail：默认显示结尾10行

cat ：用于连接文件并打印到标准输出设备上



### 进程操作

#### ps命令

用于显示当前进程的状态。

选项

* -A 显示所有进程
* -e 等于“-A”
* -aux 显示所有包含其他使用者的进程



#### pstree命令

pstree命令将所有进程以**树状图**显示，树状图将会以 pid (如果有指定) 或是以 init 这个基本进程为根 (root)，如果有指定使用者 id，则树状图会只显示该使用者所拥有的进程。



#### kill命令

用于删除执行中的程序或工作。

选项：

- 9 (KILL)：杀死一个进程。

```shell
kill -9 $(ps -ef | grep hnlinux) #过滤出hnlinux用户进程 
```



#### jobs命令(no)

可以用来查看当前终端放入后台的工作。



#### fg命令(no)

将后台中的命令调至前台继续运行



#### bg命令(no)

将一个在后台暂停的命令，变成继续执行



#### nohup命令(no)

no hang up（不挂起），用于在系统后台不挂断地运行命令，退出终端不会影响程序的运行。



#### nice命令(no)

Linux nice命令**以更改过的优先序来执行程序**，如果未指定程序，则会印出目前的排程优先序，内定的 adjustment 为 10，范围为 -20（最高优先序）到 19（最低优先序）。



### 其它

#### pwd命令

用于显示工作目录。

直接在命令行中键入pwd。



#### cd命令

用于切换当前工作目录。

注意：**~** 也表示为 home 目录 的意思， **.** 则是表示目前所在的目录， **..** 则表示目前目录位置的上一层目录。

```shell
# 语法
cd [dirName]
# 例
cd ../.. # 跳到目前目录的上上两层
```



#### ar命令(no)

ar是打包命令，用于建立或修改备存文件，或是从备存文件中抽取文件。



#### file命令(no)

用于辨识文件类型。

```shell
[root@Leoneserver sh]# file C_Lib/
C_Lib/: directory
```



#### tar命令

```shell
tar zxvf application.tar.gz 
```

注意：tar命令是打包/解包命令，并不是压缩命令

**参数：**

* -z或--gzip或--ungzip 通过**gzip指令**处理备份文件。

* -x或--extract或--get 从备份文件中还原文件(提取)。

* -c或--create 建立新的备份文件(打包)。

* -v或--verbose 显示指令执行过程。

* -f<备份文件>或--file=<备份文件> 指定备份文件。

常用参数组合：

解包：tar xvf FileName.tar
打包：tar cvf FileName.tar DirName

解压：tar zxvf FileName.tar.gz
压缩：tar zcvf FileName.tar.gz DirName

注意：这里的DirName是需要打包的文件或文件夹的名称。



#### who命令

用于显示系统中**有哪些使用者**正在上面，显示的资料包含了使用者 ID、使用的终端机、从哪边连上来的、上线时间、呆滞时间、CPU 使用量、动作等等。



#### whoami命令

显示自身用户名称。

```shell
[root@Leoneserver include]# whoami
root
```



#### passwd命令

用来更改使用者的密码。



#### su命令

用于变更为其他使用者的身份，除 root 外，需要键入该使用者的密码。



#### uname命令

用于**显示系统信息**。

选项：

* -a：显示全部的信息。

```shell
[root@Leoneserver include]# uname -a
Linux Leoneserver 3.10.0-514.26.2.el7.x86_64 #1 SMP Tue Jul 4 15:04:05 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux
```



### 文本编辑器

vi/vim

emacs

gedit





## 重定向

* 重定向
* 标准输入、标准输出、标准错误
  * 对应的文件描述符：0, 1, 2
  * C语言变量：stdin, stdout, stderr
* `<`, ` >`, ` >>`, ` 2>`
  * 例：kill –HUP 1234 > killout.txt 2> killerr.txt
  * 例：kill –HUP 1234 > killout.txt 2>& 1



**输出重定向**

| 类 型                      | 符 号                    | 作 用                                                        |
| -------------------------- | ------------------------ | ------------------------------------------------------------ |
| 标准输出重定向             | command >file            | 以覆盖的方式，把 command 的正确输出结果输出到 file 文件中。  |
|                            | command >>file           | 以追加的方式，把 command 的正确输出结果输出到 file 文件中。  |
| 标准错误输出重定向         | command 2>file           | 以覆盖的方式，把 command 的错误信息输出到 file 文件中。      |
|                            | command 2>>file          | 以追加的方式，把 command 的错误信息输出到 file 文件中。      |
| 正确输出和错误信息同时保存 | command >file 2>&1       | 以覆盖的方式，把正确输出和错误信息同时保存到同一个文件（file）中。 |
|                            | command >>file 2>&1      | 以追加的方式，把正确输出和错误信息同时保存到同一个文件（file）中。 |
|                            | command >file1 2>file2   | 以覆盖的方式，把正确的输出结果输出到 file1 文件中，把错误信息输出到 file2 文件中。 |
|                            | command >>file1 2>>file2 | 以追加的方式，把正确的输出结果输出到 file1 文件中，把错误信息输出到 file2 文件中。 |

注意：

`i>&j`

1. 重定向文件描述符i到j. 
2. 指向i文件的所有输出都发送到j. 

例如上面的`2>&1`

1. 重定向stderr到stdout. 
2. 将错误消息的输出, 发送到与标准输出所指向的地方. 



**输入重定向：**

| 符号                    | 说明                                                         |
| ----------------------- | ------------------------------------------------------------ |
| command < file          | 将 file 文件中的内容作为 command 的输入。                    |
| command << END          | 从标准输入（键盘）中读取数据，**直到遇见分界符 END 才停止**（分界符可以是任意的字符串，用户自己定义）。 |
| command < file1 > file2 | 将 **file1 作为 command 的输入**，并**将 command 的处理结果输出到 file2**。 |





## 管道命令符

* 管道
* **一个进程的输出作为另一个进程的输入**
* 例: 
  * `ls | wc –l`：存在一个管道文件，这个文件作为输入传递给后面的命令
  * ls –lF | grep ^d
  * ar t /usr/lib/libc.a | grep printf | pr -4 -t



## 环境变量

* 环境变量
  * 操作环境的参数
  * 查看和设置环境变量
  * echo, env, set
* 例: PATH环境变量
  * `echo $PATH`
  * `/usr/local/bin:/bin:/usr/bin:/usr/X11R6/bin:/home/song/bin`
  * `PATH=$PATH:.`
  * `export PATH`





## 正则表达式

### sed

ppt上例子：

```shell
sed 's/\([0-9A-Za-z_]\{1,\}\)\[ \{0,\}\]\[ \{0,\}\]/*\1\[\]/g' code1.cpp
```

注意下面列出来的含义：

```shell
\(..\)
x\{m,\}
\1
```



注意作业一

```shell
# 将当前目录下所有子目录下以a开头的.cpp文件改名为 以b开头的.c文件。
find ./ -name "a*.cpp" -exec rename 's#
(.*/)a([^/]*).cpp#$1b$2.c#g' {} \;
```



