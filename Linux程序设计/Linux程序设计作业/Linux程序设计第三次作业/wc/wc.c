#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <stdlib.h>

int totalWord = 0;
int totalLine = 0;
int totalBytes = 0;

int wordCount(char *path);

int lineCount(char *path);

int lineCount(char *path) {
    FILE *fp;
    char ch;
    int num = 0;

    if ((fp = fopen(path, "rt")) == NULL) {
        printf("无法查看文件:%s", path);
        exit(0);
    }

    ch = (char) fgetc(fp);
    while (ch != EOF) {
        if (ch == '\n')
            num++;
        ch = (char) fgetc(fp);
    }
    fclose(fp);
    totalLine+=num;
    return num;
}

int wordCount(char *path) {
    FILE *fp;
    int num = 0;

    if ((fp = fopen(path, "rt")) == NULL) {
        printf("无法查看文件:%s", path);
        exit(0);
    }
    char first_ch;
    char second_ch;
    first_ch = (char)fgetc(fp);
    while (first_ch != EOF) {
        second_ch = (char)fgetc(fp);
        if ((first_ch != ' ' && first_ch != '\r' && first_ch != '\f' && first_ch != '\t' && first_ch != '\v' &&
             first_ch != '\n') &&
            (second_ch == ' ' || second_ch == '\r' || second_ch == '\t' || second_ch == '\v' || second_ch == '\f' ||
             second_ch == '\n' || second_ch == EOF))
            num++;
        first_ch = second_ch;
    }
    fclose(fp);
    totalWord += num;
    return num;
}

int main(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        wc(argv[i]);
    }
    if(argc > 2) {
        printf("  %d\t%d\t%d\t总用量\t\n", totalLine, totalWord, totalBytes);
    }
    return 0;
}

void wc(char *argv) {
    struct stat st;

    if (stat(argv, &st) < 0) {
        printf("wc: 找不到该文件或目录\n");
        exit(0);
    } else if ((st.st_mode & S_IFMT) == S_IFDIR)
        printf("wc: %s: 是一个目录\n0\t0\t0 %s\n", argv, argv);
    else {
        int wordNum = wordCount(argv);
        int lineNum = lineCount(argv);
        printf("  %d\t%d\t%d\t%s\t\n", lineNum, wordNum, (int) st.st_size, argv);
        totalBytes+=(int)st.st_size;
    }
}