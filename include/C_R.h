#ifndef C_R_h
#define C_R_h
#include <string>
#include <iostream>
#include "C_BASE.h";
#include "Torrent.h"
#include "json/json.h"
//string SERVER_IP_ADDRESS="127.0.0.1";
extern string CLIENT_PORT;
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
    C_R_CLIENT()
    {
        user_name="zhouqingyu";
    }
    static const string TORRENT_LIST ;
    string user_name;
    //others implement in the same way
    Json::FastWriter jwriter;
    Json::Reader jreader;

    string generate_request(int type, C_INFO_BASE * c_info_base)
    {
        string retrunstr;
        Json::Value root;
        Json::Value parameter;
        Json::Value parameters;

        if(type==CLIENT_REQUEST_TYPE.UPLOAD_TORRENT_INFO)
        {
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
            parameter["torrent_name"]=torrent->torrent_name;
            parameter["torrent_SHA"]=torrent->torrent_SHA;
            parameter["torrent_size"]=torrent->torrent_size;
            parameter["torrent_port"]=CLIENT_PORT;
            parameters.append(parameter);
            root["request_type"]=type;
            root["parameters"]=parameters;

        }


        retrunstr= jwriter.write(root);
        //   std::cout<<"in the writer"<<retrunstr<<endl;
        add_header_ender(retrunstr);
        //     std::cout<<"in the writer"<<retrunstr<<endl;
        return retrunstr;
    }

    int get_reponse_ack(string instruction)
    {

        int flag=0;
        Json::Value jroot;
        remove_header_ender(instruction);
        if(!jreader.parse(instruction,jroot,false))
        {
            std::cout<<"erro in json parse"<<endl;
            return false;
        }
        int type=jroot["response_type"].asInt();
        cout<<"type is "<<type<<endl;
        if (type==SERVER_RESPONSE_TYPE.UPLOAD_TORRENT_INFO)
        {
            std::cout<<"this is the server upload response"<<std::endl;
            Json::Value parameters=jroot["parameters"];
            std::cout<<"parameters size is "<<parameters.size()<<endl;
            for(int i=0;i<parameters.size();i++)
            {
//
                if(parameters[i].isMember("torrent_name"))
             //   {

<<<<<<< HEAD

                    std::cout<<"file name is"<< parameters[i]["torrent_name"].asString()<<std::endl;
            //    }
            //    else if(parameters[i].isMember("torrent_ack"))
             //   {

                    std::cout<<"ack is"<< (parameters[i]["torrent_ack"]).asInt()<<std::endl;
                    if(parameters[i]["torrent_ack"].asInt()==1)
                        flag=1;

            //    }
            //    else if(parameters[i].isMember("torrent_id"))
           //     {

                    std::cout<<"idis"<< (parameters[i]["torrent_id"]).asInt()<<std::endl;

            //    }
=======
            }
            else if(type == SERVER_RESPONSE_TYPE.REGISTER_IN_PEERLIST)
            {
                T_PEER_LIST *  peer_list= (T_PEER_LIST *)c_info_base;
                parameter["torrent_port"]=peer_list->port;
                for(int i = 0; i < peer_list->info.size(); i++){
                    parameter["user_name"]=peer_list->info.at(i).user_name;
                    parameter["user_id"]=peer_list->info.at(i).user_ip;
                }
                parameters.append(parameter);
                root["reponse_type"]=type;
                root["parameters"]=parameters;
>>>>>>> 4b61685cfdd2e51c5bd421b51a91111d880b8f56
            }

        }

        return flag;

    }

<<<<<<< HEAD
    void add_header_ender(string & inputstring)
    {
        inputstring = STX_FLAG+inputstring.substr(0,inputstring.size()-1)+ETX_FLAG;
    }

};

class C_R_SERVER :public C_R
        //use this to create C_R in client
{
public:
    static const string TORRENT_LIST ;
    //others implement in the same way
=======
            if(type==SERVER_RESPONSE_TYPE.TORRENT_LIST)
            {
                for(int i=0; i<inputvector.size();i++)
                {
                    T_TORRENT  * torrent=(T_TORRENT *) inputvector.at(i);
                    parameter["torrent_name"]=torrent->torrent_name;
                    parameter["torrent_id"]=torrent->torrent_id;
                    parameters.append(parameter);
                    root["reponse_type"]=type;
                    root["parameters"]=parameters;
                }
>>>>>>> 4b61685cfdd2e51c5bd421b51a91111d880b8f56

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
            // string json="{\"response_type\":21,\"parameters\":[{\"torrent_name\":\"raw_avi.torrent\"},{\"torrent_ack\":true},{\"torrent_id\":1}}]}";
            parameter["torrent_name"]=torrent->torrent_name;
            parameter["torrent_id"]=torrent->torrent_id;
            parameter["torrent_ack"]=1;
            parameters.append(parameter);
            root["response_type"]=type;
            root["parameters"]=parameters;

        }


        retrunstr= jwriter.write(root);
        add_header_ender(retrunstr);
        return retrunstr;
    }


    string generate_respose(int type, vector <T_TORRENT *>  inputvector)
    {
        string retrunstr;
        Json::Value root;
        Json::Value parameter;
        Json::Value parameters;

        if(type==SERVER_RESPONSE_TYPE.TORRENT_LIST)
        {
            for(int i=0; i<inputvector.size();i++)
            {
                T_TORRENT  * torrent=(T_TORRENT *) inputvector.at(i);
                parameter["torrent_name"]=torrent->torrent_name;
                parameter["torrent_id"]=torrent->torrent_id;
                parameters.append(parameter);
                root["reponse_type"]=type;
                root["parameters"]=parameters;


            }

        }


        retrunstr= jwriter.write(root);
        add_header_ender(retrunstr);
        return retrunstr;
    }

    void add_header_ender(string & inputstring)
    {
        inputstring = STX_FLAG+inputstring.substr(0,inputstring.size()-1)+ETX_FLAG;
    }


};

#endif
