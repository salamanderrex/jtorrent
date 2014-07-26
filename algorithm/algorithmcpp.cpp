===================================================================================
==========		this file was created by jtorrent team1 	===========
==========		copyright belongs to UMJI_VE489_2014_TEAM1	===========
==========		Last modified date: 2014_7_26			===========
===================================================================================



#include <iostream>
#include <vector>
#include "client_t.h"
using namespace std;

unsigned int random_seed=200;

vector <vector <int> > G_pieces;
//client list 
vector <CLIENT> client_list;


void print_G_PIECES()
{


	for(int i=0;i<G_pieces.size();i++)
	{
		cout<<" ["<<i<<"] \t =>";
		for (int j=0;j<G_pieces[i].size();j++)
		{
			cout<<" "<<G_pieces[i][j]<<"\t";
		}
		cout<<endl;
	}
}



CLIENT & get_CLIENT_N(int client_id )
{
	return client_list.at(client_id);
}

bool everyone_done()
{
	for(int i=0;i<client_list.size();i++)
	{
		if( ! client_list.at(i).done())
			return false;
	}

	return true;
}

void print_all_client_status()
{
	for (int client_id=0;client_id<CLIENT_NUMBER;client_id++)
	{

		client_list.at(client_id).print_status();
	}
}


void get_action_queue(int * arr)
{
	
	for(int i=0;i<CLIENT_NUMBER;i++)
	{
		arr[i] = i;           //给每个单元顺序赋值
	}
	
	int N = CLIENT_NUMBER*2;              //假设重复对调 200 次
	for(int i=0;i<N;i++)
	{
		int a = get_random_number(0,CLIENT_NUMBER);//随机选取两个单元下标
		int b = get_random_number(0,CLIENT_NUMBER);//区间范围 0 ~ 99

		int temp = arr[a];     //经典的两数交换
		arr[a] = arr[b];
		arr[b] = temp;
	}

}

void print_action_queue(int *  arr)
{
		cout<<endl<<"action queue[ ";
	for(int i=0;i<CLIENT_NUMBER;i++)
	{
		cout<<arr[i]<<" ";
	}
	cout<<"]" <<endl;

}
int main()
{
	//initial G_pieces, 
	G_pieces.resize(PIECES_NUMBER);


	srand(random_seed);
	// client number 10

	for (int client_id=0;client_id<CLIENT_NUMBER;client_id++)
	{
		CLIENT temp_client (client_id);
		client_list.push_back(temp_client);
	}

	//initial the uploader
	for (int i=0;i<PIECES_NUMBER;i++)
	{
		get_CLIENT_N(0).add_piece(i);
	}

	//print_all_client_status();

	get_CLIENT_N(1).add_piece(0);
	vector <int> temp;
	temp.push_back(1);
	temp.push_back(0);
	//cout<<get_rarest_piece(temp);
	int turn=1;

	//to BE SAFE each first to kick people than to download 
	//suspend
	int * action_queue = new int[CLIENT_NUMBER]; //初始化数组;

	while(1)
	{
	//print something
	cout<<"==========================================="<<endl;
	cout<<"turn "<<turn<<endl;
	cout<<"==========================================="<<endl;
	get_action_queue(action_queue);
	print_action_queue(action_queue);

	cout<<"print G_pieces"<<endl;;
	print_G_PIECES();

	cout<<"all status"<<endl;
	print_all_client_status();


	//start kick out

	for(int i=0;i<CLIENT_NUMBER;i++)
	{
		client_list[action_queue[i]].kick_my_lazyiest_friend_to_betray();
		client_list[action_queue[i]].kick_the_passbay();
	}
	//start allocate


		for(int i=0;i<CLIENT_NUMBER;i++)
	{
		client_list[action_queue[i]].go_download_on_piece();
		//just down 1 , whant to download more at one time ??
			//client_list[action_queue[i]].go_download_on_piece();
	}


	//start update the status of pieces using add
	//in anther word to check the down table of each guy
	//more simple , just check -1of each client, 

	for(int i=0;i<CLIENT_NUMBER;i++)
	{
		CLIENT temp=client_list[action_queue[i]];
		//check -1
		for(int j=0;j<PIECES_NUMBER;j++)
		{
			if(temp.pieces[j]==-1) 
				client_list[action_queue[i]].add_piece(j);
		}
		
	}

	//make betrayer friends
	if(turn%2==0)
	{
			for(int i=0;i<CLIENT_NUMBER;i++)
	{
		client_list[action_queue[i]].period_clean_betrayer_list();

		
	}
	}
	turn++;
	char input;
	input=getchar();
	}
	delete [] action_queue;


}