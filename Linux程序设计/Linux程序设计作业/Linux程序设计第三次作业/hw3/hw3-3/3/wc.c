
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

int my_wc(char *path);

int main(int argc, char **argv){
	
	if(strcmp(argv[0], "./wc") == 0)
		my_wc(argv[1]);
	else
		printf("暂不支持该命令\n");
}


int wordCount(char* path);
int lineCount(char* path);

int my_wc(char *path){
	struct stat st;
	
	if(stat(path, &st) < 0){				
		printf("wc: 找不到该文件或目录\n");
		return -1;
	}
	else if((st.st_mode & S_IFMT) == S_IFDIR)
		printf("wc: %s: 是一个目录\n0\t0\t0 %s\n", path, path);
	else{								
		int wordNum = wordCount(path);
		int lineNum = lineCount(path);
		printf(" %d %d %d %s\n", lineNum, wordNum, st.st_size, path);
	}				
}

int lineCount(char* path){
	FILE *fp = fopen(path, "r");
	char ch;
	int num = 0;
	 
	if((fp = fopen(path,"rt")) == NULL){
        printf("无法查看文件");
        return -1;
    }
	
    ch = fgetc(fp);
    while(ch != EOF){
        if(ch == '\n')
			num++;
        ch = fgetc(fp);
    }
    fclose(fp);
	return num;
}

int wordCount(char* path){
	FILE *fp = fopen(path, "r");
	int num = 0;
	 
	if((fp = fopen(path,"rt")) == NULL){
        printf("无法查看文件");
        return -1;
    }

	char ch1;
	char second_ch;
	ch1 = fgetc(fp);
	while(ch1 != EOF){
		ch2 = fgetc(fp);
		
		if((ch1 != ' ' && ch1 != '\r' && ch1 != '\f' && ch1 != '\t' && ch1 != '\v' && ch1 != '\n') &&
			(ch2 == ' ' || ch2 == '\r' || ch2 == '\t' || ch2 == '\v' || ch2 == '\f' || ch2 == '\n' || ch2 == EOF))
			num++;
			
		ch1 = ch2;
	}
	
	return num;
}
