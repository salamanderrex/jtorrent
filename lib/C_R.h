#ifndef C_R_h
#define C_R_h
#include <string>
#inlcude <stdlib>

class CLIENT_REQUEST_TYPE
{
	static const int TORRENT_LIST	=0x11;
	static const int UPLOAD_FILE_NAME=0x12;
	static const int REGISTER_IN_PEERLIST=0x13;
	static const int UP_LOAD_FILE=0x14;
	
	
};
class SERVER_RESPONSE
{
	static const int ACK=1;
	static const int NAK=0;
};
class C_R
{
	//instruction flag
        static const string STX_FLAG = "$@!*";
        static const string ETX_FLAG =	"*!@$";
	
};

class C_R_CLIENT : C_R
   //use this to create C_R in client
{
        static const string TORRENT_LIST =STX_FLAG+CLIENT_REQUEST_TYPE.TORRENT_LIST+ETX_FLAG;
        //others implement in the same way

 };



#endif
