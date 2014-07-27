#ifndef CLIENT_CONSOLE_H
#define CLIENT_CONSOLE_H
#include "sha1.h"
#include <fstream>
#include <termios.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <string>
#include <sstream>
#include "Torrent.h"
#include "C_R.h"
#define MAX_DATABUF 4096
char data_buf[MAX_DATABUF + 1];

extern vector <T_TORRENT *> torrents;

extern C_R_CLIENT c_r_client;
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}
int send_instrucation(string instruction);

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int kbhit()
{
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

char  getch()
{
    int r;
    char   c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}

int  getchs(char * input)
{
    int n=0;
    while(1){
        input[n]=getch();   // 读入1个字符
        if (input[n]=='\n')
        {
            //printf("break\n");
            break;  // 如果是换行符，则结束循环
        }
        n++;
    }
    input[n]='\0';
}
char * get_local_ipaddress()
{

    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /* I want to get an IPv4 IP address */
    ifr.ifr_addr.sa_family = AF_INET;

    /* I want IP address attached to "eth0" */
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);

    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    /* display result */
    printf("%s\n", inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));

    return inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
}

void hexPrinter( unsigned char* c, int l, FILE* torrent )
{
    while( l > 0 )
    {
        fprintf( torrent, "%02x ", *c );
        l--;
        c++;
    }
    fprintf(torrent, "\n");
}

