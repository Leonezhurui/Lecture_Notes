# Linux程序设计复习--第四章

> By Leone, Software Engineering

## 编译链接图

![变异链接图](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/变异链接图.png)

![编译链接图头文件](../rui文档/大三第二学期/学期一/Linux程序设计/Linux程序设计课堂笔记/img/编译链接图头文件.png)

源文件(.c)  --预处理器(preprocess)--> 纯c文件(.c) --编译器(comilation)--> 汇编程序(.s) --汇编器(assembly)--> 目标文件(.o) --链接器(linking)--> 可执行文件





## gcc

常用选项

Gcc指令的一般格式为：`Gcc [选项] 要编译的文件 [选项] [目标文件]`

* -E: 只对源程序进行预处理(调用cpp预处理器)
* -S: 只对源程序进行预处理、编译
* -c: 执行预处理、编译、汇编而不链接
* -o output_file: **指定输出文件名**
* -g: **产生调试工具必需的符号信息**
* -O/On: 在程序编译、链接过程中进行**优化处理** (-O0是不开优化，-O1/2/3优化程度逐渐加强)
* -Wall: 显示所有的警告信息



* `-Idir`: 指定额外的**头文件**搜索路径
* `-Ldir`: 指定额外的**库文件**搜索路径
* `-lname`: 链接时搜索指定的库文件
* `-DMACRO[=DEFN]`: 定义MACRO宏(针对#define)

在不改变源代码的基础上，在文件中添加#define预处理指令。

```shell
gcc -DAA=2
# 就相当于在源码中添加了#define AA 2
```



### gcc代码实战

源文件(try.c)

```C
#include <stdio.h>
int main(){
	printf("The king of 421: su!");
}
```

-E参数命令

```shell
gcc -E try.c
```

注意：**-E 仅对源代码进行预处理(比如替换宏)，这个选项不会产生一个文件，而是直接将结果输出到屏幕。**

但是可以使用重定向，将输出结果保存到文件try.e中。

```shell
gcc -E try.c > try.e
```



-S参数命令

```shell
gcc -S try.c
```

**-S 对源代码进行预处理、编译，不执行汇编、链接工作，有时我们想查看源代码的汇编代码，可以通过-S 选项实现。**这里会产生则会自动生成一个try.s文件，这个文件里是汇编代码。



-c参数命令

```shell
gcc -c try.c
```

**c 对源代码进行预处理、编译、汇编，但不执行链接，产生的是源代码的目标文件(\*.o)**



-o参数命令

```shell
gcc -o try.s try.c
```

注意：这里不会管output_file的名称是什么的，只会生成最后的可执行文件！

```shell
#所以可以直接运行了
./try.s
```



测试一下宏定义：`-DMACRO[=DEFN]`

```C
#include <stdio.h>
int main(){
  printf("%d\n", AA);
	printf("The king of 421: su!\n");
}
```

gcc命令

```shell
gcc -DAA=2 -o try try.c
#相当于#define AA 2
```





## gdb

GDB: GNU Debug

* 设置断点(break)
* 监视变量值(display/print)
* 单步执行(next/step)
* 修改变量值(print x=4)

| gdb命令      | 作用                                                         |
| ------------ | ------------------------------------------------------------ |
| file         | 打开要调试的文件                                             |
| run          | 执行当前调试的程序                                           |
| **list**     | 列出源代码的一部分                                           |
| next         | 执行一条语句但不进入函数内部                                 |
| step         | 执行一条语句，是函数则进入函数内部                           |
| **display**  | **显示表达式的值**                                           |
| print        | 临时显示表达式的值                                           |
| kill         | 中止正在调试的程序                                           |
| quit         | 退出gdb                                                      |
| shell        | 不退出gdb就执行shell命令                                     |
| make         | 不退出gdb就执行make                                          |
| break/tbreak | 设置断点，可以是行号、函数名及地址(以*开头)<br />tbreak：设置临时断点 |



## Makefile

**make & makefile**

* makefile描述模块间的依赖关系；
* make命令根据makefile对程序进行管理和维护；make判断被维护文件的时序关系



**Makefile 规则结构**

* **target ... : prerequisites **

  **...command......**

* target是一个目标文件，可以是Object File，也可以是执行文件

* prerequisites是要生成target所需要的文件或是目标

* command是make需要执行的命令。（可以是任意的Shell命令）



**Makefile执行次序**

1. make会在当前目录下找名字叫“Makefile”或“makefile”的文件。
2. 查找文件中的第一个目标文件（target），举例中的hello
3. 如果**hello文件不存在**，或是**hello所依赖的文件修改时间要比hello新**，就会执行后面所定义的命令来生成hello文件。
4. 如果hello所依赖的.o文件不存在，那么make会在当前文件中找目标为.o文件的依赖性，如果找到则再根据那一个规则生成.o文件。（类似一个堆栈的过程）
5. make根据.o文件的规则生成.o 文件，然后再用.o 文件生成hello文件。



**伪目标**

```makfile
clean:
	rm *.o hello
```

* “伪目标”**并不是一个文件**，**只是一个标签**，所以make无法生成它的依赖关系和决定它是否要执行，只能通过显示地指明这个“目标”才能让其生效
* “伪目标”的**取名不能和文件名重名**
* 为了避免和文件重名的这种情况，可以使用一个特殊的标记**“.PHONY”来显示地指明一个目标是“伪目标”**，向make说明，不管是否有这个文件，这个目标就是“伪目标”
* 伪目标一般没有依赖的文件，但也可以为伪目标指定所依赖的文件。
* 伪目标同样可以作为“默认目标”，只要将其放在第一个。

一般情况下，`make`会默认执行第一个命令；且一般第一个伪目标就是`all`，所以敲不敲`all`都差不多。



**多目标**

* 用处
* **当多个目标同时依赖于一个文件**，并且其生成的命令大体类似，可以使用一个自动化变量**“$@”表示着目前规则中所有的目标的集合**
* 举例

```makefile
bigoutput littleoutput : text.ggenerate text.g -$(subst output,,$@) > $@ 
```

`$@`

`subst output,,$@`：是makefile预定义的一个小函数，subst是子串的意思，将`$@`中的output替换成空`,,`。

上述规则等价于

```makefile
bigoutput : text.g
	generate text.g -big > bigoutput

littleoutput : text.g
	generate text.g -little > littleoutput 
```



**预定义变量**(不需要懂，看看就行)

* `$<`**第一个依赖文件的名称**
* `$?`**所有的依赖文件**，以空格分开，这些依赖文件的**修改日期比目标的创建日期晚**(需要更新的依赖文件)
* `$+`**所有的依赖文件**，以空格分开，并以出现的先后为序，可能包含重复的依赖文件
* `$^`**所有的依赖文件**，以空格分开，**不包含重复的依赖文件**
* `$*`**不包括扩展名的目标文件名称**
* `$@`目标的完整名称
* `$%`如果目标是归档成员(.a或.so)，则该变量表示目标的归档成员名称



**多目标扩展**(较难懂)

* 语法

`<targets ...>: <target-pattern>: <prereq-patterns ...><commands>... `

* 举例

```makefile
objects = foo.o bar.o
all: $(objects)
$(objects): %.o: %.c #注意：这里的%是匹配符
	$(CC) -c $(CFLAGS) $< -o $@
```

* 目标从$object中获取
* “%.o”表明要所有以“.o”结尾的目标，即“foo.o bar.o”，就是变量$object集合的模式
* 依赖模式“%.c”则取模式“%.o”的“%”，也就是“foo bar”，并为其加下“.c”的后缀，于是依赖的目标就是“foo.c bar.c”

上述规则等价于

```makefile
foo.o : foo.c
	$(CC) -c $(CFLAGS) foo.c -o foo.o

bar.o : bar.c
	$(CC) -c $(CFLAGS) bar.c -o bar.o 
```



**使用函数**

* 调用语法
  * `$(<function> <arguments>) `
  * `${<function> <arguments>} `
* 字符串处理函数
  * `$(subst <from>,<to>,<text>)`字符串替换
  * `$(strip <string>)`将一个字符串的开头结尾的空格之类的去除
  * ……
* 文件名操作函数
  * `$(dir <names...>)`
  * `$(basename <names...>)`
  * ……
* **foreach** 函数
  * `$(foreach <var>,<list>,<text>)`
* **if** 函数
  * `$(if <condition>,<then-part>) `
  * `$(if <condition>,<then-part>,<else-part>) `
* **call**函数
  * `$(call <expression>,<parm1>,<parm2>,<parm3>...) `
* ……

```makefile
names = a b c d
files = $(foreach, n, $names, $(n).o)
```

简单的makefile编写

```makefile
reverse = $(2) $(1)
$(call reverse, a, b)
# 则输出的就是ba
```



由于只要求读写简单的makefile，只要将ppt上所有的makefile读懂应该就没问题了。

