
#include"ls.h"

void file_gid_uid(int uid, int gid)
{
    struct passwd *ptr;
    struct group *str;

    ptr = getpwuid((__uid_t)uid);// 文件所有者指针
    str = getgrgid((__uid_t)gid);// 文件所有者组指针
    printf("\t%s\t%s", ptr ->pw_name, str -> gr_name); // 打印文件所有者和文件所有者组
}
