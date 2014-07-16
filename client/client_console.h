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
struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

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

void  *pthread_client_console(void *ptr)
{



    std::cout<<"====================================="<<endl
            <<"torrent main panel"<<endl
           <<"====================================="<<endl
          << "hello user : username"<<endl
          <<  "1.create a torrent file"<<endl
           <<  "2.upload the torrent file to the server"<<endl
            << "3.get the torrent list from the server"<<endl
            << "4.get the torrent you want from server"<<endl
            <<  "5.start downloading file for specific torrent file."<<endl
             <<  "6.show the torrent file in local "<<endl
              <<  "7.check the connection status "<<endl
               <<  "8.exit"<<endl
                <<  "========================================"<<endl
                 <<endl;

    // set_conio_terminal_mode();



    while(1)
    {
        while (!kbhit()) {
            /* do some work */
        }


        char instruction_id=getch();
        getch();
       // std::cout<<instruction_id;
       // printf("get orderid\n");

        if(instruction_id=='1')
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
        }
    }

}


#endif // CLIENT_CONSOLE_H
