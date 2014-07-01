#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "sha1.h"
#include "send.h"
//testgit
//usage: ./create_torrent <FILE> <torrent FILE> <IP>
using namespace std;
int BLOCK = 4096*1024;
void hexPrinter( unsigned char* c, int l, FILE* torrent )
{
	while( l > 0 )
	{
		fprintf( torrent, " %02x", *c );
		l--;
		c++;
	}
	fprintf(torrent, "\n");
}
int main(int argc, char* argv[]){
    char buffer[BLOCK];
    FILE *fp, *temp, *torrent;
    fp = fopen(argv[1], "rb");
    torrent = fopen(argv[2], "w");
    fclose(torrent);
    torrent = fopen(argv[2], "a");
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
    fp = fopen(argv[1], "rb");  
    while(!feof(fp)){
        ret = fread(buffer,1,BLOCK,fp);
        if (ret == 0) break;
        sha1->addBytes(buffer, ret);
        digest = sha1->getDigest();
        hexPrinter(digest,10, torrent);
        //cout << endl;
        //sprintf(b, "%d", count);
        //s = a + b;
        //schar = s.c_str();
        //temp = fopen(schar,"ab");
        //fwrite(buffer,1,ret,temp);
        //count ++;
        size += ret;
    }
    delete sha1;
    free(digest);
    fprintf(torrent, "%s\n", argv[3]);
    fclose(torrent);
    send(argv[2], argv[3]);
}
