# Linux特殊权限--SUID、SGID、SBIT

> By Leonezhurui, Software Engineering



## SUID

Set UID

当s这个标志出现在**文件所有者owner的权限x上**时，例如/usr/bin/passwd的权限状态是`”-rwsr-xr-x”`，这样的形式就被称为SetUID。



SUID限制和功能

1. 首先，SUID**仅对二进制程序有效**; 
2. 执行者要对该程序**具有x权限;**
3. 这个权限是在程序的**执行过程中发挥作用的**;
4. **执行者将暂时拥有程序owner的权限**。



例子：

```shell
[root@Leoneserver ~]# ls -l /usr/bin/passwd
-rwsr-xr-x. 1 root root 27832 6月  10 2014 /usr/bin/passwd
```

注意到这里owner的执行权限上是s；

这个s的体现在于：该可执行文件的所有者是root，对于root，他肯定是可以使用passwd命令来修改密码的；但是对于另一个普通用户zhangsan，由于特殊权限SUID，zhangsan会暂时拥有程序owner的权限，所以zhangsan也可以使用passwd命令修改密码，但是其只可以修改自己的密码。



看文件`/etc/shadow`的权限：

```shell
[root@Leoneserver ~]# ls -l /etc/shadow
---------- 1 root root 778 2月  25 20:12 /etc/shadow
```

注意到`/etc/shadow`文件权限是`000`，也就是说只有root有权编辑修改(root不受到权限的限制)。

由于普通用户zhangsan是暂时拥有root的权限，所以其执行passwd命令可以修改`/etc/shadow`里面的内容。



那如果是使用`/bin/cat`命令，则不行，原因如下：

看文件`/bin/cat`的权限：

```shell
[root@Leoneserver ~]# ls -l /bin/cat
-rwxr-xr-x. 1 root root 54080 11月  6 2016 /bin/cat
```

注意到`/bin/cat`并没有SUID权限，所以普通用户zhangsan使用`cat /etc/shadow`命令时，则不能访问。

```shell
[zhangsan@Leoneserver root]$ cat /etc/shadow
cat: /etc/shadow: 权限不够
```



## SGID

与SUID类似，SGID就是s在表示**Group**权限的x处。

**SGID的功能主要分为两方面一是对文件，二是对目录，首先我们看SGID对文件有哪些功能：**

1. SGID同样是对二进制程序有效。
2. SGID同样要求程序的执行者要具有x权限。
3. 与SETUID类似的是，程序的执行者会在执行的过程中获得用户组的支持。



## Sticky Bit(SBIT)

SBIT即Sticky Bit，它出现在其他用户权限的执行位x上，**只能用来修饰一个目录**。当某一个目录拥有SBIT权限时，则任何一个能够在这个目录下建立文件的用户，该用户在这个目录下所建立的文件，**只有该用户自己和root可以删除，其他用户均不可以。**



查看`/tmp`的权限：

```shell
drwxrwxrwt.  9 root root 4096 4月  9 03:37 tmp
```

注意到`/tmp`其他用户权限的执行位上是t，这就表示SBIT。

下面是对SBIT的测试：

普通用户zhangsan在`/tmp`中新建一个`su.java`，然后切换到普通用户admin上，尝试删除`su.java`，发现是不允许的！

```shell
[zhangsan@Leoneserver tmp]$ touch su.java
[admin@Leoneserver tmp]$ rm -rf su.java 
rm: 无法删除"su.java": 不允许的操作
```



