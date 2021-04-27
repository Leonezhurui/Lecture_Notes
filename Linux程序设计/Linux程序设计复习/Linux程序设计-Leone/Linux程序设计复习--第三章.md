# Linux程序设计复习--第三章

> By Leone, Software Engineering

## Shell是什么？

* Shell:
  * 命令解释器和编程环境
* 用户和操作系统之间的接口
* 作为核外程序而存在

Shell: 用户和操作系统之间的接口

![shell是接口](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/shell是接口.png)

Shell: 作为核外程序而存在

![shell是核外程序](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/shell是核外程序.png)



## 各种shell

| shell名称  | 描述                                             | 位置            |
| ---------- | ------------------------------------------------ | --------------- |
| bash       | “Bourne Again Shell”。Linux中的主角，来自GNU项目 | /bin/bash       |
| sh         | bash的一个符号链接                               | /bin/sh         |
| csh        | C shell, tcsh的一个符号链接                      | /bin/csh        |
| tcsh       | 和csh兼容的shell                                 | /bin/tcsh       |
| ksh        | Korn Shell                                       | /bin/ksh        |
| ash        | 一个小的shell                                    | /bin/ash        |
| ash.static | 一个不依靠软件库的ash版本                        | /bin/ash.static |
| bsh        | ash的一个符号链接                                | /bin/bsh        |



## 脚本执行方式

* 方法1:
   $ sh script_file

* 方法2:

  chmod +x script_file (chown, chgrp optionally)

  ./script_file

* 方法3:
  source script_file, or . script_file



### 三种执行方式的区别

1. sh script 

脚本文件不需要有执行权限。

使用sh执行一个脚本时，该脚本会使用一个**新的bash环境**来执行脚本，脚本执行完成后**不会对现有的bash环境产生影响**。

2. source script(. script)

脚本文件不需要有执行权限。

使用source执行一个脚本时。该脚本**会在当前的bash环境中执行**。因此脚本中的操作会对当前的bash产生影响。这也是为什么当我们修改了~/.bashrc文件时，需要使用source ~/.bashrc在不重启系统的情况下使修改生效。

3. ./

脚本文件**需要有执行权限**。

使用./执行一个脚本时。该脚本会使用一个**新的bash环境**来执行脚本，脚本执行完成后不会对现有的bash环境产生影响。



## 用户环境

.bashrc文件主要**保存个人的一些个性化设置，如命令别名、路径等**。也即在同一个服务器上，只对某个用户的个性化设置相关。

* .bash_profile：用户登录时被读取，其中包含的命令被bash执行
* .bash_logout：登录退出时读取执行
* .bashrc：启动一个新的shell时读取并执行

注意：上述的文件都是用户目录(/home)下的。



除了可以修改用户目录下的.bashrc文件外，还可以修改如“/etc/profile”文件、“/etc/bashrc”文件及目录“/etc /profile.d”下的文件。但是**修改/etc路径下的配置文件将会应用到整个系统**，属于系统级的配置，而修改用户目录下的.bashrc则只是限制在用户应用上，属于用户级设置。两者在应用范围上有所区别，建议如需修改的话，修改用户目录下的.bashrc，即无需root权限，也不会影响其他用户。



```shell
# ~/.bashrc内容
# .bashrc

# User specific aliases and functions

alias rm='rm -i'
alias cp='cp -i'
alias mv='mv -i'

# Source global definitions
if [ -f /etc/bashrc ]; then
        . /etc/bashrc
fi
```



### alias别名

注意到上面的`alias rm='rm -i'`，说明使用的`rm`实际上是`rm -i`命令。

alias命令

```shell
alias 别名=’原命令 -选项/参数’
```



### export命令

用于设置或显示环境变量。export 的效力仅限于**该次登陆操作**(写到/etc/porfile则会持久化)。

例: PATH环境变量

* `echo $PATH`
* `/usr/local/bin:/bin:/usr/bin:/usr/X11R6/bin:/home/song/bin`
* `PATH=$PATH:.`
* `export PATH`





## read函数

```shell
read [-rs] [-a ARRAY] [-d delim] [-n nchars] [-N nchars] [-p prompt] [-t timeout] [var_name1 var_name2 ...]

```

选项说明：

* -d：指定读取行的**结束符号**。默认结束符号为换行符。
* -n：限制**读取N个字符**就自动结束读取，如果没有读满N个字符就按下回车或遇到换行符，则也会结束读取。
* -N：严格要求读满N个字符才自动结束读取，即使中途按下了回车或遇到了换行符也不结束。其中换行符或回车算一个字符。
* -p：**给出提示符**。默认不支持"\n"换行，要换行需要特殊处理，见下文示例。例如，"-p 请输入密码："
* -s：**静默模式**。输入的内容不会回显在屏幕上。
* -t：**给出超时时间**，在达到超时时间时，read退出并返回错误。也就是说不会读取任何内容，即使已经输入了一部分。





## bash中其他特殊符号

