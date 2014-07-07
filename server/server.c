/*
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
    /*
    FILE* fp = NULL;
    fp = fopen(argv[1], "ab");
    if (fp == NULL) printf("open binary file failed\n");
    int ret;
    int count = 0;
    
    while(1){
        //printf("accept begin\n");
        
        //printf("accept ok\n");
        n = recv(connfd, buff, 4096,0);
        if (n == 0) break;
        //printf("ddd\n");
        //printf("recv ok\n");
        //buff[n];
        //printf("connfd: recv msg from client\n");
        fwrite(buff,1,n,fp);
    }
    close(connfd);
    close(listenfd);
}
*/
