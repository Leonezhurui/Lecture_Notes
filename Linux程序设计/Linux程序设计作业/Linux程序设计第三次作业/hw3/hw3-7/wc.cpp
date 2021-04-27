#include <sys/stat.h>
#include <sys/types.h>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>

using namespace std;

set<char> stupidChar;

void InitStupidChar() {
    stupidChar.insert(' ');
    stupidChar.insert('\t');
    stupidChar.insert('\r');
    stupidChar.insert('\n');
    stupidChar.insert('\t');
    stupidChar.insert('\f');
    stupidChar.insert('\v');
    stupidChar.insert(EOF);
}

int CountWord(string path) {
    FILE* fp;

    if ((fp = fopen(path.c_str(), "rt")) == NULL) {
        printf("wc: 文件无法打开");
        return 0;
    }

    int num = 0;
    char firstCh;
    char secondCh;
    firstCh = fgetc(fp);
    while (firstCh != EOF) {
        secondCh = fgetc(fp);
        if (!stupidChar.count(firstCh) && stupidChar.count(secondCh))
            num++;

        firstCh = secondCh;
    }
    fclose(fp);
    return num;
}

int CountLine(string path) {
    FILE* fp;

    if ((fp = fopen(path.c_str(), "rt")) == NULL) {
        printf("wc: 文件无法打开");
        return 0;
    }

    int num = 0;
    char ch = fgetc(fp);
    while (ch != EOF) {
        if (ch == '\n')
            num++;
        ch = fgetc(fp);
    }
    fclose(fp);
    return num;
}

int main(int argc, char** argv) {
    InitStupidChar();

    if (argc == 1) {
        puts("wc: 没有输入文件路径");
        return 1;
    }
    string path = argv[1];

    struct stat st;

    if (stat(path.c_str(), &st) < 0) {
        printf("wc: 无此文件或目录\n");
        return -1;
    } else if ((st.st_mode & S_IFMT) == S_IFDIR) {
        printf("wc: %s: 是一个目录\n", path.c_str());
        printf("%8d%8d%8lld %s\n", 0, 0, 0LL, path.c_str());
    } else {
        int wordNum = CountWord(path);
        int lineNum = CountLine(path);
        printf("%8d%8d%8lld %s\n", lineNum, wordNum, st.st_size, path.c_str());
    }
    return 0;
}