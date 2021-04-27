#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

static int totallines=0;
static int totalwords=0;
static int totalsize=0;


int wc(char *path);

int main(int argc, char **argv){
	
	if(strcmp(argv[0], "./wc") == 0){
        for(int i=1;i<argc;i++)
	        wc(argv[i]);
        if(totallines>0&&argc>2)
            printf(" %d %d %d %s\n", totallines, totalwords, totalsize, "总用量");
        }
	else
		printf("暂不支持该命令\n");
}


void count(int size,char* path);


int wc(char *path){
	struct stat st;
	
	if(stat(path, &st) < 0){				
		printf("wc: ");
		printf("%s ",path);
        printf( "找不到该文件或目录\n");
		return -1;
	}
	else if((st.st_mode & S_IFMT) == S_IFDIR)
		printf("wc: %s: 是一个目录\n0 0 0 %s\n", path, path);
	else{								
		count(st.st_size,path);
	}				
}

void count(int size,char* path){
	FILE *fp = fopen(path, "r");
	int num1 = 0;
	int num2 = 0;
	if((fp = fopen(path,"rt")) == NULL){
        printf("无法查看文件");
    }
	
	char ch1;
	char ch2;
    ch1 = fgetc(fp);
	while(ch1 != EOF){
		ch2 = fgetc(fp);
        if(ch2=='\n')
            num2++;
		if((ch1 != ' ' && ch1 != '\r' && ch1 != '\f' && ch1 != '\t' && ch1 != '\v' && ch1 != '\n') &&
			(ch2 == ' ' || ch2 == '\r' || ch2 == '\t' || ch2 == '\v' || ch2 == '\f' || ch2 == '\n' || ch2 == EOF))
            num1++;

		ch1 = ch2;
	}
	totalsize+=size;
	totalwords+=num1;
	totallines+=num2;
    printf(" %d %d %d %s\n", num2, num1, size, path);

}
