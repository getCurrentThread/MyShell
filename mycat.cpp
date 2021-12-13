#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
 
#define NUMBER_OPTION 1
 
int main(int argc, char *argv[])
{
    const int BUFSIZE = 1024;
    char buf[BUFSIZE+1];
    char *filename;
    char *delim = "\r\n";
    char *token;
    int fd, nread;
    int opt, flags;
    int cnt, stline;
    cnt = 0; flags=0; opt = 0; stline= 0;
    memset(buf,0, sizeof(char)*(BUFSIZE+1));
    while((opt = getopt(argc, argv, "n")) != -1)
    {
        //옵션 문자로 n과 t가 있으며 t옵션 뒤에는 인자가 와야 한다.
        switch(opt)
        {
            case 'n': flags |= NUMBER_OPTION;
                break;
            default:
                fprintf(stderr, "Usage: %s [-n] name\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if(argc < 2){
        while((nread = read(0, buf, BUFSIZE)) > 0){
            write(1, buf, nread);
        }
    }
    else{
        for(int i = 1; i < argc; i++)
        {
            if((filename = argv[i])[0] == '-') continue;
            if((fd = open(filename, O_RDONLY))== -1){ //파일을 읽는 것에 실패한 경우
                printf("%s: %s: No such file or directory\n", buf, filename);
            }
            else{
                if(flags & NUMBER_OPTION)
                {
                    memset(buf,0, sizeof(char)*(BUFSIZE+1));
                    while((nread = read(fd,buf,BUFSIZE)) == BUFSIZE)
                    {
                        token = strtok(buf,delim);
                        while(token)
                        {
                            if(stline){printf("%s", token);stline =0;}
                            else{
                                printf("%6d  %s", ++cnt, token);
                            }
                            token = strtok(NULL, delim);
                            if(token){printf("\n");}
                        }
                        stline = 1;
                        memset(buf,0, sizeof(char)*(BUFSIZE+1));
                    }
                    token = strtok(buf,delim);
                    while(token)
                    {
                        if(stline){printf("%s", token);stline =0;}
                        else{
                            printf("%6d  %s", ++cnt, token);
                        }
                        token = strtok(NULL, delim);
                        printf("\n");
                    }
                }
                else
                {
                    while((nread = read(fd,buf,BUFSIZE)) == BUFSIZE)
                    {
                        printf("%s", buf);
                        memset(buf,0, sizeof(char)*(BUFSIZE+1));
                    }
                    printf("%s\n", buf);
                }
            }
 
            close(fd);
        }
    }
    return 0;
}
