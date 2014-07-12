#ifndef C_R_h
#define C_R_h
#include <string>
#include <iostream>
#include "C_BASE.h";
#include "Torrent.h"
#include "json/json.h"
using namespace std;

class T_CLIENT_REQUEST_TYPE
{
public :
    enum
    {
    TORRENT_LIST	=15,
    UPLOAD_TORRENT_INFO=12,
    REGISTER_IN_PEERLIST=13,
  //  UP_LOAD_FILE=14
    };
};
class T_SERVER_RESPONSE
{
public :
    enum
    {
    TORRENT_LIST	=51,
    UPLOAD_TORRENT_INFO=21,
    REGISTER_IN_PEERLIST=31,
 //   UP_LOAD_FILE=41
    };
};

extern T_CLIENT_REQUEST_TYPE CLIENT_REQUEST_TYPE;
extern T_SERVER_RESPONSE SERVER_RESPONSE_TYPE;
class C_R
{
public:

        //instruction flag
    static const  string STX_FLAG ;
    static const string ETX_FLAG ;
       // static const string START_CONVERSISON_FLG="@!*";
       // static const string END_CONVERSISION_FLG="*!@";
        bool detect_STX(string &input)
        {
            if(!input.find_first_of(STX_FLAG))
                return true;
            else
                return false;

        }
        bool detect_ETX(string &input)
        {
            if(input.find_last_of(ETX_FLAG))
                return true;
            else
                return false;
        }

       void remove_header(string  &input)
        {
           input=input.substr(input.find_first_of(STX_FLAG)+STX_FLAG.length(),input.length());
        //   std::cout<<input<<endl;

        }

       void remove_ender(string &input)
       {
           input=input.substr(0,input.find_last_of(ETX_FLAG)-1);
       //     std::cout<<input<<endl;
       }

       void remove_header_ender(string &input)
       {
           remove_header(input);
           remove_ender(input);
      //      std::cout<<input<<endl;
       }
	
};

class C_R_CLIENT :public  C_R
   //use this to create C_R in client
{
public:
        static const string TORRENT_LIST ;
        //others implement in the same way

 };

class C_R_SERVER :public C_R
   //use this to create C_R in client
{
public:
        static const string TORRENT_LIST ;
        //others implement in the same way

        Json::FastWriter jwriter;

        string generate_respose(int type, C_INFO_BASE * c_info_base)
        {
            string retrunstr;
            Json::Value root;
            Json::Value parameter;
            Json::Value parameters;

            if(type==SERVER_RESPONSE_TYPE.UPLOAD_TORRENT_INFO)
            {
                T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
               // string json="{\"reponse_type\":21,\"parameters\":[{\"torrent_name\":\"raw_avi.torrent\"},{\"torrent_ack\":true},{\"torrent_id\":1}}]}";
                parameter["torrent_name"]=torrent->torrent_name;
                parameter["torrent_id"]=torrent->torrent_id;
                parameter["ack"]=true;
                parameters.append(parameter);
                root["reponse_type"]=type;
                root["parameters"]=parameters;

            }
          retrunstr= jwriter.write(root);
            add_header_ender(retrunstr);
            return retrunstr;
        }

        void add_header_ender(string & inputstring)
        {
            inputstring = STX_FLAG+inputstring+ETX_FLAG;
        }


 };

#endif
