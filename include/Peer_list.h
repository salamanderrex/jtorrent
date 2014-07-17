#ifndef PEER_LIST_H
#define PEER_LIST_H
#include <string>
#include <iostream>
#include <vector>
#include "C_BASE.h"
using namespace std;

class user_info
{
public:
    string user_name;
    string user_ip;
     int port;
};


class T_PEER_LIST:public C_INFO_BASE
{
public:
    T_PEER_LIST()
    {

    }

    T_PEER_LIST(int torrent_id)
    {
        this->torrent_id = torrent_id;
    }

    int torrent_id;

    vector <user_info> uploader_list;
};

#endif PEER_LIST_H
