# makefile补充学习

> By Leonezhurui, Software Engineering



函数定义与调用

函数定义：

```makefile
define 函数名
	函数题
endef

test：
	$(call 函数名, ...参数)
```



```makefile
.PHONY: test # 表示test是一个伪目标
PWD:=$(shell pwd)
STR:='    su   shabi   shi  '

define fun
        @echo "my name is $0"
        @echo "param 1 is $1"
        @echo $(PWD)
        @echo $(subst /,.,$(PWD))
        @echo $(strip $(STR))
endef

test: 
        $(call fun, su)
```

结果如下：

```shell
my name is fun
param 1 is  su
/home/sh/test_makefile
.home.sh.test_makefile
 su shabi shi 
 
```



strip函数

函数功能：去掉字串（若干单词，使用若干空字符分割）开头和结尾的空字符，并将其中多个连续空字符合并为一个空字符。 (经过测试，发现字串前后的空格并没有被去掉，而是多个空格变成单个的空格)

返回值：==无前导和结尾空字符？==、使用单一空格分割的多单词字符串。 

函数说明：空字符包括空格、[Tab]等不可显示字符。 



```makefile
clean:
        rm -rf $(foreach n, $(shell ls | grep su), $(n))
```

```shell
[root@Leoneserver test_makefile]# make clean
rm -rf  su  su.cpp
[root@Leoneserver test_makefile]# ls
makefile  rui  tao
```

foreach函数

`$(foreach var , list, text)`

函数解释：把**参数list中的单词逐一取出来放到var所指的变量中**，然后**再执行text所包含的表达式**。每一次text会返回一个字符串，循环过程中，text所返回的每一个字符串以空格隔开。最后结束循环时，text所返回的每个字符串所组成的整个字符将会是foreach函数的返回值。



```makefile
try: 
        @echo $(if $FLAG=='TRUE', 'TRUE', 'FLASE')
```



makefile补充学习的全部代码：

```makefile
.PHONY: test
PWD:=$(shell pwd)
STR:='    su   shabi   shi  '
FLAG:='TRUE'

define fun
        @echo "my name is $0"
        @echo "param 1 is $1"
        @echo $(PWD)
        @echo $(subst /,.,$(PWD))
        @echo $(strip $(STR))
endef

test: 
        $(call fun, su)
        
try: 
        @echo $(if $FLAG=='TRUE', 'TRUE', 'FLASE')

clean:
        rm -rf $(foreach n, $(shell ls | grep su), $(n))
```

