#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>
int main(int argc, char** argv){
    int sockfd;
    struct sockaddr_in servaddr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port= htons(6666);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    printf("send msg to server: \n");
    connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
    //打开二进制文件
    FILE* fp = NULL;
    fp = fopen(argv[2], "rb");
    if (fp == NULL) printf("open binary file failed\n");
    //while loop read binary file
    char buffer[4096];
    int ret;
    int count = 0;
    
    //每次发送1KB数据给服务器
    while(!feof(fp)){
        fread(buffer,1,4096,fp);
        send(sockfd, buffer,sizeof(buffer),0);
        //count += ret;
        //printf("send\n");
    }
    close(sockfd);
    //fgets(sendline, 4096, stdin);
    //send(sockfd, sendline, strlen(sendline), 0);
    //close(sockfd);
}
