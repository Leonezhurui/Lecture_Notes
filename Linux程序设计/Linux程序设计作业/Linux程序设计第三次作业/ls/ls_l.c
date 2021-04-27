#include "ls.h"

// 获取文件的类型和权限
void file_mode(struct stat *buf) {
    int i;
    char mode[10] = {"----------"};
    switch(buf->st_mode & S_IFMT)//按位&获取文件基本属性
    {
        case S_IFIFO:
            mode[0] = 'f';
            break;
        case S_IFDIR:
            mode[0] = 'd';
            break;
        case S_IFSOCK:
            mode[0] = 's';
            break;
        case S_IFBLK:
            mode[0] = 'b';
            break;
        case S_IFLNK:
            mode[0] = 'l';
            break;
        case S_IFCHR:
            mode[0] = 'c';
            break;
        default:break;
    }

    if(buf->st_mode & S_IRUSR )
    {
        mode[1] = 'r';
    }
    if(buf->st_mode & S_IWUSR )
    {
        mode[2] = 'w';
    }
    if(buf->st_mode & S_IXUSR )
    {
        mode[3] = 'x';
    }
    if(buf->st_mode & S_IRGRP )
    {
        mode[4] = 'r';
    }
    if(buf->st_mode & S_IWGRP )
    {
        mode[5] = 'w';
    }
    if(buf->st_mode & S_IXGRP )
    {
        mode[6] = 'x';
    }
    if(buf->st_mode & S_IROTH )
    {
        mode[7] = 'r';
    }
    if(buf->st_mode & S_IWOTH )
    {
        mode[8] = 'w';
    }
    if(buf->st_mode & S_IXOTH )
    {
        mode[9] = 'x';
    }

    for(i = 0; i < 10; i++)
    {
        printf("%c", mode[i]); //打印类型和权限
    }
}

void file_gid_uid(int uid, int gid)
{
    struct passwd *ptr;
    struct group *str;

    ptr = getpwuid((__uid_t)uid);// 文件所有者指针
    str = getgrgid((__uid_t)gid);// 文件所有者组指针
    printf("\t%s\t%s", ptr ->pw_name, str -> gr_name); // 打印文件所有者和文件所有者组
}


void ls_l(struct stat *buf){
    file_mode(buf);
    printf("  %d", (int) buf->st_nlink);
    file_gid_uid(buf->st_uid, buf->st_gid);
    printf("\t%ld", buf->st_size);
    printf("\t%.12s ", 4 + ctime(&buf->st_mtime));
}