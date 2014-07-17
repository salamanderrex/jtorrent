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
};


class T_PEER_LIST:public C_INFO_BASE
{
public:
    T_PEER_LIST()
    {

    }

    T_PEER_LIST(int torrent_id, int port)
    {
        this->torrent_id = torrent_id;
        this->port = port;
    }

    int torrent_id;
    int port;
    vector <user_info> info;
};

#endif PEER_LIST_H
