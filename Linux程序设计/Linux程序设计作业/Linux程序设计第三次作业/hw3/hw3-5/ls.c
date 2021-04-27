
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>

int file_mode(struct stat* states);//获取文件权限和类型
void file_guid(int uid, int gid);//获取持有者信息
void file_operation(int *argv);//文件操作包装

char* colorset[]={"01;34","01;36","33","01;35","01;33","01;33","01;32"};//强调颜色，颜色来自ls源码（因为我真的算不出色号）

struct pending
{
    int isStart;
    char* name;
    struct stat buf;
    struct pending *next;
};

int main(int argc, char **argv)
{
    int num = argc;
    int params[4] = {0,0,0,0};
    int count = 0;
    while (num != 0 && count < argc) {
        if(strcmp(argv[count], "-d") == 0)
            params[0] = 1;
        else if(strcmp(argv[count], "-R") == 0)
            params[1] = 1;
        else if(strcmp(argv[count], "-a") == 0)
            params[2] = 1;
        else if(strcmp(argv[count], "-i") == 0)
            params[3] = 1;
        num--;
        count++;
    }//用数组对命令进行组合
    file_operation(params);
    return 0;
}


void recursion(const int *argv, char *dirName) {
    DIR *fd; // 定义文件夹类型
    struct dirent *fp;// 文件夹返回值
    struct stat states;
    int totalsize=0;//文件占用块数
    char temp[100]; // 中间变量存放文件路径
    char dirs[100][100]; // 存放子目录用于R指令
    int count = 0; // 子目录计数

    if ((fd = opendir(dirName)) == NULL)//打开文件夹
    {
        perror("open file failed");
        exit(0);
    }

    // -R指令输出文件夹名
    if (argv[1] == 1) {
        printf("%s:\n", dirName);
    }

    /*pending start
    struct pending header;
    header.isStart=1;
    struct pending *h;
    h=&header;
    ending end*/

    while ((fp = readdir(fd)) != NULL) // 循环读取文件夹中的文件信息
    {
        strcpy(temp, dirName);
        strcat(temp, fp->d_name); // 把文件名字添加到路径后面形成完整的路径

        if (stat(temp, &states) == -1) //获取文件基本属性
        {
            perror("get stat failed");
            exit(0);
        }

        // -R指令：记录子文件夹（不包括.和..）
        char buffer = ' ';
        switch ((&states)->st_mode & S_IFMT) {
            case S_IFDIR:
                buffer = 'd';
        }
        if (buffer == 'd') {
            if (strcmp(fp->d_name, ".") != 0 && strcmp(fp->d_name, "..") != 0) {
                // -a
                    if(argv[2]!=1&&strncmp(fp->d_name, ".", 1) == 0){ continue;}
                    else{
                        char completeDName[200];
                        strcpy(completeDName, dirName);
                        strcat(completeDName, fp->d_name);
                        strcat(completeDName, "/");
                        strcpy(dirs[count], completeDName);
                        count++;
                    }
            }
        }

            if (argv[2]!=1&&strncmp(fp->d_name, ".", 1) == 0) {
                continue;
            } else {
                // -i
                /*pending start
                struct pending newPending;
                newPending.isStart=0;
                newPending.name=fp->d_name;
                newPending.states=states;
                while(1){
                    if(h->next==NULL) {
                        h->next = &newPending;
                        h=&newPending;
                        break;
                    }
                    else{
                        if(strcmp(newPending.name,h->next->name)<0){
                            struct pending *tempPending=h->next;
                            h->next=&newPending;
                            newPending.next=tempPending;
                            break;
                        }
                        else{
                            h=h->next;
                        }
                    }
                }
                h=&header;
                pending end*/

                if (argv[3] == 1) {
                    printf("%llu ", (long long int) states.st_ino);
                }
                int color=0;
                if(states.st_size % 4096 != 0){
                    totalsize+= ((int)(states.st_size / 4096)) * 4 + 4;
                }
                else{
                    totalsize+= states.st_size / 1024;
                }
                color=file_mode(&states);
                printf("  %d", (int) states.st_nlink);
                file_guid(states.st_uid, states.st_gid);
                printf("\t%ld", states.st_size);
                printf("\t%.12s ", 4 + ctime(&states.st_mtime));

                if(color==0) {
                    printf("   %s\n", fp->d_name); //打印文件名
                }else{

                    printf("   \e[%sm%s\e[0m\n", colorset[color-1],fp->d_name); //打印带颜色的名
                }
            }
        }


    /*pending start
    printf("now is node link\n");
    while(h->next!=NULL){
        if (argv[3] == 1) {
            printf("%llu ", (long long int) h->next->states.st_ino);
        }
        int color=0;
        color=file_mode(&states);
        printf("  %d", (int) h->next->states.st_nlink);
        file_guid(states.st_uid, h->next->states.st_gid);
        printf("\t%ld", h->next->states.st_size);
        printf("\t%.12s ", 4 + ctime(&(h->next->states).st_mtime));

        if(color==0) {
            printf("   %s\n", h->next->name); //打印文件名
        }else{

            printf("   \e[%sm%s\e[0m\n", colorset[color-1],h->next->name); //打印带颜色的名
        }
    }
    pending end*/
    printf("总用量 %d\n", totalsize); //打印用量
    // -R指令会遍历子文件夹
    if (argv[1] == 1) {
        printf("\n");
        for (int i = 0; i < count; i++) {
            recursion(argv, dirs[i]);
        }
    }
}

