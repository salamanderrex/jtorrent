#ifndef TORRENT_H
#define TORRENT_H
#include <string>
#include <iostream>
#include <vector>
#include "C_BASE.h"
#include "Peer_list.h"
#include "C_R.h"
using namespace std;


class T_TORRENT_PIECE
{
public:
    string SHA;
    string location;
    int size;
    int order;
    int done;
};

class T_TORRENT:public C_INFO_BASE{
public:
    T_TORRENT()
    {
 this->torrent_size=0;
    }

    T_TORRENT(int torrent_size,string up_loader,int piece_number)
    {
        this->torrent_size=torrent_size;
        this->up_loader=up_loader;
        this->piece_number=piece_number;
    }

    T_TORRENT(string torrent_name,int torrent_size,string up_loader,string SHA)
    {
        this->torrent_name=torrent_name;
        this->torrent_size=torrent_size;
        this->up_loader=up_loader;
        this->torrent_SHA=SHA;

    }
    T_TORRENT(string torrent_name,int torrent_size,string up_loader,string SHA,int file_size,int piece_number)
    {
        this->torrent_name=torrent_name;
        this->torrent_size=torrent_size;
        this->up_loader=up_loader;
        this->torrent_SHA=SHA;
        this->file_size=file_size;
        this->piece_number=piece_number;

    }
    int torrent_id;
    int torrent_size;
    string torrent_name;
    int file_size;
    string file_name;
    int status;
    string up_loader;
    string torrent_SHA;
    int piece_number;
    vector <T_TORRENT_PIECE *> pieces;
    T_PEER_LIST * peer_list;    //does this need modify when we delete the torrent and its peer list?????
};
extern vector <T_TORRENT *> torrents;
#endif TORRENT_H
