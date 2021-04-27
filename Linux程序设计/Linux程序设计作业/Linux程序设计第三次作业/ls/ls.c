#include "ls.h"

int main(int argc, char **argv)
{
    int params[5] = {0, 0, 0, 0, 0}; //The order of parameters: -l, -d, -R, -a, -i
    int count = 0;
    while (count < argc) {
        if (strcmp(argv[count], "-l") == 0)
        {
            params[0] = 1;
        }
        else if(strcmp(argv[count], "-d") == 0)
            params[1] = 1;
        else if(strcmp(argv[count], "-R") == 0)
            params[2] = 1;
        else if(strcmp(argv[count], "-a") == 0)
            params[3] = 1;
        else if(strcmp(argv[count], "-i") == 0)
            params[4] = 1;
        count++;
    }
    file_operation(params);
    return 0;
}