
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/types.h>
#include "json/json.h"
#include "../include/C_R.h"
#include "../include/Torrent.h"
#include "client_console.h"
#include <fstream>
#include <pthread.h>
#define MAXBUF 1024


 C_R_CLIENT c_r_client;

typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;
}CLIENT;

 string SERVER_IP_ADDRESS="127.0.0.1";
 int SEREVER_PORT=1234;
 int CLIENT_PORT=1235;
/***************************
**server for multi-client
**PF_SETSIZE=1024
****************************/
int main(int argc, char** argv)
{


    //create the therad
    pthread_t id;
    int ret;
    ret=pthread_create(&id,NULL,pthread_client_console,NULL);
    if(ret!=0){
    printf ("Create pthread error!\n");
    exit (1);
    }



    //following is the upload client
    int i,n,maxi = -1;
    int nready;
    int slisten,sockfd,maxfd=-1,connectfd;

    unsigned int myport,lisnum;

    struct sockaddr_in  my_addr,addr;
    struct timeval tv;

    socklen_t len;
    fd_set rset,allset;

    char buf[MAXBUF + 1];
     char res_buf[MAXBUF + 1];
    CLIENT client[FD_SETSIZE];

    if(argv[1])
        myport = atoi(argv[1]);
    else
        myport = 1235;

    if(argv[2])
        lisnum = atoi(argv[2]);
    else
        lisnum = FD_SETSIZE;

    if((slisten = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        perror("socket");
        std::cout<<std::endl;
        exit(1);
    }

    //set socket NO TIME_WAIT
    int on=1;
    if((setsockopt(slisten,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on)))<0)

    {
        perror("setsocketopt fail");
        std::cout<<std::endl;
        exit(1);
    }
    bzero(&my_addr,sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(slisten, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        perror("bind");
        std::cout<<std::endl;
        exit(1);
    }

    if (listen(slisten, lisnum) == -1) {
        perror("listen");
        std::cout<<std::endl;
        exit(1);
    }

    for(i=0;i<FD_SETSIZE;i++)
    {
        client[i].fd = -1;
    }

    FD_ZERO(&allset);
    FD_SET(slisten, &allset);
    maxfd = slisten;

    printf("Waiting for connections and data...\n");

    while (1)
    {
        rset = allset;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        nready = select(maxfd + 1, &rset, NULL, NULL, &tv);

        if(nready == 0)
            continue;
        else if(nready < 0)
        {
            printf("select failed!\n");
            std::cout<<std::endl;
            break;
        }
        else
        {
            if(FD_ISSET(slisten,&rset)) // new connection
            {
                len = sizeof(struct sockaddr);
                if((connectfd = accept(slisten,
                                       (struct sockaddr*)&addr,&len)) == -1)
                {
                    perror("accept() error\n");
                    std::cout<<std::endl;
                    continue;
                }
                for(i=0;i<FD_SETSIZE;i++)
                {
                    if(client[i].fd < 0)
                    {
                        client[i].fd = connectfd;
                        client[i].addr = addr;
                        printf("Yout got a connection from %s.\n",
                               inet_ntoa(client[i].addr.sin_addr));
                        std::cout<<std::endl;
                        break;
                    }
                }
                if(i == FD_SETSIZE)
                {
                    printf("too many connections");
                    std::cout<<std::endl;
                }

                FD_SET(connectfd,&allset);
                if(connectfd > maxfd)
                    maxfd = connectfd;
                if(i > maxi)
                    maxi = i;
            }
            else
            {
                for(i=0;i<=maxi;i++)
                {
                    if((sockfd = client[i].fd)<0)
                        continue;
                    if(FD_ISSET(sockfd,&rset))
                    {
                        bzero(buf,MAXBUF + 1);
                        if((n = recv(sockfd,buf,MAXBUF,0)) > 0)
                        {
                            printf("received data:%s\n from %s\n",buf,inet_ntoa(client[i].addr.sin_addr));
                            std::cout<<std::endl;



                        }
                        else
                        {
                            printf("disconnected by client!\n");
                            close(sockfd);
                            FD_CLR(sockfd,&allset);
                            client[i].fd = -1;
                        }
                    }
                }
            }
        }
    }
    close(slisten);
}