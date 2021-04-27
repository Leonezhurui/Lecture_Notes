#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <algorithm>
#include <pwd.h>
#include <grp.h>
using namspace std;
bool cmp(string a, string b){
    return a < b;
}

void caseL(DIR* dir);
void caseA(DIR* dir);
void caseR(DIR* dir);
void caseI(DIR* dir);
void caseD(DIR* dir);
void printFileMode(struct stat buff);
void printPermission(struct stat buff) ;
void printUser(struct stat buff);
void printGroup(struct stat buff);

int main(int argc, char* argv[]) {
    DIR *dir;
    dir = opendir(".");
    
    char str[200];
    char* res[];
    struct dirent *entry;
    
    // 分析命令行输入
    int c;
    while((c = getopt( argc , argv , "alidr" )) != -1) {
        switch (c) {
            case 'l':
                caseL(dir);
                break;
            case 'a':
                caseA(dir);
                break;
            case 'i':
                caseI(dir);
                break;
            case 'd':
                caseD(dir);
                break;
            case 'r':
                caseR(dir);
                break;
            default:
                exit(-1);
        }
    }
}

/*
    ls -l
 */
void caseL(DIR* dir) {
    
    struct dirent *entry;
    
    while ((entry = readdir(dir)) != NULL) {
        char s1[1000];
        strcpy(s1, entry->d_name)
        struct stat buff;
        
        // 获得文件信息
        lstat(entry->d_name, &buff);
        printFileMode(buff); // 打印文件类型
        printPermission(buff); // 打印文件权限
        printf(" %3d", buff.st_nlink); // 打印硬连接数
        printUser(buff); // 打印用户名
        printGroup(buff); // 打印所属组
        printf(" %5d", (int)buff.st_size); // 打印文件大小
        printf(" %s", (char *)ctime(buff.st_mtime)); // 打印修改时间
        printf(" %s", entry->d_name); // 打印文件名
        /*如果是链接文件，输出链接地址*/
        if(S_ISLNK(buff.st_mode))
        {
            readlink(s1,s1,1000);
            printf("  -> %s ",s1);
        }
        
        printf("\n"); // 换行
    }
}

void printFileMode(struct stat buff){
    if(S_ISSOCK(buff.st_mode)) printf("s");        /*socket文件*/
    if(S_ISLNK(buff.st_mode))  printf("l");        /*链接文件*/
    if(S_ISFIFO(buff.st_mode)) printf("f");        /*管道文件*/
    if(S_ISREG(buff.st_mode))  printf("-");        /*普通文件*/
    if(S_ISBLK(buff.st_mode))  printf("b");        /*块文件*/
    if(S_ISDIR(buff.st_mode))  printf("d");        /*目录文件*/
    if(S_ISCHR(buff.st_mode))  printf("c");        /*字符文件*/
}

void printPermission(struct stat buff) {
    if(buff.st_mode & S_IRUSR) printf("r");
        else printf("-");                            /*用户读权限*/
    if(buff.st_mode & S_IWUSR) printf("w");
    else printf("-");                            /*用户写权限*/
    if(buff.st_mode & S_IXUSR) printf("x");
    else printf("-");                            /*用户执行权限*/
    if(buff.st_mode & S_IRGRP) printf("r");
    else printf("-");                            /*组用户读权限*/
    if(buff.st_mode & S_IWGRP) printf("w");
    else printf("-");                            /*组用户写权限*/
    if(buff.st_mode & S_IXGRP) printf("x");
    else printf("-");                            /*组用乎执行权限*/
    if(buff.st_mode & S_IROTH) printf("r");
    else printf("-");                            /*其他用户读权限*/
    if(buff.st_mode & S_IWOTH) printf("w");
    else printf("-");                            /*其他用户写权限*/
    if(buff.st_mode & S_IXOTH) printf("x");
    else printf("-");                            /*其他用户执行权限*/
}

void printUser(struct stat buff) {
    struct passwd *pwd = getpwuid(buff.st_uid); // 获得当前用户
    if (pwd) {
        printf(" %s", pwd->pw_name);
    }
}

void printGroup(struct stat buff)
{
    struct group *grp = getgrgid(buff.st_gid);   /*获得组名*/
    if(grp)
        printf(" %s",grp->gr_name);
}


/*
    ls -a
    显示所有文件和目录，包含隐藏目录
 */
void caseA(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        printf(" %s", entry->d_name);
        printf("\n");
    }
}


/*
    ls -r 以文件名反序排列并输出目录内容列表；
    因为需要引入字符串排序，所以使用了cpp的string类型
 */
void caseR(DIR* dir) {
    struct dirent *entry;
    char str[100];
    string res[100];
    int count = 0;
    while ((entry = readdir(dir)) != NULL) {
        strcpy(str, entry->d_name);
        if (str[0] != '.') {
            res[count++] = str;
        }
    }
    
    // 排序并打印
    sort(res, res+count, cmp);
    for (int i=0;i<count;i++) {
        printf("%s", res[i]);
        printf("\n");
    }
}

/*
    ls -i 打印文件索引节点号+文件名
 */
void caseI(DIR* dir) {
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if(entry->d_name[0] != '.') {
            printf("%l", entry->d_ino);
            printf(" %s", entry->d_name);
            printf("\n");
        }
    }
}

/*
    ls -d 仅显示目录名，而不显示目录下的内容列表。显示符号链接文件本身，而不显示其所指向的目录列表
 */
void caseD(DIR* dir) {
    char *dirName = getenv("PWD");
    printf("%s", dirName);
    printf("\n")
}