void print_maenu()
{

    std::cout<<"====================================="<<endl
            <<"torrent main panel"<<endl
           <<"====================================="<<endl
          << "hello user : "<<c_r_client.user_name<<endl
          <<  "1.create a torrent file"<<endl
           <<  "2.upload the torrent file to the server"<<endl
            << "3.get the torrent list from the server"<<endl
            << "4.get the torrent you want from server"<<endl
            << "5.get the peer list you want from server [ after 4.]"<<endl
            <<  "6.start downloading file for specific torrent file."<<endl
             <<  "7.show the torrent file in local "<<endl
              <<  "8.check the connection status "<<endl
               <<  "9.exit"<<endl
                <<  "========================================"<<endl
                 <<endl;
}
void  *pthread_client_console(void *ptr)
{




    // set_conio_terminal_mode();



    while(1)
    {  int sockfd;
        struct sockaddr_in servaddr;
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        memset(&servaddr, 0, sizeof(servaddr));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port= htons(SERVER_PORT);
        inet_pton(AF_INET, SERVER_IP_ADDRESS.c_str(), &servaddr.sin_addr);

        print_maenu();
        while (!kbhit()) {
            /* do some work */
        }


        char instruction_id=getch();
        getch();
        // std::cout<<instruction_id;
        // printf("get orderid\n");

        if(instruction_id=='c')
            //clean terminal

        {
            system("clear");
            print_maenu();
            continue;
        }
        else   if(instruction_id=='1')
            //create torrent file
        {
            int BLOCK = 4096*1024;
            char buffer[BLOCK];

            char file_name[50];
            char    torrent_file_name [50];
            char * IP_address;


            cout<<"give the file name"<<endl;

            getchs(file_name);
            //   printf("%s\n",file_name);  //输出读入的字符串
            cout<<"give the torrent file name"<<endl;
            getchs(torrent_file_name);



            printf("file is : %s, torrent name is : %s\n",file_name,torrent_file_name);

            IP_address=get_local_ipaddress();
            FILE *fp, *torrent;
            //char file[100], torrent_file[100], IP[100];

            fp = fopen(file_name, "rb");
            torrent = fopen(torrent_file_name, "w");
            fclose(torrent);
            torrent = fopen(torrent_file_name, "a");
            int count = 1;
            int size = 0;
            int ret;
            int length = 10;
            string s, a;
            a = "temp";
            char b[5];
            const char* schar;
            SHA1* sha1 = new SHA1();
            unsigned char* digest;
            if (fp == NULL) printf("open binary file failed\n");
            while(!feof(fp)){
                ret = fread(buffer,1,BLOCK,fp);
                if (ret == 0) break;
                size += ret;
            }


            fprintf(torrent, "%d\n", size);
            fprintf(torrent, "%d\n", size/BLOCK + 1);
            fclose(fp);
            fp = fopen(file_name, "rb");
            while(!feof(fp)){
                ret = fread(buffer,1,BLOCK,fp);
                if (ret == 0) break;
                sha1->addBytes(buffer, ret);
                digest = sha1->getDigest();
                hexPrinter(digest,10, torrent);

                size += ret;
            }
            delete sha1;
            free(digest);
            fprintf(torrent, "%s\n",IP_address);
            fclose(torrent);



            close(sockfd);
        }



        else if (instruction_id=='2')
            //upload the torrent
        {
            int BLOCK = 4096*1024;
            char buffer[BLOCK];


            char    torrent_file_name [50];
            cout<<"give the torrent file name"<<endl;
            getchs(torrent_file_name);


            //get piece_number

            int piece_number=0;
            int file_size=0;

            fstream fin(torrent_file_name);
            string ReadLine;
            //read first line
            if(getline(fin,ReadLine))
            {
               file_size=atoi(ReadLine.c_str());
            }
            if(getline(fin,ReadLine))
            {
                piece_number=atoi(ReadLine.c_str());
            }
            fin.close();

            FILE *fp;



            int count = 1;
            int size = 0;
            int ret;
            int length = 10;
            SHA1* sha1 = new SHA1();
            string SHA_RESULT;
            unsigned char* digest;
            fp = fopen(torrent_file_name, "rb");
            if (fp == NULL) printf("open binary file failed\n");
            stringstream stream;

            while(!feof(fp)){
                ret = fread(buffer,1,BLOCK,fp);
                if (ret == 0) break;
                sha1->addBytes(buffer, ret);
                digest = sha1->getDigest();
                unsigned char* temp_digest=digest;
                int l=10;
                char buf[10];

                while( l > 0 )
                {
                    sprintf( buf, "%02x ", *temp_digest );
                    // std::cout<<buf<<endl;
                    l--;
                    temp_digest++;

                    stream<<buf;
                }
                size=ret+size;

            }
            delete sha1;
            //  free(digest);
            SHA_RESULT=stream.str();
            stream.clear();
            fclose(fp);
            // std::cout<<torrent_file_name<<" SHA is :"<<SHA_RESULT<<endl;


            //initialize the torrent class
            T_TORRENT * uploading_torrent =new T_TORRENT(torrent_file_name,size,c_r_client.user_name,SHA_RESULT,file_size,piece_number);
            //  std::cout<<uploading_torrent->torrent_name<<uploading_torrent->up_loader<<uploading_torrent->torrent_SHA<<endl;
            uploading_torrent->uploading_number = 0;
            string send_requset=c_r_client.generate_request(CLIENT_REQUEST_TYPE.UPLOAD_TORRENT_INFO,uploading_torrent);
            std::cout<<send_requset<<endl<<"to "<<SERVER_IP_ADDRESS<<endl;


            //  send_instrucation(send_requset);
            connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));


            send(sockfd, send_requset.c_str(),strlen(send_requset.c_str()),0);
            printf("sending instruction: \n");
            //wait the server respones
            char recv_data_buf[MAX_DATABUF+1];
            int recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF,0);
            printf("%s\n",recv_data_buf);
            int ack= c_r_client.get_reponse_ack(recv_data_buf);

            if(ack!=1)
            {
                std::cout<<"server decline your uplading torrent"<<endl;
            }
            else
                //server give positive respose
            {
                 int torrent_id_on_server=c_r_client.get_id_on_server(recv_data_buf);
                 uploading_torrent->torrent_id=torrent_id_on_server;
                cout<<"server accept your upload request"<<endl;
                cout<<"now start sending"<<endl;



                fp = fopen(torrent_file_name, "rb");
                bzero(data_buf, MAX_DATABUF);
                int current_order =1;
                int file_block_length = 0;
                while( (file_block_length = fread(data_buf, sizeof(char), MAX_DATABUF, fp)) > 0)
                {
                    for(int i = 0; i < uploading_torrent->piece_number; i++)
                    {
                        T_TORRENT_PIECE  *tem_piece= new T_TORRENT_PIECE;
                        tem_piece->done=1;
                        tem_piece->order=current_order++;
                        if(i == uploading_torrent->piece_number - 1) tem_piece->size = uploading_torrent->file_size - 4096 * 1024 * i;
                        else tem_piece->size = 4096 * 1024;
                        tem_piece->location=torrent_file_name;
                        uploading_torrent->pieces.push_back( tem_piece);
                    }

                    // uploading_torrent->++;
                    printf("file_block_length = %d\n", file_block_length);

                    // 发送buffer中的字符串到new_server_socket,实际上就是发送给客户端
                    if (send(sockfd, data_buf, file_block_length, 0) < 0)
                    {
                        printf("Send File:\t%s Failed!\n", torrent_file_name);
                        break;
                    }

                    bzero(data_buf, sizeof(data_buf));
                }
                fclose(fp);
                printf("File:\t%s Transfer Finished!\n", torrent_file_name);

                printf("adding the torrent into the torrent vector\n");

                torrents.push_back(uploading_torrent);
                uploading_torrent->status=1;




            }
            cout<<"closing socket"<<endl;
            close(sockfd);


        }


        else if(instruction_id=='3')
        {
            string get_torrent_list="$~{\"request_type\":15,\"parameters\":[]}~$";
            connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));


            send(sockfd, get_torrent_list.c_str(),strlen(get_torrent_list.c_str()),0);
            printf("sending instruction :%s \n",get_torrent_list.c_str());

            char recv_data_buf[MAX_DATABUF+1];
            int recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF+1,0);

            printf("%s\n",recv_data_buf);
            string temp=recv_data_buf;
            c_r_client.remove_header_ender(temp);
            cout<<temp<<endl;
            Json::Reader jreader;

            Json::Value jroot;
            std::cout<<temp<<endl;
            if(!jreader.parse(temp,jroot,false))
            {
                std::cout<<"erro in json parse"<<endl;
                continue;
            }

            Json::Value parameters=jroot["parameters"];
            std::cout<<"parameters size is "<<parameters.size()<<endl;
            for(int i=0;i<parameters.size();i++)
            {

                printf("torrent id \ttorrent name\n");

                cout<<"["<<(parameters[i]["torrent_id"]).asInt()<<"]";
                cout<<"         \t"<<parameters[i]["torrent_name"].asString()<<endl;
            }


            cout<<"closing socket"<<endl;
            close(sockfd);
        }

        else if(instruction_id=='4')
        {
            char  choosen_torrent_order[20];
            cout<<"give me the torrent id"<<endl;
            getchs(choosen_torrent_order);
            int down_load_id=atoi(choosen_torrent_order);
            //be lazy here
            string temp="$~{\"request_type\":14,\"parameters\":[{\"torrent_id\":";
            string end="}]}~$";
            string id;
            stringstream ss;
            ss<<down_load_id;
            ss>>id;
            ss.flush();
            string get_torrent=temp+id+end;
            connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
            send(sockfd, get_torrent.c_str(),strlen(get_torrent.c_str()),0);
            cout<<"sending instruction finish "<<get_torrent<<endl;
            char recv_data_buf[MAX_DATABUF+1];
            int recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF+1,0);
            temp=recv_data_buf;
            c_r_client.remove_header_ender(temp);
            cout<<temp<<endl;
            Json::Reader jreader;
            Json::Value jroot;
            std::cout<<temp<<endl;
            if(!jreader.parse(temp,jroot,false))
            {
                std::cout<<"erro in json parse"<<endl;
                continue;
            }
            Json::Value parameters=jroot["parameters"];
            T_TORRENT * torrent = new T_TORRENT;
            for(int i=0;i<parameters.size();i++)
            {
            torrent->torrent_id=parameters[i]["torrent_id"].asInt();
            torrent->torrent_SHA=parameters[i]["torrent_SHA"].asString();
            torrent->torrent_size=parameters[i]["torrent_size"].asInt();
            torrent->torrent_name=parameters[i]["torrent_name"].asString();
            }
            torrent->status = 0;

            get_torrent =  c_r_client.generate_request(CLIENT_REQUEST_TYPE.READY_TO_RECEIVE_TORRENT_FROM_SERVER,torrent);
            send(sockfd, get_torrent.c_str(),strlen(get_torrent.c_str()),0);
            cout<<"sending instruction finish "<<get_torrent<<endl;
            string download_torrent = "./torrents/" + torrent->torrent_name;
            ofstream out_stream;
            out_stream.open(download_torrent.c_str(),ios::binary);
            recv_n = 0;
            int recv_size = 0;
            while(1)
            {
                bzero(recv_data_buf, sizeof(recv_data_buf));
                recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF,0);
                recv_size=recv_n+recv_size;
                if (recv_n == 0) break;
                out_stream<<recv_data_buf;
                out_stream.flush();
                if(recv_size==torrent->torrent_size)
                    break;
            }
            fstream torrent_file_stream(download_torrent.c_str());
            string readline;
            getline(torrent_file_stream,readline);
            torrent->file_size = atoi(readline.c_str());
            getline(torrent_file_stream,readline);
            torrent->piece_number=atoi(readline.c_str());
            cout<<"piece number:"<<torrent->piece_number<<endl;
            for(int i = 0; i < torrent->piece_number; i++)
            {
                T_TORRENT_PIECE* piece = new T_TORRENT_PIECE;
                piece->done = 0;
                piece->location = "./file/temp/";
                piece->order = i+1;
                getline(torrent_file_stream,readline);
                piece->SHA = readline;
                if(i == torrent->piece_number - 1) piece->size = torrent->file_size - 4096 * 1024 * i;
                else piece->size = 4096 * 1024;
                torrent->pieces.push_back(piece);
            }
            for(int i = 0; i < torrent->piece_number;i++)
            {
                T_TORRENT_PIECE * piece = torrent->pieces.at(i);
                cout<<piece->SHA<<"\t"<<piece->size<<endl;
            }
            torrents.push_back(torrent);
            cout<<"closing socket"<<endl;
            close(sockfd);

        }

        else if(instruction_id=='5')
        {

            char  choosen_torrent_order[20];
            cout<<"give me the torrent id"<<endl;
            getchs(choosen_torrent_order);
            int down_load_id=atoi(choosen_torrent_order);
            //be lazy here
            string temp="$~{\"request_type\":13,\"parameters\":[{\"torrent_id\":";
            string end="}]}~$";;
            string id;
            stringstream ss;
            ss<<down_load_id;
            ss>>id;
            ss.flush();

            string get_peer_list=temp+id+end;


            connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));
            send(sockfd, get_peer_list.c_str(),strlen(get_peer_list.c_str()),0);
            cout<<"sending instruction finish "<<get_peer_list<<endl;
            char recv_data_buf[MAX_DATABUF+1];
            int recv_n = recv(sockfd, recv_data_buf, MAX_DATABUF+1,0);

            //   printf("%s\n",recv_data_buf);



            temp=recv_data_buf;
            c_r_client.remove_header_ender(temp);
            //    cout<<temp<<endl;
            Json::Reader jreader;

            Json::Value jroot;
            // std::cout<<temp<<endl;
            if(!jreader.parse(temp,jroot,false))
            {
                std::cout<<"erro in json parse"<<endl;
                continue;
            }



            Json::Value parameters=jroot["parameters"];
            std::cout<<"parameters size is "<<parameters.size()<<endl;
            int torrent_id=jroot["torrent_id"].asInt();

            T_TORRENT *torrent;
            for(int i = 0; i < torrents.size(); i++)
            {
                torrent = torrents.at(i);
                if(torrent->torrent_id == torrent_id) break;
            }

            cout<<"torrent id is "<<torrent_id<<endl;
            for(int i=0;i<parameters.size();i++)
            {
                user_info* user = new user_info;
                T_PEER_LIST* peerlist = new T_PEER_LIST;
                peerlist->torrent_id = torrent_id;
                printf("\n");
                user->port = (parameters[i]["torrent_port"]).asInt();
                cout<<"port is :["<<user->port<<"]";
                user->user_name = parameters[i]["user_name"].asString();
                cout<<" peer name is  "<< user->user_name << " ";
                if(user->user_name==c_r_client.user_name)
                {
                    cout<<"myself"<<endl;
                    delete user;
                    delete peerlist;
                    continue;
                }
                user->user_ip = (parameters[i]["user_ip"]).asString();
                cout<<"ip :" <<user->user_ip<<endl;
                user->uploading_number = 0;
                peerlist->uploader_list.push_back(user);
                torrent->peer_list = peerlist;
            }


            //            parameter["torrent_port"]=peer_list->uploader_list.at(i).port;
            //            parameter["user_name"]=peer_list->uploader_list.at(i).user_name;
            //            parameter["user_ip"]=peer_list->uploader_list.at(i).user_ip;

            cout<<"closing socket"<<endl;
            close(sockfd);
        }


        else if (instruction_id=='6')
        {
            printf("local torrent list\n");
            printf("\t torrent_id \ttorrent_name \t torrent_size \tpiece_number \tstatus \t file_size \n");

            for (int i=0;i<torrents.size();i++)
            {

                cout<< "\t ["<<((T_TORRENT *)torrents.at(i))->torrent_id<< "] \t"<<((T_TORRENT *)torrents.at(i))->torrent_name<< "\t"<<((T_TORRENT *)torrents.at(i))->torrent_size<<"\t  "
                    << "\t  "<<   ((T_TORRENT *)torrents.at(i))->piece_number  <<"\t"<< ((T_TORRENT *)torrents.at(i))->status<<"\t  "
                    <<"\t "<<((T_TORRENT *)torrents.at(i))->file_size <<endl;

                cout<<"<================= pieces done=================>\n";
                for(int j=0;j<((T_TORRENT *)torrents.at(i))->piece_number;j++)
                {
                    cout<<"\t   piece order"<<(T_TORRENT_PIECE *)(((T_TORRENT *)torrents.at(i))->pieces.at(j)) ->order<<"\t  "
                       <<"\t    piece done"<<(T_TORRENT_PIECE *)(((T_TORRENT *)torrents.at(i))->pieces.at(j))->done<<"\t  "
                         <<"\t    piece done"<<(T_TORRENT_PIECE *)(((T_TORRENT *)torrents.at(i))->pieces.at(j))->size<<"\t  "<<endl;
                }
                cout<<endl;
                cout<<endl;
            }


        }
    }

}


#endif // CLIENT_CONSOLE_H
