#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include "sha1.h"
//usage: ./create_torrent <FILE> 
#define BLOCK 4096*256
#define CODE_LEN 10
using namespace std;

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
//***********************************

    char buffer[BLOCK];
    FILE *upload;
    FILE *temp;
    int part_value=0;
    int size;
    string name, a;
    int len = CODE_LEN;
    a = "temp";
    char b[5];
    const char* schar;
    SHA1* sha1 = new SHA1();
    unsigned char* digest;
    String strtemp;
 	ofstream openfile;
	openfile.open(argv[2]);
    upload = fopen(argv[2], "rb");
//Open the upload file
    if (upload == NULL){
	 printf("open binary file failed\n");
	}
//Cut in parts
    while(!feof(upload)){
        size = fread(buffer,1,BLOCK,upload); 	//Read upload into the buffer
        send(sockfd, size,sizeof(size),0);	//send the size of upload
        if (size == 0) break;
	len = CODE_LEN;
        sha1->addBytes(buffer, size);
        digest = sha1->getDigest();
       // sha1->hexPrinter(digest, 10);
	while( len > 0 )
	{
		openfile << strtemp.Format("%02X ",digest);
		len--;
		digest++;
	}
        cout <<endl;
        fread(buffer,1,4096,fp);
        send(sockfd, buffer,sizeof(buffer),0);
        sprintf(b, "%d", count);
        s = a + b;
        schar = s.c_str();
        temp = fopen(schar,"ab");
        fwrite(buffer,1,ret,temp);
        count ++;
   }
	openfile << ret<<endl;
	openfile << count <<endl;
	
	openfile.close();
    delete sha1;
    free(digest);
}
    //打开二进制文件
    FILE* fp = NULL;
    fp = fopen(argv[2], "rb");
    if (fp == NULL) printf("open binary file failed\n");
    //while loop read binary file
    char buffer[4096];   
    //每次发送1KB数据给服务器
    while(!feof(fp)){
        fread(buffer,1,4096,fp);
        send(sockfd, buffer,sizeof(buffer),0);
    }
    close(sockfd);
}
