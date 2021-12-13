#include <stdio.h>
#include <unistd.h>
#include <signal.h>
 
void alarm_handler(int i){}
 
void mysleep(int n){
    static struct sigaction newact, oldact;
 
    newact.sa_handler = alarm_handler;
    sigemptyset(&newact.sa_mask);
 
    sigaction(SIGALRM,  &newact, &oldact);//시그널엑션 교체
    alarm(n);
    pause();
 
    sigaction(SIGALRM, &oldact, NULL);//이전 시그널엑션으로 변경
}
 
int main(int argc, char *argv[])
{
    int i = 0;
    while(i < 10){
        printf("Loop %d\n", i++);
        mysleep(1);
    }
    return 0;
}