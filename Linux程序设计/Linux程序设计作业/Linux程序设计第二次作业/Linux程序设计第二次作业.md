# Linux程序设计第二次作业

1. 用命令行打印HOME、PATH、SHLVL、LOGNAME变量的值。 

答：

```shell
echo $HOME
echo $PATH
echo $SHLVL
echo $LOGNAME
```



2. 请用中文解释Shell脚本程序，并说明运行结果： 

```shell
#!/bin/sh 
clear 
select item in Continue Finish 
do 
	case “$item” in 
	Continue) ;; 
	Finish) break ;; 
	*) echo “Wrong choice! Please select again!” ;; 
	esac 
done 
```

答：

让用户在`Continue`和`Finish`中选择，用户只能输入1和2，其中1代表`Continue`，2代表`Finish`；查看用户的输入，如果输入的是1，则直接跳过，让用户继续选择；如果输入的是2，则直接结束select语句；如果输入既不是1也不是2，则在屏幕上打印提示信息`Wrong choice! Please select again!`，并让用户继续选择。



3. 阅读下列Makefile 并用中文说明其含义。 

答：

```makefile
=========Makefile1========= 
export Top:=${shell pwd} # 将当前所在路径赋值给变量Top，并导出变量Top
export Src:=$(Top)/src/  # 将当前路径下src文件夹的路径赋值给Src，并导出变量Src
export Include:=$(Top)/include/ # 将当前路径下include文件夹的路径赋值给Include，并导出变量Include
export Build:=$(Top)/build/ # 将当前路径下build文件夹的路径赋值给Build，并导出变量Build
all: 
	@$(MAKE) -C $(Src) # 切换到文件夹src下，执行make命令(编译链接)
install: 
	@cp $(Build)/test $(Top) # 将build文件夹下的test文件复制到当前目录下
clean: 
	@-rm -rf $(Build) $(Top)/test # 强制删除build文件夹和当前目录下的test文件
	# '-'表示：在删除文件的过程中，如果碰到文件不存在或者已经创建，那么希望忽略掉这个错误，继续执行。
========= Makefile2========= 
all:main.o test4.o  # 指定以来文件main.o和test4.o
	@mkdir -p $(Build) # 递归构建Build变量指代的文件夹
	@mv *.o $(Build) # 将当前目录下所有的.o后缀的文件或文件夹都移到Build变量指代的文件夹内
	$(MAKE) -C $(Src)/dir1 # 切换到Src变量指代的文件夹下的dir1文件夹，执行make命令
	$(MAKE) -C $(Src)/dir2 # 切换到Src变量指代的文件夹下的dir2文件夹，执行make命令
	$(CC) -o $(Build)/test $(Build)/*.o $(Build)/dir1/*.o $(Build)/dir2/*.o # 使用gcc编译器，将Build变量指代的文件夹下的所有.o文件、Build变量指代的文件夹下的dir1文件夹下的所有.o文件和Build变量指代的文件夹下的dir2文件夹下的所有.o文件链接生成可执行文件test，并将test存储在Build变量指代的文件夹下。
main.o : $(Include)/func.h # 需要依赖Include变量指代的文件夹下的func.h头文件
	$(CC) -c main.c -I$(Include) # 使用gcc编译器，并告诉编译器在Include变量指代的文件夹下寻找头文件，编译main.c文件获得main.o。
```

