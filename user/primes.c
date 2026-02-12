#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int leftpipe[2]){
	int p;
	read(leftpipe[0],&p,sizeof(p));
	if(p == -1){
		exit(1);
	}
	printf("prime %d\n",p);

	int rightpipe[2];
	pipe(rightpipe);
	if(fork()==0){
		close(leftpipe[0]);
		close(rightpipe[1]);
		sieve(rightpipe);
	}else{
		close(rightpipe[0]);
		int buf;
		while(read(leftpipe[0],&buf,sizeof(buf)) && buf != -1){
			if(buf % p != 0){
				write(rightpipe[1],&buf,sizeof(buf));
			}
		}
		buf = -1;
		write(rightpipe[1],&buf,sizeof(buf));
		wait(0);
		exit(0);
	}
}

int main(int argc,char **argv){
	int p[2];
	pipe(p);
	if(fork()==0){
		close(p[1]);
		sieve(p);
		exit(0);
	}else{
		close(p[0]);
		int i;
		for(int i = 2;i<=35;++i){
			write(p[1],&i,sizeof(i));
		}
		i = -1;
		write(p[1],&i,sizeof(i));
	}
	wait(0);
	exit(0);
}

