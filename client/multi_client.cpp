
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

vector <T_TORRENT *> torrents;


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
        myport = CLIENT_PORT;

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
                            string instruction=buf;
                            if(c_r_client.detect_STX(instruction)&c_r_client.detect_ETX(instruction))
                            {
                                c_r_client.remove_header_ender(instruction);
                                std::cout<<"finish receive instruction from client, it is :"<<instruction<<std::endl;
                            }
                            else if(c_r_client.detect_STX(instruction))

                            {

                                memset(buf,0,MAXBUF);
                                while(1)
                                {
                                    if((n = recv(sockfd,buf,MAXBUF,0)) > 0)
                                    {
                                        printf("received data:%s\n from %s\n",buf,inet_ntoa(client[i].addr.sin_addr));
                                        std::cout<<std::endl;
                                    }
                                    instruction.append(buf);
                                    string temp(buf);
                                    if(c_r_client.detect_ETX(temp))
                                    {
                                        memset(buf,0,MAXBUF);
                                        break;
                                    }

                                }
                                std::cout<<"finish receive instruction from client"<<std::endl;
                            }
                            Json::Reader jreader;
                            Json::Value jroot;

                            if(!jreader.parse(instruction,jroot,false))
                            {
                                perror("json reader");
                                exit(-1);
                            }
                            int j_request_type=jroot["request_type"].asInt();

                            std::cout<<jroot["request_type"]<<std::endl;
                            if(j_request_type==CLIENT_REQUEST_TYPE.C_C_REQUEST_SHAKE_HAND)
                            {
                                Json::Value parameters=jroot["parameters"];
                                int i, j;
                                T_TORRENT *torrent;
                                for(i = 0; i < torrents.size(); i++)
                                {
                                    torrent = torrents.at(i);
                                    for(j = 0; j < parameters.size(); j++)
                                    {
                                        if(torrent->torrent_id == parameters[j]["torrent_id"].asInt()) break;
                                    }
                                    if(j != parameters.size()) break;
                                }
                                if(i == torrents.size()) printf("invalid torrent id\n");
                                else
                                {
                                    strcpy(res_buf,(c_r_client.generate_response(CLINET_RESPONSE_TYPE.C_C_REQUEST_SHAKE_HAND_REPLY,torrent)).c_str());
                                    std::cout<<"server response:"<<res_buf<<std::endl;
                                    int  len = send(sockfd, res_buf, strlen(res_buf) , 0);
                                    if (len > 0)
                                        printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                    else {
                                        printf("msg:'%s  failed!\n", res_buf);
                                    }
                                }
                            }

                            else if(j_request_type==CLIENT_REQUEST_TYPE.C_C_REQUEST_BTFIELD)
                            {
                                Json::Value parameters=jroot["parameters"];
                                int i, j;
                                T_TORRENT *torrent;
                                for(i = 0; i < torrents.size(); i++)
                                {
                                    torrent = torrents.at(i);
                                    for(j = 0; j < parameters.size(); j++)
                                    {
                                        if(torrent->torrent_id == parameters[j]["torrent_id"].asInt()) break;
                                    }
                                    if(j != parameters.size()) break;
                                }
                                if(i == torrents.size()) printf("invalid torrent id\n");
                                else
                                {
                                    strcpy(res_buf,(c_r_client.generate_response(CLINET_RESPONSE_TYPE.C_C_RESPONSE_BTFIELD,torrent)).c_str());
                                    std::cout<<"server response:"<<res_buf<<std::endl;
                                    int  len = send(sockfd, res_buf, strlen(res_buf) , 0);
                                    if (len > 0)
                                        printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                    else {
                                        printf("msg:'%s  failed!\n", res_buf);
                                    }
                                }
                            }
                            else if(j_request_type==CLIENT_REQUEST_TYPE.C_C_REQUEST_REQUEST)
                            {
                                    Json::FastWriter jwriter;
                                    jroot["request_type"] = CLINET_RESPONSE_TYPE.C_C_REQUEST_RESPONSE;
                                    string returnstr= jwriter.write(jroot);
                                    returnstr = "$~"+returnstr.substr(0,returnstr.size()-1)+"~$";
                                    strcpy(res_buf,returnstr.c_str());
                                    std::cout<<"server response:"<<res_buf<<std::endl;
                                    int  len = send(sockfd, res_buf, strlen(res_buf) , 0);
                                    if (len > 0)
                                        printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                    else {
                                        printf("msg:'%s  failed!\n", res_buf);
                                    }
                            }
                            else if(j_request_type == CLIENT_REQUEST_TYPE.C_C_REQUESR_REQUEST_1)
                            {
                                Json::Value parameters=jroot["parameters"];
                                int i, j;
                                T_TORRENT *torrent;
                                for(i = 0; i < torrents.size(); i++)
                                {
                                    torrent = torrents.at(i);
                                    for(j = 0; j < parameters.size(); j++) if(torrent->torrent_id == parameters[j]["torrent_id"].asInt()) break;
                                    if(j != parameters.size()) break;
                                }
                                if(i == torrents.size()) printf("invalid torrent id\n");
                                else
                                {
                                    T_TORRENT_PIECE* piece =  torrent->pieces.at(parameters[j]["p_index"].asInt() - 1);
                                    char data_buf[MAXBUF * 4096 + 1];
                                    FILE *fp;
                                    fp = fopen(torrent->file_name.c_str(), "rb");
                                    int file_block_length = 0;
                                    cout <<torrent->file_name<<" "<< piece->order << endl;
                                    for(int i = 0; i < piece->order; i++) file_block_length = fread(data_buf, 1, MAX_DATABUF * 1024, fp);
                                    cout<<file_block_length<<endl;
                                    send(sockfd, data_buf, file_block_length, 0);
                                }
                            }
                            memset(res_buf,NULL,sizeof(res_buf));

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