| 符号 | 作用                                                     |
| ---- | -------------------------------------------------------- |
| ' '  | 单引号。在单引号中所有符号**没有特殊含义**               |
| " "  | 双引号。符号有**特殊含义**                               |
| ``   | 反引号。反引号括起来的内容是**系统命令**，bash会先执行它 |
| $()  | 和反引号作用一样，推荐使用这个。                         |
| #    | 注释                                                     |
| $    | 用于**调用变量的值**(${})                                |
| \    | 转义符，跟在转义符后面的特殊符号将失效。                 |



## 条件测试

* 退出码
* test命令
  * test expression 或[ expression ]
* test命令支持的条件测试
  * 字符串比较
  * 算术比较
  * 与文件有关的条件测试、逻辑操作

注意：`[`是shell的内置命令(所以expression相当于是`[`的参数)，`[[`是shell的关键字



## 条件判断

字符串比较

| 字符串比较  | 结果                       |
| ----------- | -------------------------- |
| str1 = str2 | 两个字符串相同则结果为真   |
| str1!=str2  | 两个字符串不相同则结果为真 |
| -z str      | 字符串为空则结果为真       |
| -n str      | 字符串不为空则结果为真     |



算术比较

| 算术比较        | 结果                             |
| --------------- | -------------------------------- |
| expr1 –eq expr2 | 两个表达式相等则结果为真         |
| expr1 –ne expr2 | 两个表达式不等则结果为真         |
| expr1 –gt expr2 | expr1 大于expr2 则结果为真       |
| expr1 –ge expr2 | expr1 大于或等于expr2 则结果为真 |
| expr1 –lt expr2 | expr1 小于expr2 则结果为真       |
| expr1 –le expr2 | expr1 小于或等于expr2 则结果为真 |



与文件有关的条件测试

| 文件条件测试 | 结果                         |
| ------------ | ---------------------------- |
| -e file      | 文件存在则结果为真           |
| -d file      | 文件是一个子目录则结果为真   |
| -f file      | 文件是一个普通文件则结果为真 |
| -s file      | 文件的长度不为零则结果为真   |
| -r file      | 文件可读则结果为真           |
| -w file      | 文件可写则结果为真           |
| -x file      | 文件可执行则结果为真         |



逻辑操作

| 逻辑操作       | 结果                        |
| -------------- | --------------------------- |
| ! expr         | 逻辑表达式求反              |
| expr1 –a expr2 | 两个逻辑表达式“And”（“与”） |
| expr1 –o expr2 | 两个逻辑表达式“Or”（“或”）  |

注意：`-a/-o`和`&&/||`的区别是`-a/-o`是`[]`使用的，而`&&/||`是`[[]]`使用的。





## shell语句

### if语句

形式

```shell
if [ expression ]
then
	statements
elif [ expression ]
then
	statements
elif …
else
	statements
fi
```



### case语句

形式

```shell
case str in
	str1 | str2) statements;;
	str3 | str4) statements;;
	*) statements;;
esac
```



### for语句

* 形式

```shell
for var in list
do
	statements
done
```

* 适用于对一系列字符串循环处理



### while语句

* 形式

```shell
while condition
do
	statements
done
```



### until语句

* 形式

```shell
until condition
do
	statements
done
```

* Not recommended (while statement is preferred)



### select语句

* 形式

```shell
select item in itemlist
do
	statements
done
```

注意作业二



### 函数定义

* 形式

```shell
func()
{
 statements
}
```

* 函数的调用
   func para1 para2 ...



对于shell编程，还是需要去写几个程序才能检验是否掌握！！！



## 代码实战

### 声明/使用数组

```shell
read n
array=() #不需要明确数组长度

for i in `seq 1 $n`
do
	read tmp
	array[$(($i-1))]=$tmp
done

sum=0
for i in $array[@] #遍历数组
do
	sum=$(($sum+$i))
done

echo ${#array[@]}输出数组长度
echo "scale=5; $sum/$n" | bc -l | xargs printf "%.3f" #需要保留几位小数的话，暂时就使用bc和printf

# 补充：还可以使用while循环遍历数组
i=0
while [ $i -lt ${#array[@]} ]
do
	sum=$(($sum+${array[$i]}))
done
```



### if语句

```shell
su="123.txt"
if [ $su = "123" -o $su = "123txt"   ]
then
    echo "123ya"
elif [ $su = "123." -a $su = "123.txt" ]
then
    echo "123.txt"
else
    echo "no"
fi
#答案是no
```





### case语句

```shell
su="421"
while [ true ]
do
    case $su in
        "421") echo $su;su="317";;
        "317") su="graduate";;
        "graduate") break;;
        *) echo "error"; break;;
    esac
done
echo $su;
```

注意：

1. 语句结尾的`;;`一定要有！
2. 一定要有结束符`esac`！
3. statements可以有多条语句！



### select语句

```shell
#!/bin/bash
array=("su"  "421"  "317") #注意数组声明时，中间是没有逗号的！
select su1 in ${array[@]}
do
    case $su1 in
        "su") echo "niude";break;;
        "421") echo "end";break;;
        "317") echo "begin";break;;
        *) echo "error";;
    esac
done
```



### 函数定义

```shell
#!/bin/bash

su(){
    echo "su func start"
    n=$1
    name=$2
    echo "age: $n"
    echo "name: $name"
}

su 18 sutaorui
#结果
su func start
age: 18
name: sutaorui
```

