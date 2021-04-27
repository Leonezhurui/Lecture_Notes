#ifndef LS_H
#define LS_H

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

void file_mode(struct stat* buf);
void file_gid_uid(int uid, int gid);
void file_operation(int *argv);

#endif /* LIST_H */