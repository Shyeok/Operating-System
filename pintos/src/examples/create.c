#include<stdio.h>
#include<stdlib.h>
#include<syscall.h>


int main(int argc,char **argv){
	printf("%d\n",create(argv[1],atoi(argv[2])));
}