void file_operation(int *argv) {
    struct stat states;

    char temp[100];

    // ls -d则只显示当前文件夹
    if (argv[0] == 1) {
        strcpy(temp, "./");
        if (stat(temp, &states) == -1)
        {
            perror("get stat failed");
            exit(0);
        }
        // ls -i显示ino
        if (argv[3] == 1) {
            printf("%llu ", (long long int) states.st_ino);
        }
        file_mode(&states);
        printf("  %d", (int) states.st_nlink);
        file_guid(states.st_uid, states.st_gid);
        printf("\t%ld", states.st_size);
        printf("\t%.12s ", 4 + ctime(&states.st_mtime));
        printf("   \e[%sm%s\e[0m\n", colorset[0],".");
    }
        // ls 非-d处理
    else {
        recursion(argv, "./");
    }
}


int file_mode(struct stat *states) {
    int i;
    int color=0;
    char buffer[10] = {"----------"};
    switch(states->st_mode & S_IFMT)//按位&获取文件基本属性
    {
        case S_IFDIR:
            buffer[0] = 'd';
            color=1;
            break;
        case S_IFLNK:
            buffer[0] = 'l';
            color=2;
            break;
        case S_IFIFO:
            buffer[0] = 'f';
            color=3;
            break;
        case S_IFSOCK:
            buffer[0] = 's';
            color=4;
            break;
        case S_IFBLK:
            buffer[0] = 'b';
            color=5;
            break;
        case S_IFCHR:
            buffer[0] = 'c';
            color=6;
            break;
        default:break;
    }

    if(states->st_mode & S_IRUSR )
    {
        buffer[1] = 'r';
    }
    if(states->st_mode & S_IWUSR )
    {
        buffer[2] = 'w';
    }
    if(states->st_mode & S_IXUSR )
    {
        buffer[3] = 'x';
    }
    if(states->st_mode & S_IRGRP )
    {
        buffer[4] = 'r';
    }
    if(states->st_mode & S_IWGRP )
    {
        buffer[5] = 'w';
    }
    if(states->st_mode & S_IXGRP )
    {
        buffer[6] = 'x';
    }
    if(states->st_mode & S_IROTH )
    {
        buffer[7] = 'r';
    }
    if(states->st_mode & S_IWOTH )
    {
        buffer[8] = 'w';
    }
    if(states->st_mode & S_IXOTH )
    {
        buffer[9] = 'x';
        if(color!=1)
            color=7;
    }

    for(i = 0; i < 10; i++)
    {
        printf("%c", buffer[i]); //打印类型和权限
    }
    return color;//根据文件类型返回颜色号
}
void file_guid(int uid, int gid)
{
    struct passwd *ptr;
    struct group *str;

    ptr = getpwuid((__uid_t)uid);// 文件所有者指针
    str = getgrgid((__uid_t)gid);// 文件所有者组指针
    printf("\t%s\t%s", ptr ->pw_name, str -> gr_name); // 打印文件所有者和文件所有者组
}
