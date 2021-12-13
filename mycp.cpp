#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#define MAX_LEN 1024
int main(int argc, char *argv[])
{
    struct stat stat_src, dest;
    char buffer[MAX_LEN];
    int fdin, fdout;
    int nread;
    const int LAST_INDEX = argc - 1;
    mode_t src_mode, dest_mode;
    if (argc == 1)
    {
        printf("cp: 파일 명령어 누락\n");
    }
    else if (argc == 2)
    {
        printf("cp: \'%s\'다음에 대상 파일 명령이 누락됨\n", argv[1]);
    }
    else
    {
        if ((fdin = open(argv[1], O_RDONLY)) < 0)
        {
            printf("cp: \'%s\'를 설명할 수 없음: 그런 파일이나 디렉터리가 없습니다\n", argv[1]);
            exit(1);
        }
        if (stat(argv[1], &stat_src) < 0)
        {
            printf("cp: 파일의 정보를 읽어오는 과정을 실패했습니다.\n");
            exit(1);
        }
 
        fdout = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, stat_src.st_mode & 0777);
 
        while ((nread = read(fdin, buffer, MAX_LEN)) > 0)
        {
            if (write(fdout, buffer, nread) < nread)
            {
                close(fdin);
                close(fdout);
                exit(1);
            }
        }
        close(fdin);
        close(fdout);
    }
    return 0;
}
