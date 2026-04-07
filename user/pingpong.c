#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc,char **argv){
	//两个管道两个方向
	int p1[2],p2[2];
	pipe(p1);
	pipe(p2);
	if(fork()!=0){//父进程
		write(p1[1],".",1);
		close(p1[1]);
		char buf;
		//父进程读管道，读不到就阻塞
		read(p2[0],&buf,1);
		printf("%d:received pong\n",getpid());
		//等待子进程结束
		wait(0);
	}else{
		char buf;
		read(p1[0],&buf,1);
		printf("%d:received ping\n",getpid());
		write(p2[1],",",1);
		close(p2[1]);
	}
	close(p1[0]);
	close(p2[0]);
	exit(0);
}
