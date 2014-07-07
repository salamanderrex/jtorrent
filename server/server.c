#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
int main(int argc, char* argv[]){
    int listenfd,connfd;
    int n;
    char buff[4096];
    struct sockaddr_in servaddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(6666);
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(listenfd,10);
    printf("listenfd: %d,waiting...\n",listenfd);
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
    /*打开二进制文件*/
    FILE* fp = NULL;
    fp = fopen(argv[1], "ab");
    if (fp == NULL) printf("open binary file failed\n");
    int ret;
    int count = 0;
    
    //while(1){
        n = recv(connfd, buff, 4096,0);	//n是实际复制的字节数
       // if (n == 0) break;
        fwrite(buff,1,n,fp);	//每次写入1个字节，一共写入n个字节
	count++;
    //}
    close(connfd);
    close(listenfd);
	printf("count = %d", count);
}
