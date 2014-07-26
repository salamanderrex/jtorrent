#include "client_t.h"
int get_random_number(int begin, int end)
{
	int temp=rand()%(end-begin)+begin;

	//	temp=rand()%(end-begin)+begin;
	
	return temp;
}
int get_rarest_piece(vector <int > request_already_have)
	//return -1, mean fins no one
{
	int id=-1;
	int smallest_size=CLIENT_NUMBER;
	for(int i=0;i<G_pieces.size();i++)
	{
		if(G_pieces[i].size()<smallest_size)// well this is small , but if i already have?
		{
			bool have=false;
			for(int j=0;j<request_already_have.size();j++)
			{
				if(i==request_already_have.at(j))		//aa , i have it
				{
					have=true;;
					break;
				}
			}
			if(!have)
			{
				id=i;
				smallest_size=G_pieces[i].size();
			}

		}
	}

	if(id==-1)
	{
		cout<<"no rareest";
	}
	return id;

}
void vector_remove_value(vector  <int >  &input, int val )
{

   //remove first element with value val
   std::vector<int>::iterator pos;
   pos = find(input.begin(),input.end(),
              val);
   if (pos != input.end()) {
       input.erase(pos);
   }
}

int CLIENT:: try_fetch_piece_N(int piece_id)
{

		//first I ask my friend then I random
		for(int i=0;i<up_friend_slot.size();i++)
		{
			if(client_list[up_friend_slot[i].peer_id].has_specific_piece_N(piece_id))
			{
				cout<<"I am"<<client_id<<"yep , my friend{"<<up_friend_slot[i].peer_id<<"} has ["<<piece_id<<"]"<<endl;
				//insert my self int uploader's table
				client_list[up_friend_slot[i].peer_id].down_table.push_back(myself);
				//set my piece be downing
				set_my_piece_downloading(piece_id);
				return up_friend_slot[i].peer_id;
			}
		}


		//ask GOD who has piece
		//since my friend do not have  i do not need to worry about select them again
		vector <int> targets=G_pieces[piece_id];
		int random_target=-1;
		while(targets.size()>0)
		{
			random_target=get_random_number(0,targets.size());

			if(am_I_hate_him(random_target))
			{
				vector_remove_value(targets,random_target);
				continue;
			}
			if(client_list.at(random_target).up_friend_slot.size()<UP_SLOT_FRIEND_SIZE)
			{
				cout<<"I am"<< client_id<<" : "<<"make friend with {"<<random_target<<"} to get ["<<piece_id<<endl;
				client_list[random_target].down_table.push_back(myself);
				PEER temp(random_target);
				up_friend_slot.push_back(temp);
				//set my piece be downing
				set_my_piece_downloading(piece_id);
				client_list[random_target].up_friend_slot.push_back(myself);
				break;
			}
			else if(client_list.at(random_target).up_passby_slot.size()<UP_SLOT_PASSBY_SIZE)
			{
				cout<<"I am"<< client_id<<" : "<<"make passby with {"<<random_target<<"} to get ["<<piece_id<<endl;
				client_list[random_target].down_table.push_back(myself);
				//set my piece be downing
				set_my_piece_downloading(piece_id);
				client_list[random_target].up_passby_slot.push_back(myself);
				break;
			}

			else
			{
				//this guay is fully loaded the down guys
				vector_remove_value(targets,random_target);
				cout<<"I am"<< client_id<<" : "<<"no body want me"<<endl;
				continue;
			}
		}
		
	//	while(target!=client_id)//well not my self
		//	target=G_pieces[piece_id][get_random_number(0,G_pieces[piece_id].size)]

		return random_target;
	}




	 void CLIENT::kick_my_lazyiest_friend_to_betray()

{
	if(up_friend_slot.size()!=UP_SLOT_FRIEND_SIZE)return ; //no need to dump any one since not full loaded
	
		 refresh_friends_balance_number();
		 sort(up_friend_slot.begin(),up_friend_slot.end());

		 //kick the lazy friend
		 for(int i=0;i<KICK_EACH_TURN;i++)
		 { 
			 PEER temp=up_friend_slot.at(up_friend_slot.size()-1);
			 cout<<"{"<<client_id<<"} "<<"kick out " <<temp.peer_id<<endl; 
			  up_friend_slot.pop_back();
			 //tell the kick outer , let he revenge 

			  client_list[temp.peer_id].betrayer_list.push_back(myself);
			
			//client_list[temp.peer_id].up_friend_slot;
			 std::vector<PEER>::iterator pos=	client_list[temp.peer_id].up_friend_slot.begin();
			 while(pos!=	client_list[temp.peer_id].up_friend_slot.end())
			 {
				 if(pos->peer_id==myself.peer_id)	//delete me from his friend list
				 {
					 client_list[temp.peer_id].up_friend_slot.erase(pos);

					 break;

				 }
				 else
				 pos++;

			 }

			 

		 }

	 }