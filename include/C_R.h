#ifndef C_R_h
#define C_R_h
#include <string>
#include <iostream>
#include "C_BASE.h";
#include "Torrent.h"
#include "json/json.h"
extern string SERVER_IP_ADDRESS;
extern int CLIENT_PORT;
extern int SERVER_PORT;
using namespace std;

class T_CLIENT_REQUEST_TYPE
{
public :
    enum
    {
        TORRENT_LIST	=15,
        UPLOAD_TORRENT_INFO=12,
        REQUEST_PEERLIST=13,
        DOWNLOAD_TORRENT=14,
        READY_TO_RECEIVE_TORRENT_FROM_SERVER=16,
        C_C_REQUEST_SHAKE_HAND=17,
        C_C_REQUEST_BTFIELD=18,
        C_C_REQUEST_REQUEST=19,
        C_C_HAVE=18,
        C_C_REQUEST_REQUEST_1=20
        //  UP_LOAD_FILE=14
    };
};

class T_CLIENT_RESPONSE_TYPE
{
public :
    enum
    {
        C_C_REQUEST_SHAKE_HAND_REPLY=71,
        C_C_RESPONSE_BTFIELD=81,

        C_C_REQUEST_RESPONSE=91
    };
};
class T_SERVER_RESPONSE
{
public :
    enum
    {
        TORRENT_LIST	=51,
        UPLOAD_TORRENT_INFO=21,
        RESPONSE_PEERLIST=31,
        DOWNLOAD_TORRENT=41
        //   UP_LOAD_FILE=41
    };
};

extern T_CLIENT_REQUEST_TYPE CLIENT_REQUEST_TYPE;
extern T_SERVER_RESPONSE SERVER_RESPONSE_TYPE;
extern T_CLIENT_RESPONSE_TYPE CLIENT_RESPONSE_TYPE;
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
    string generate_response(int type, C_INFO_BASE * c_info_base)
    {
        string retrunstr;
        Json::Value root;
        Json::Value parameter;
        Json::Value parameters;
        if(type == CLIENT_RESPONSE_TYPE.C_C_REQUEST_SHAKE_HAND_REPLY)
        {
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
             parameter["torrent_id"]=torrent->torrent_id;
             parameters.append(parameter);
             root["response_type"]=type;
             root["parameters"]=parameters;
        }
        else if(type == CLIENT_RESPONSE_TYPE.C_C_RESPONSE_BTFIELD)
        {
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
            parameter["torrent_id"] = torrent->torrent_id;
            string temp_bitfield;
            for(int i = 0; i < torrent->pieces.size(); i++)
            {
                T_TORRENT_PIECE *piece = torrent->pieces.at(i);
                if(piece->done == 0) temp_bitfield = temp_bitfield + "0";
                else temp_bitfield = temp_bitfield + "1";
            }
            parameter["p"] = temp_bitfield;
            parameter["uploading_number"] = torrent->uploading_number;
            parameters.append(parameter);
            root["response_type"]=type;
            root["parameters"]=parameters;
        }

        retrunstr= jwriter.write(root);
        add_header_ender(retrunstr);
        return retrunstr;
    }
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
            parameter["torrent_uploader"]=user_name;
            parameters.append(parameter);
            root["request_type"]=type;
            root["parameters"]=parameters;

        }
        else if(type==CLIENT_REQUEST_TYPE.READY_TO_RECEIVE_TORRENT_FROM_SERVER)
        {
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
             parameter["torrent_id"]=torrent->torrent_id;
             parameter["torrent_SHA"]=torrent->torrent_SHA;
             parameters.append(parameter);
             root["request_type"]=type;
             root["parameters"]=parameters;
        }
        else if(type == CLIENT_REQUEST_TYPE.C_C_REQUEST_SHAKE_HAND){
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
            parameter["torrent_id"]=torrent->torrent_id;
            parameters.append(parameter);
            root["request_type"]=type;
            root["parameters"]=parameters;
        }
        else if(type == CLIENT_REQUEST_TYPE.C_C_REQUEST_BTFIELD){           
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
            parameter["torrent_id"] = torrent->torrent_id;
            string temp_bitfield;
            for(int i = 0; i < torrent->pieces.size(); i++)
            {
                T_TORRENT_PIECE *piece = torrent->pieces.at(i);
                if(piece->done == 0) temp_bitfield = temp_bitfield + "0";
                else temp_bitfield = temp_bitfield + "1";
            }
            parameter["p"] = temp_bitfield;
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
    //for downloader request begin to downloading
    string generate_request(int type, int p_index, int torrent_id){
        string retrunstr;
        Json::Value root;
        Json::Value parameter;
        Json::Value parameters;
        if (type == CLIENT_REQUEST_TYPE.C_C_REQUEST_REQUEST){
            parameter["torrent_id"]=torrent_id;
            parameter["p_index"]= p_index;
            parameter["begin"] = 0;
            parameter["block_length"] = 4194304;
            parameters.append(parameter);
            root["request_type"]=type;
            root["parameters"] = parameters;
        }
        else if (type == CLIENT_REQUEST_TYPE.C_C_REQUEST_REQUEST_1){
            parameter["torrent_id"]=torrent_id;
            parameter["p_index"]= p_index;
            parameter["begin"] = 0;
            parameter["block_length"] = 4194304;
            parameters.append(parameter);
            root["request_type"]=type;
            root["parameters"] = parameters;
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

            }

        }



        return flag;

    }



    int get_id_on_server(string instruction)
    {

        int id=0;
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

                if(parameters[i].isMember("torrent_name"))


                    std::cout<<"file name is"<< parameters[i]["torrent_name"].asString()<<std::endl;


                std::cout<<"id is"<< (parameters[i]["torrent_id"]).asInt()<<std::endl;
           id=(parameters[i]["torrent_id"]).asInt();



            }

        }



        return id;

    }


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
        else if(type==SERVER_RESPONSE_TYPE.DOWNLOAD_TORRENT)
        {
            T_TORRENT *  torrent= (T_TORRENT *)c_info_base;
            parameter["torrent_id"]=torrent->torrent_id;
            parameter["torrent_SHA"]=torrent->torrent_SHA;
            parameter["torrent_size"]=torrent->torrent_size;
            parameter["torrent_name"]=torrent->torrent_name;
            parameters.append(parameter);
            root["response_type"]=type;
            root["parameters"]=parameters;

        }
        else if(type == SERVER_RESPONSE_TYPE.RESPONSE_PEERLIST)
        {
            T_PEER_LIST *  peer_list= (T_PEER_LIST *)c_info_base;

            for(int i = 0; i < peer_list->uploader_list.size(); i++){
                user_info* user = peer_list->uploader_list.at(i);
                parameter["torrent_port"]=user->port;
                parameter["user_name"]=user->user_name;
                parameter["user_ip"]=user->user_ip;
            }

            parameters.append(parameter);
            root["reponse_type"]=type;
            root["parameters"]=parameters;
            root["torrent_id"]=peer_list->torrent_id;


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
