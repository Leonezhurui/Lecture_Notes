#include"ls.h"

int main(int argc, char **argv)
{
    int argNum = argc;
    int params[4] = {0,0,0,0};
    int count = 0;
    while (argNum != 0 && count < argc) {
        if(strcmp(argv[count], "-d") == 0)
            params[0] = 1;
        else if(strcmp(argv[count], "-R") == 0)
            params[1] = 1;
        else if(strcmp(argv[count], "-a") == 0)
            params[2] = 1;
        else if(strcmp(argv[count], "-i") == 0)
            params[3] = 1;
        argNum--;
        count++;
    }
    file_operation(params);
    return 0;
}