#include <dirent.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool arg_l = 0;
bool arg_d = 0;
bool arg_R = 0;
bool arg_a = 0;
bool arg_i = 0;

string arg_path = ".";

// 处理输入的参数
bool DealWithArgs(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-')
            for (int j = 1; argv[i][j] != '\0'; j++)
                switch (argv[i][j]) {
                    case 'l':
                        arg_l = 1;
                        break;
                    case 'd':
                        arg_d = 1;
                        break;
                    case 'R':
                        arg_R = 1;
                        break;
                    case 'a':
                        arg_a = 1;
                        break;
                    case 'i':
                        arg_i = 1;
                        break;
                    default:
                        printf("ls: 不合法的参数\n");
                        return 1;
                }
        else
            arg_path = string(argv[i]);
    }
    return 0;
}

string GetFilename(string path) {
    if (path.find('/') == string::npos)
        return path;
    return path.substr(path.find_last_of('/') + 1);
}

void PrintFileDetail(const char *path, const char *filename, const bool &first) {
    if (!first) puts("");

    struct stat st;
    stat(path, &st);

    // 输出inode
    if (arg_i)
        printf("%8llu ", st.st_ino);

    struct passwd *pw;
    struct group *gr;
    struct tm *tm;

    switch (st.st_mode & S_IFMT) {
        case S_IFREG:
            printf("-");
            break;
        case S_IFDIR:
            printf("d");
            break;
        case S_IFLNK:
            printf("l");
            break;
        case S_IFBLK:
            printf("b");
            break;
        case S_IFCHR:
            printf("c");
            break;
        case S_IFIFO:
            printf("p");
            break;
        case S_IFSOCK:
            printf("s");
            break;
    }

    int i;
    for (i = 8; i >= 0; i--) {
        if (st.st_mode & (1 << i)) {
            switch (i % 3) {
                case 2:
                    printf("r");
                    break;
                case 1:
                    printf("w");
                    break;
                case 0:
                    printf("x");
                    break;
            }
        } else
            printf("-");
    }

    pw = getpwuid(st.st_uid);
    gr = getgrgid(st.st_gid);
    printf("%2d %s %s %4lld", st.st_nlink, pw->pw_name, gr->gr_name, st.st_size);

    tm = localtime(&st.st_ctime);
    printf(" %04d-%02d-%02d %02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min);

    printf(" %s", filename);
}

void PrintFile(const char *path, const char *filename, const ino_t &ino, const bool &first) {
    string pStr = path;
    if (arg_l)
        PrintFileDetail(path, filename, first);
    else {
        if (arg_i)
            printf("%llu %s\t", ino, filename);
        else
            printf("%s\t", filename);
    }
}

void Deal(string path) {
    // 判断文件或目录或不存在
    struct stat st;

    // 不存在
    if (stat(path.c_str(), &st) < 0) {
        printf("ls: 无此文件或目录\n");
        return;
    }

    // 是文件或者-d
    if ((st.st_mode & S_IFMT) != S_IFDIR || arg_d) {
        PrintFile(path.c_str(), GetFilename(path).c_str(), st.st_ino, true);
        return;
    }

    DIR *dir = opendir(path.c_str());

    // 如果是目录
    dirent *d;

    // 记录子目录
    vector<string> subDirs;

    bool first = true;
    while ((d = readdir(dir)) != NULL) {
        // 决定是否排除隐藏文件
        if (!arg_a && d->d_name[0] == '.') continue;

        if (arg_R && (d->d_type & DT_DIR))
            subDirs.push_back(path + "/" + string(d->d_name));

        PrintFile((path + "/" + string(d->d_name)).c_str(), d->d_name, d->d_ino, first);
        first = false;
    }

    // 如果有-R参数
    if (arg_R)
        for (int i = 0; i < subDirs.size(); i++) {
            string subDir = subDirs[i];
            if (GetFilename(subDir) == "." || GetFilename(subDir) == "..") continue;
            puts("");
            puts("");
            printf("%s:\n", subDir.c_str());
            Deal(subDir);
        }
}

int main(int argc, char **argv) {
    if (DealWithArgs(argc, argv))
        return 1;
    Deal(arg_path);
    puts("");
    return 0;
}