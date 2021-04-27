#include"ls.h"

// 获取文件的类型和权限
void file_mode(struct stat *buf) {
    int i;
    char buff[10] = {"----------"};
    switch(buf->st_mode & S_IFMT)//按位&获取文件基本属性
    {
        case S_IFIFO:
            buff[0] = 'f';
            break;
        case S_IFDIR:
            buff[0] = 'd';
            break;
        case S_IFSOCK:
            buff[0] = 's';
            break;
        case S_IFBLK:
            buff[0] = 'b';
            break;
        case S_IFLNK:
            buff[0] = 'l';
            break;
        default:break;
    }

    if(buf->st_mode & S_IRUSR )
    {
        buff[1] = 'r';
    }
    if(buf->st_mode & S_IWUSR )
    {
        buff[2] = 'w';
    }
    if(buf->st_mode & S_IXUSR )
    {
        buff[3] = 'x';
    }
    if(buf->st_mode & S_IRGRP )
    {
        buff[4] = 'r';
    }
    if(buf->st_mode & S_IWGRP )
    {
        buff[5] = 'w';
    }
    if(buf->st_mode & S_IXGRP )
    {
        buff[6] = 'x';
    }
    if(buf->st_mode & S_IROTH )
    {
        buff[7] = 'r';
    }
    if(buf->st_mode & S_IWOTH )
    {
        buff[8] = 'w';
    }
    if(buf->st_mode & S_IXOTH )
    {
        buff[9] = 'x';
    }

    for(i = 0; i < 10; i++)
    {
        printf("%c", buff[i]); //打印类型和权限
    }
}
