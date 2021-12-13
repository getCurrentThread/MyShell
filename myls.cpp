#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
 
#include <grp.h>
#include <pwd.h>
#include <time.h>
 
#define TRUE 1
#define FALSE 0
 
#define OPTION_ALL          1   //'a'
#define OPTION_INODE        2   //'i'
#define OPTION_SIZE         4   //'s'
#define OPTION_CLASSIFY     8   //'F'
#define OPTION_LONG         16  //'l'
#define OPTION_RECURSIVE    32  //'R'
#define OPTION_REVERSE      64  //'r'
 
#define ADD_OPT(x,y)    ((x)|=(y))
#define IS_OPT(x,y)     ((x)& (y))
 
#ifndef S_ISEXE
#define S_ISEXE(x)      ((x) & (S_IXUSR | S_IXGRP | S_IXOTH ))
#endif
 
#define MAX_LEN 1024
#define MAX_NAMELEN 256
 
void getOption(int argc, char *argv[], int *flags);
void getMode(char *buffer, mode_t mode);
char getFormat(mode_t mode);
void getStringTime(char *buffer, time_t *time);
void getUserName(char *buffer, uid_t uid);
void getGrpName(char *buffer, gid_t gid);
unsigned long getTotal(struct dirent **dentryp, int size, int flags);
void printFileInfo(struct dirent **dentryp, int size, int flags);
void scanDirectory(const char *path, int flags, int depth)
{
    struct dirent **dentryp;
    int i,j, n;
    struct stat finfo;
    char strdir[MAX_LEN];
    
    chdir(path);
    if((n = scandir(".", &dentryp, NULL, alphasort)) <= 0){ printf("ls: \'%s\'에 접근할 수 없습니다: 그런 파일이나 디렉터리가 없습니다.\n", path); return; }//호출이 실패했을 경우 종료.
    if(IS_OPT(flags, OPTION_RECURSIVE)) printf("%s:\n", path);
    if(IS_OPT(flags, OPTION_LONG) || IS_OPT(flags, OPTION_SIZE)) printf("합계 %lu\n", getTotal(dentryp, n, flags));
 
    printFileInfo(dentryp, n, flags);//파일리스트의 정보 출력
 
    if(IS_OPT(flags, OPTION_RECURSIVE)) //R 옵션
    {
        for(i = 0; i < n; i++)
        {
            if(stat(dentryp[i]->d_name, &finfo) < 0) {printf("error occured\n"); exit(1);} 
            if(S_ISDIR(finfo.st_mode) && dentryp[i]->d_name[0] != '.') //디렉터리 인 경우 재귀호출
            {
                strcpy(strdir, path);
                strcat(strdir, "/"); strcat(strdir, dentryp[i]->d_name);// strdir = path + "/" + d_name 
                for(j=0;j<depth; j++)
                {
                    chdir("..");//부모 폴더로 돌아감
                }
                scanDirectory(strdir, flags, depth+1);
                chdir(path);
            }
        }
        for(j=0;j<depth; j++)
        {
            chdir("..");//부모 폴더로 돌아감
        }
    }
    for(i = 0; i < n; i++)//메모리 정리
    {
        free(dentryp[i]);
    }
    free(dentryp);
}
 
int main(int argc, char *argv[])
{
 
    int i, dcnt;
    unsigned int flags = 0;//옵션에 따른 플레그
 
    if(argc == 1)//인자가 없는 경우
    {
        scanDirectory(".", 0, 0);
    }
    else
    {
        getOption(argc, argv, &flags); dcnt =0;
        for(i = 1; i < argc; i++)
        {
            if(argv[i][0] == '-') continue;
            scanDirectory(argv[i], flags, 0);
        }
        if(dcnt == 0) scanDirectory(".", flags, 0);
    }
    return 0;
}
 
void getMode(char *buffer, mode_t mode)
{
    if(     S_ISDIR(mode) ){    buffer[0]='d';}
    else if(S_ISBLK(mode) ){    buffer[0]='b';}
    else if(S_ISCHR(mode) ){    buffer[0]='c';}
    else if(S_ISLNK(mode) ){    buffer[0]='l';}
    else if(S_ISFIFO(mode)){    buffer[0]='p';}
    else if(S_ISSOCK(mode)){    buffer[0]='s';}
    else{buffer[0]='-';}
 
    if(IS_OPT(mode,S_IRUSR)){   buffer[1]='r';} else{buffer[1]='-';}
    if(IS_OPT(mode,S_IWUSR)){   buffer[2]='w';} else{buffer[2]='-';}
    if(IS_OPT(mode,S_IXUSR)){   buffer[3]='x';} else{buffer[3]='-';}
 
    if(IS_OPT(mode,S_IRGRP)){   buffer[4]='r';} else{buffer[4]='-';}
    if(IS_OPT(mode,S_IWGRP)){   buffer[5]='w';} else{buffer[5]='-';}
    if(IS_OPT(mode,S_IXGRP)){   buffer[6]='x';} else{buffer[6]='-';}
 
    if(IS_OPT(mode,S_IROTH)){   buffer[7]='r';} else{buffer[7]='-';}
    if(IS_OPT(mode,S_IWOTH)){   buffer[8]='w';} else{buffer[8]='-';}
    if(IS_OPT(mode,S_IXOTH)){   buffer[9]='x';} else{buffer[9]='-';}
 
    buffer[10]='\0';
 
}
 
char getFormat(mode_t mode)
{
    if(     S_ISDIR(mode) ){    return '/';}
    else if(S_ISLNK(mode) ){    return '@';}
    else if(S_ISFIFO(mode)){    return '|';}
    else if(S_ISSOCK(mode)){    return '=';}
    else if(S_ISEXE(mode) ){    return '*';}
    return (char)0;
}
 
