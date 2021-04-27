#include"ls.h"

void recursion(const int *argv, char *dirName) {
    DIR *fd; // 定义文件夹类型
    struct dirent *fp;// 目录项
    struct stat buf;

    char temp[100]; // 中间变量存放文件路径

    char dirs[100][100]; // 存放子目录用于R指令

    int count = 0; // 子目录计数

    if ((fd = opendir(dirName)) == NULL)//打开文件夹
    {
        perror("open file failed");
        exit(0);
    }

    // -R指令输出文件夹名
    printf("%s:\n", dirName);

    while ((fp = readdir(fd)) != NULL) // 循环读取文件夹中的文件信息
    {
        strcpy(temp, dirName);
        strcat(temp, fp->d_name); // 把文件名字添加到路径后面形成完整的路径

        if (stat(temp, &buf) == -1) //获取文件属性
        {
            perror("get stat failed");
            exit(0);
        }

        // -R：按照参数要求，记录子目录
        char buff = ' ';
        switch ((&buf)->st_mode & S_IFMT) {
            case S_IFDIR:
                buff = 'd';
        }
        if (buff == 'd') {
            if (strcmp(fp->d_name, ".") != 0 && strcmp(fp->d_name, "..") != 0) {
                if (argv[3] == 1) { //记录下除.和..的所有目录名
                    char complete_d_name[200];
                    strcpy(complete_d_name, dirName);
                    strcat(complete_d_name, fp->d_name);
                    strcat(complete_d_name, "/");
                    strcpy(dirs[count], complete_d_name);
                    count++;
                } else {
                    if (strncmp(fp->d_name, ".", 1) != 0) { // 去除掉所有的隐藏文件夹
                        char complete_d_name[200];
                        strcpy(complete_d_name, dirName);
                        strcat(complete_d_name, fp->d_name);
                        strcat(complete_d_name, "/");
                        strcpy(dirs[count], complete_d_name);
                        count++;
                    }
                }
            }
        }

        if(argv[4] == 1){ // 处理-i
            printf("%llu ", (long long int) buf.st_ino);
        }

        // -a则处理隐藏文件
        if (argv[3] == 1) {
            if(argv[0] == 1){
                ls_l(&buf);
                printf("   %s\n", fp->d_name); //打印文件名
            }else{
                printf("%s\n", fp->d_name);
            }
        }
        // 否则略过隐藏文件
        else {
            if (strncmp(fp->d_name, ".", 1) == 0) {
                continue;
            } else {
                if (argv[0] == 1)
                {
                    ls_l(&buf);
                    printf("   %s\n", fp->d_name);
                }else{
                    printf("%s\n", fp->d_name);
                }
            }
        }
    }

    //递归完成子目录ls
    printf("\n");
    for (int i = 0; i < count; i++) {
        recursion(argv, dirs[i]);
    }
}

void ls_lai(const int* argv, char *dirName){
    DIR *fd; // 定义文件夹类型
    struct dirent *fp;// 文件夹返回值
    struct stat buf;

    char temp[100];//存储文件名

    if ((fd = opendir(dirName)) == NULL)//打开文件夹
    {
        perror("open file failed");
        exit(0);
    }

    while ((fp = readdir(fd)) != NULL) // 循环读取文件夹中的文件信息
    {
        strcpy(temp, dirName);
        strcat(temp, fp->d_name); // 把文件名字添加到路径后面形成完整的路径

        if (stat(temp, &buf) == -1) //获取文件属性
        {
            perror("get stat failed");
            exit(0);
        }

        if (argv[3] == 0) {
            // -i则在前面显示ino
            if (strncmp(fp->d_name, ".", 1) == 0) {
                continue;
            }
        }
            
        if (argv[4] == 1){
            printf("%llu ", (long long int) buf.st_ino);
        }

        if(argv[0] == 1){
            ls_l(&buf);
            printf("   %s\n", fp->d_name);
        }else{
            printf("%s\n", fp->d_name);
        }
    }
}


void file_operation(int *argv) {
    struct stat buf;
    char temp[100];

    if (argv[1] == 1) { //如果有-d参数
        strcpy(temp, "./");
        if (stat(temp, &buf) == -1) //获得当前文件夹的文件属性
        {
            perror("get stat failed");
            exit(0);
        }

        if (argv[4] == 1) { //先处理-i选项，在最前面添加上inode
            printf("%llu ", (long long int) buf.st_ino);
        }
        if (argv[3] == 1){} 
        if (argv[2] == 1){} //不处理-a和-R

        if (argv[0] == 1) // 处理-l
        {
            ls_l(&buf);
            printf("   %s\n", "."); 
        }else{
            printf(".\n");
        }
              
    }else if(argv[2] == 0){
        ls_lai(argv, "./");
    }
    else{
        recursion(argv, "./");
    }
}


