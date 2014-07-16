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
#include <fstream>
#define MAXBUF 1024
#define MAX_DATABUF 4096



typedef struct CLIENT {
    int fd;
    struct sockaddr_in addr;
}CLIENT;


C_R_SERVER c_r_server;
vector <T_TORRENT *> torrents;
/***************************
**server for multi-client
**PF_SETSIZE=1024
****************************/
int main(int argc, char** argv)
{


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
        myport = 1234;

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
    std::cout<<std::endl;
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
                            //detect the start of the instruction
                            string instruction=buf;
                            if(c_r_server.detect_STX(instruction)&c_r_server.detect_ETX(instruction))
                            {
                                c_r_server.remove_header_ender(instruction);
                                std::cout<<"finish receive instruction from client, it is :"<<instruction<<std::endl;
                            }
                            else if(c_r_server.detect_STX(instruction))

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
                                    if(c_r_server.detect_ETX(temp))
                                    {
                                        memset(buf,0,MAXBUF);
                                        break;
                                    }

                                }
                                std::cout<<"finish receive instruction from client"<<std::endl;
                            }



                            //analyze the request instrucion
                            //initial the jsoncpp engine
                            Json::Reader jreader;
                            Json::Value jroot;

                            if(!jreader.parse(instruction,jroot,false))
                            {
                                perror("json reader");
                                exit(-1);
                            }
                            int j_request_type=jroot["request_type"].asInt();

                            std::cout<<jroot["request_type"]<<std::endl;


                            if (j_request_type==CLIENT_REQUEST_TYPE.UPLOAD_TORRENT_INFO)
                                //entering the upload mode
                            {
                                T_TORRENT * up_loading_torrent=new T_TORRENT();
                                T_PEER_LIST *peer_list = new T_PEER_LIST();
                                user_info info;
                                up_loading_torrent->torrent_id=torrents.size()+1;
                                peer_list->torrent_id = torrents.size()+1;
                                info.user_ip = inet_ntoa(client[i].addr.sin_addr);
                                std::cout<<"user want to upload a torrent server"<<std::endl;
                                Json::Value parameters=jroot["parameters"];
                                for(int i=0;i<parameters.size();i++)
                                {
                                    if(parameters[i].isMember("torrent_name"))
                                    {
                                        up_loading_torrent->torrent_name=parameters[i]["torrent_name"].asString();
                                        std::cout<<"file name is"<< parameters[i]["torrent_name"].asString()<<std::endl;
                                    }
                                    else if(parameters[i].isMember("torrent_size"))
                                    {
                                        up_loading_torrent->torrent_size=parameters[i]["torrent_size"].asInt();
                                        std::cout<<"file name is"<< parameters[i]["torrent_size"].asInt()<<std::endl;
                                    }
                                    else if(parameters[i].isMember("torrent_uploader"))
                                    {
                                        info.user_name = parameters[i]["torrent_uploader"].asString();
                                    }
                                    else if(parameters[i].isMember("torrent_port"))
                                    {
                                        peer_list->port = parameters[i]["torrent_port"].asInt();
                                    }
                                }
                                peer_list->info.push_back(info);
                                up_loading_torrent->peer_list = peer_list;
                                torrents.push_back(up_loading_torrent);


                                //send response


                                strcpy(res_buf,(c_r_server.generate_respose(SERVER_RESPONSE_TYPE.UPLOAD_TORRENT_INFO,up_loading_torrent)).c_str());
                                std::cout<<"server response:"<<res_buf<<std::endl;
                                int  len = send(sockfd, res_buf, strlen(res_buf) - 1, 0);
                                if (len > 0)
                                    printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                else {
                                    printf("msg:'%s  failed!\n", res_buf);
                                }

                                //wait for the uping the .torrent file
                                char recv_data_buf[MAX_DATABUF+1];
                                ofstream out_stream;
                                out_stream.open(up_loading_torrent->torrent_name.c_str(),ios::binary);
                                int recv_n;
                                while(1)
                                {
                                    recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF,0);
                                      printf("received data:%s\n from %s\n",recv_data_buf,inet_ntoa(client[i].addr.sin_addr));
                                                     std::cout<<"close out stream"<<std::endl;
                                    if (recv_n == 0) break;
                                     out_stream<<recv_data_buf;
                                     out_stream.flush();
                                }
                                std::cout<<"close out stream"<<std::endl;
                                out_stream.close();
                                  std::cout<<"upload torrent file finished"<<std::endl;

                                /*
          FILE* fp = NULL;
          fp = fopen(jroot["torrent_name"].asString().c_str(), "ab");
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
          */

                            }

                            //ask for torrent list

                            else if(j_request_type==CLIENT_REQUEST_TYPE.TORRENT_LIST)
                            {

                                        //response
                                memset(res_buf,NULL,sizeof(res_buf));
                                strcpy(res_buf,(c_r_server.generate_respose(SERVER_RESPONSE_TYPE.TORRENT_LIST,torrents)).c_str());
                                std::cout<<"server response:"<<res_buf<<std::endl;
                                int  len = send(sockfd, res_buf, strlen(res_buf) - 1, 0);
                                if (len > 0)
                                    printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                else {
                                    printf("msg:'%s  failed!\n", res_buf);
                                }
                            }

                            else if(j_request_type==CLIENT_REQUEST_TYPE.REGISTER_IN_PEERLIST)
                            {
                                Json::Value parameters=jroot["parameters"];
                                int j = 0;
                                for(int i=0;i<parameters.size();i++)
                                {
                                    if(parameters[i].isMember("torrent_id"))
                                    {
                                        j=parameters[i]["torrent_id"].asInt();
                                    }
                                }
                                T_TORRENT  * torrent=(T_TORRENT *) torrents.at(j-1);
                                memset(res_buf,NULL,sizeof(res_buf));
                                strcpy(res_buf,(c_r_server.generate_respose(SERVER_RESPONSE_TYPE.REGISTER_IN_PEERLIST,torrent->peer_list)).c_str());
                                std::cout<<"server response:"<<res_buf<<std::endl;
                                int  len = send(sockfd, res_buf, strlen(res_buf) - 1, 0);
                                if (len > 0)
                                    printf("msg:%s send successful锛宼otalbytes: %d锛乗n", res_buf, len);
                                else {
                                    printf("msg:'%s  failed!\n", res_buf);
                                }
                            }


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
