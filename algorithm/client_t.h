#include <iostream>
#include <vector>
#include <algorithm>


using namespace std;
const int UP_SLOT_SIZE=4;			//do not specify this in the vector
const int UP_SLOT_FRIEND_SIZE=3;
const int UP_SLOT_PASSBY_SIZE=UP_SLOT_SIZE-UP_SLOT_FRIEND_SIZE;
const int KICK_EACH_TURN=1;
//const int DOWN_TABLE_SIZE=10;   well down table size seems unnecessay, what if a guy has a infinite download bandwidth
const int PIECES_NUMBER=10;
const int CLIENT_NUMBER=6;

//GOD LOVES PEOPLE, HE KNOWS EVERYTHING

extern vector <vector <int> > G_pieces;
// row is pieces, each has the info of who has it 


int get_random_number(int begin, int end);

void vector_remove_value(vector  <int >  &input, int val );
int get_rarest_piece(vector <int > request_already_have);


class PEER
{
public:
	PEER(int id)
	{
		this->peer_id=id;
		up_number=0;
		down_number=0;
		be_killed=0;
	}
		PEER()
	{
	
		up_number=0;
		down_number=0;
		be_killed=0;
	}
	int peer_id;
	int up_number;
	int down_number;
	//int balance_number;
	int balance_number;
	int be_killed;
	void refresh_balance_number()
	{
		balance_number= down_number-up_number;
		
	}
	 bool   operator <  (const   PEER&   rhs   )  const   
   {   
      return   balance_number   <   rhs.balance_number; 
    }
    bool   operator >  (const   PEER&   rhs   )  const  
   {   
       return   balance_number   >   rhs.balance_number; 
    }


};

class CLIENT
{
	public:
		CLIENT (int client_id)
		{
			this->client_id=client_id;

			for(int i=0;i<PIECES_NUMBER;i++)
			{
				pieces[i]=0;
			}
			myself.peer_id=client_id;
		}
	int client_id;
	PEER myself;
	vector <PEER> up_friend_slot;

	vector <PEER> up_passby_slot;

	vector <PEER> betrayer_list;

	vector <PEER > down_table;

	void kick_the_passbay()
	{
		for (int i=0;up_passby_slot.size()!=0;i++)
		{
			up_passby_slot.pop_back();
			
		}

	}


	void refresh_friends_balance_number()
	{

		for(int i=0;i<up_friend_slot.size();i++)
		{
			up_friend_slot[i].refresh_balance_number();
		}
	}

	 void kick_my_lazyiest_friend_to_betray();
	 
	bool am_I_hate_him(int id)
	{

		for (int i=0;i<betrayer_list.size();i++)
		{
			if (betrayer_list[i].peer_id==id)
				return true;
		}
		return false;
	}
	void period_clean_betrayer_list()
	{
		for(int i=0;betrayer_list.size()!=0;i++)
		{
			cout<<client_id<<" forgive " <<betrayer_list.at(betrayer_list.size()-1).peer_id<<endl;
			betrayer_list.pop_back();
		}
	}
	

	int  pieces [PIECES_NUMBER];
	vector <int > own_what()
	{
		vector <int >temp;
		for (int i=0;i<PIECES_NUMBER;i++)
		{
			if(pieces[i]==1)
			{
				temp.push_back(i);
			}
		}

		return temp;
	}

	bool has_specific_piece_N(int i)
	{
		if (pieces[i]==1)
			return true;
		else 
			return false;
	}

	void set_my_piece_downloading(int i)
	{

		pieces[i]=-1;
	}
	int try_fetch_piece_N(int piece_id);
	
	int go_download_on_piece()
		//return -1 means finish all pieces
	{
		if(own_what().size()==PIECES_NUMBER)	//download all stuff
		{
			return -1;
		}
		int piece_want=get_rarest_piece(own_what());
		cout<<endl<<"going to get ["<<piece_want<<endl;
		return try_fetch_piece_N(piece_want);


	}
	//pieces id start from 0 
	void add_piece(int pieces_id)
	{
		pieces[pieces_id]=1;
		G_pieces[pieces_id].push_back(this->client_id);
	}

	bool done()
	{
		for (int i=0; i <PIECES_NUMBER;i++)
		{
			if(pieces[i]!=1)
			return false;
		}

		return true;
	}

	void print_status()
	{
		cout<<"{"<<client_id<<"} ";
		for(int i=0;i<PIECES_NUMBER;i++)
		{
			if(pieces[i]==1)
			{
			
				cout<<"#";
			}
			else
			{
				cout<<"_";
			}
		}

		cout<<endl;

	}


	void feed_my_downloader()
	{

	}
};

extern vector <CLIENT> client_list;