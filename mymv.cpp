#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
 
#define MAX_LEN 1024
 
int main(int argc, char *argv[])
{
    struct stat fin_stat, finfo_desc;
    char newpath[MAX_LEN];
    if(argc == 1)
    {
        printf("mv: 파일 명령어 누락\n");
    }
    else if(argc ==2)
    {
        printf("mv: \'%s\'다음에 대상 파일 명령이 누락됨", argv[1]);
    }
    else
    {
        if(stat(argv[1], &fin_stat) == -1)//src파일이 존재하는지 체크
        {
            printf("\'%s\'를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다", argv[1]);
            exit(1);
        }
        if(stat(argv[2], &finfo_desc) == -1)//desc파일이 존재하는지 체크
        {
            if(rename(argv[1], argv[2])== 0){}//src파일 rename
            else { printf("작업을 실패하였습니다\n"); exit(1);}
        }
        else//desc파일이 존재한다면
        {
            if(S_ISDIR(finfo_desc.st_mode))//desc가 폴더 :"desc"안으로 "src"가 들어감
            {
                sprintf(newpath,"%s/%s", argv[2], argv[1]);
                //printf("%s", newpath);
                if(link(argv[1], newpath))
                {
                    printf("링크 과정을 실패하였습니다\n");
                    exit(1);
                }
                else
                {
                    if(unlink(argv[1]))
                    {
                        printf("언링크 과정을 실패하였습니다\n");
                        exit(1);
                    }
                }
            }
            else
            {
                if(S_ISDIR(fin_stat.st_mode))
                {
                    printf("디렉터리가 아닌 \'%s\'를 디렉터리 \'%s\'로 덮어 쓸 수 없음",argv[2], argv[1]);
                    exit(1);
                }
                else
                {
                    if(rename(argv[1], argv[2])== 0){}
                    else { printf("작업을 실패하였습니다\n"); exit(1);}
                }
            }
        }
    }
    return 0;
}