void getOption(int argc, char *argv[], int *flags)
{
    char opt; int total = 0;
    while((opt = getopt(argc, argv, "aisFlRr") ) != -1)//옵션 처리
    {
        switch(opt)
        {
        case 'a': ADD_OPT(total,OPTION_ALL);        break;
        case 'i': ADD_OPT(total,OPTION_INODE);      break;
        case 's': ADD_OPT(total,OPTION_SIZE);       break;
        case 'F': ADD_OPT(total,OPTION_CLASSIFY);   break;
        case 'l': ADD_OPT(total,OPTION_LONG);       break;
        case 'R': ADD_OPT(total,OPTION_RECURSIVE);  break;
        case 'r': ADD_OPT(total,OPTION_REVERSE);    break;
        default:
            printf("ls: 부적절한 옵션 -- \'%c\'\n", opt);
            puts("구현이 되지 않은 옵션일 수 있습니다.");
            exit(1);
        }
        *flags = total;
    }
}
 
void getStringTime(char *buffer, time_t *time)
{
    struct tm *timeptr;
    timeptr = localtime(time);
    strftime(buffer, MAX_LEN, "%b %d %OH:%OM", timeptr);
    if(buffer[4] == '0') buffer[4] = ' ';
}
void getUserName(char *buffer, uid_t uid)
{
    struct passwd *fpwd;
    fpwd = getpwuid(uid);
    strcpy(buffer, fpwd->pw_name);
}
void getGrpName(char *buffer, gid_t gid)
{
    struct group *fpgrp;
    fpgrp = getgrgid(gid);
    strcpy(buffer, fpgrp->gr_name);
}
 
unsigned long getTotal(struct dirent **dentryp, int size, int flags)
{
        unsigned long total = 0;
        int a_opt = IS_OPT(flags, OPTION_ALL);// a 옵션 유무 확인
        char* name; int i; struct stat finfo;
        for(i = 0; i <size; i++)
        {
            name = dentryp[i]->d_name;
            if((!a_opt) && name[0] == '.') continue;
            stat(name, &finfo);
            total += finfo.st_blocks;
        }
        return total/2UL;
}
void printFileInfo(struct dirent **dentryp, int size, int flags)
{
    struct dirent *dentry;
    struct stat finfo;//파일 상태 정보
    char buffer[MAX_LEN];//문자열을 담을 버퍼
 
    int i;
    if(IS_OPT(flags, OPTION_REVERSE))//r 옵션
    {
        for(i = size-1; i >= 0; i--)
        {
            dentry = dentryp[i];
            if((!IS_OPT(flags, OPTION_ALL)) && dentry->d_name[0] == '.') continue;//a 옵션
            stat(dentry->d_name, &finfo);
            if(IS_OPT(flags, OPTION_INODE))//i 옵션
            {
                printf("%8lu ", finfo.st_ino);
            }
            if(IS_OPT(flags, OPTION_SIZE))//s 옵션
            {
                printf("%2ld ", finfo.st_blocks/2UL);
            }
            if(IS_OPT(flags, OPTION_LONG))//l 옵션
            {
                getMode(buffer, finfo.st_mode);//접근권한
                printf("%s ", buffer);
                printf("%lu ", finfo.st_nlink);//하드링크수
                getUserName(buffer,finfo.st_uid);//유저이름
                printf("%s ",buffer);
                getGrpName(buffer, finfo.st_gid);//그룹이름
                printf("%s ", buffer);
                printf("%5ld ", finfo.st_size);//파일크기
                getStringTime(buffer, &finfo.st_mtime);//시간정보
                printf("%s ", buffer);
            }
            printf("%s", dentry->d_name);//파일이름
            if(IS_OPT(flags, OPTION_CLASSIFY))//F 옵션
            {
                printf("%c ", getFormat(finfo.st_mode));
            }
            putchar(' ');
            if(IS_OPT(flags, OPTION_LONG))puts("");//개행
        }            
    }
    else
    {
        for(i = 0; i < size; i++)
        {
            dentry = dentryp[i];
            if((!IS_OPT(flags, OPTION_ALL)) && dentry->d_name[0] == '.') continue;//a 옵션
            stat(dentry->d_name, &finfo);
            if(IS_OPT(flags, OPTION_INODE))//i 옵션
            {
                printf("%8lu ", finfo.st_ino);
            }
            if(IS_OPT(flags, OPTION_SIZE))//s 옵션
            {
                printf("%2ld ", finfo.st_blocks/2UL);
            }
            if(IS_OPT(flags, OPTION_LONG))//l 옵션
            {
                getMode(buffer, finfo.st_mode);//접근권한
                printf("%s ", buffer);
                printf("%lu ", finfo.st_nlink);//하드링크수
                getUserName(buffer,finfo.st_uid);//유저이름
                printf("%s ",buffer);
                getGrpName(buffer, finfo.st_gid);//그룹이름
                printf("%s ", buffer);
                printf("%5ld ", finfo.st_size);//파일크기
                getStringTime(buffer, &finfo.st_mtime);//시간정보
                printf("%s ", buffer);
            }
            printf("%s", dentry->d_name);//파일이름
            if(IS_OPT(flags, OPTION_CLASSIFY))//F 옵션
            {
                printf("%c ", getFormat(finfo.st_mode));
            }
            putchar(' ');
            if(IS_OPT(flags, OPTION_LONG))puts("");//개행
        }
    }
    
    puts("");//개행
}
