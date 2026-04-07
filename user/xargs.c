#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void run(char *program,char **args){
    if(fork() == 0){
        exec(program,args);
        exit(0);
    }
    return;
}

int main(int argc,char *argv[]){
    char buf[2048];
    char *p = buf,*lastp = buf;
    char *argsbuf[128];
    char **args = argsbuf;
    //将命令行参数放在argsbuf中
    for(int i = 1;i<argc;++i){
        *args = argv[i];
        args++;
    }
    char **pa = args;

    while(read(0,p,1) != 0){
        if(*p == ' ' || *p == '\n'){
            char *newline = p;
            *p = '\0';
            //将当前参数的起始地址放在argsbuf中
            *(pa++) = lastp;
            lastp = p+1;
            //如果读到换行符，说明当前参数已经结束了，可以执行命令了
            if(*newline == '\n'){
                *pa = 0;
                run(argv[1],argsbuf);
                pa = args;
            }
        }
        p++;
    }
    if(pa != args){
        *p = '\0';
        *(pa++) = lastp;
        *pa = 0;
        run(argv[1],argsbuf);
    }
    while(wait(0) != -1){}
    exit(0);
}