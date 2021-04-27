//
//  ls.c
//  
//
//  Created by JerryZhang on 2017/3/30.
//
//

#include "ls.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

int ls_l(char *path);
int ls_d(char *path);
int ls_R(char *path);
int ls_a(char *path);
int ls_i(char *path);

//ls -l
int display_FILE(char *path, char *filename);
int display_DIR(char *dirname);

int ls_l(char *path){
    struct stat st;
    
    if(stat(path, &st) < 0){
        printf("ls: 无法访问: 找不到该文件或目录\n");
        return -1;
    }
    else if((st.st_mode & S_IFMT) == S_IFDIR)
        display_DIR(path);
    else
        display_FILE(path, path);
}

int display_FILE(char *path, char *filename){
    struct stat st;
    stat(path, &st);
    
    struct passwd  *pw;
    struct group *gr;
    struct tm *tm;
    
    switch(st.st_mode & S_IFMT){
        case S_IFREG:  printf("-");    break;
        case S_IFDIR:  printf("d");    break;
        case S_IFLNK:  printf("l");    break;
        case S_IFBLK:  printf("b");    break;
        case S_IFCHR:  printf("c");    break;
        case S_IFIFO:  printf("p");    break;
        case S_IFSOCK: printf("s");    break;
    }
    
    int i;
    for(i = 8; i >= 0; i--)
    {
        if(st.st_mode & (1 << i))
        {
            switch(i%3)
            {
                case 2: printf("r"); break;
                case 1: printf("w"); break;
                case 0: printf("x"); break;
            }
        }
        else
            printf("-");
    }
    
    pw = getpwuid(st.st_uid);
    gr = getgrgid(st.st_gid);
    printf("%2d %s %s %4ld", st.st_nlink, pw->pw_name, gr->gr_name, st.st_size);
    
    tm = localtime(&st.st_ctime);
    printf(" %04d-%02d-%02d %02d:%02d",tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);
    
    printf(" %s\n", filename);
}

int display_DIR(char *dirname){
    DIR *dir;
    dir = opendir(dirname);
    struct dirent *dirent;
    
    char buf[1024];
    struct stat st;
    while((dirent = readdir(dir)) != NULL){
        strcpy(buf, dirname);
        strcat(buf, "/");
        strcat(buf, dirent->d_name);
        
        if(stat(buf, &st) < 0){
            printf("ls: 无法访问: 找不到该文件或目录\n");
            return -1;
        }
        
        if(dirent->d_name[0] != '.')
            display_FILE(buf, dirent->d_name);
    }
}

//ls -d
int ls_d(char *path){
    struct stat st;
    
    if(stat(path, &st) < 0){
        printf("ls: 无法访问: 找不到该文件或目录\n");
        return -1;
    }
    else
        printf("%s\n", path);
}

//ls -R
int display_Subfile(char* path);

int ls_R(char *path){
    struct stat st;
    
    if(stat(path, &st) < 0){
        printf("ls: 无法访问: 找不到该文件或目录\n");
        return -1;
    }
    else if((st.st_mode & S_IFMT) == S_IFDIR)
        display_Subfile(path);
    else
        printf("%s\n", path);
}

int display_Subfile(char* path){
    
    printf("%s:\n", path);
    
    DIR *dir;
    dir = opendir(path);
    struct dirent *dirent;
    
    char subPath[1024][1024];
    int num = 0;
    
    char buf[1024];
    struct stat st;
    while((dirent = readdir(dir)) != NULL){
        strcpy(buf, path);
        strcat(buf, "/");
        strcat(buf, dirent->d_name);
        stat(buf, &st);
        
        if(dirent->d_name[0] != '.'){
            printf("%s\t", dirent->d_name);
            if((st.st_mode & S_IFMT) == S_IFDIR){
                strcpy(subPath[num], buf);
                num++;
            }
        }
    }
    printf("\n");
    
    num--;
    for( ;num >= 0; num--){
        display_Subfile(subPath[num]);
    }
}

//ls -a
int display_All(char* path);

int ls_a(char *path){
    struct stat st;
    
    if(stat(path, &st) < 0){
        printf("ls: 无法访问: 找不到该文件或目录\n");
        return -1;
    }
    else if((st.st_mode & S_IFMT) == S_IFDIR)
        display_All(path);
    else
        printf("%s\n", path);
}

int display_All(char* path){
    DIR *dir;
    dir = opendir(path);
    struct dirent *dirent;
    
    while((dirent = readdir(dir)) != NULL){
        printf("%s\t", dirent->d_name);
    }
    printf("\n");
}


//ls -i
int display_AllInode(char* path);

int ls_i(char *path){
    struct stat st;
    
    if(stat(path, &st) < 0){				
        printf("ls: 无法访问: 找不到该文件或目录\n");
        return -1;
    }
    else if((st.st_mode & S_IFMT) == S_IFDIR)	
        display_AllInode(path);
    else 									
        printf("%9ld %s\n", st.st_ino, path);
}

int display_AllInode(char* path){
    DIR *dir;
    dir = opendir(path);
    struct dirent *dirent;
    
    char buf[1024];
    struct stat st;
    while((dirent = readdir(dir)) != NULL){		
        strcpy(buf, path);
        strcat(buf, "/");
        strcat(buf, dirent->d_name);
        stat(buf, &st);
        
        if(dirent->d_name[0] != '.')
            printf("%9ld %s\t", st.st_ino, dirent->d_name);
    }	
    printf("\n");
}

//主函数入口
int main(int argc, char **argv){
    
    if(strcmp(argv[0], "./ls") == 0){
        if(strcmp(argv[1], "-l") == 0)
            ls_l(argv[2]);
        else if(strcmp(argv[1], "-d") == 0)
            ls_d(argv[2]);
        else if(strcmp(argv[1], "-R") == 0)
            ls_R(argv[2]);
        else if(strcmp(argv[1], "-a") == 0)
            ls_a(argv[2]);
        else if(strcmp(argv[1], "-i") == 0)
            ls_i(argv[2]);
    }
    else
        printf("暂不支持该命令\n");
}
