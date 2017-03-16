// Brian Dunn
// CS163
// Filename: main5.cpp
#include <fstream>
#include "graph.h"
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>

using namespace std;

int main(){
// Buckets	
char _song[50], song2[50];

// VAR for switch
char choice;
int more = 1;

graph setlist;

// Choice Menu
while(more){
	cout << "\nEnter Choice: "<< endl;
	cout << "\n(1) Enter a new Song:";
	cout << "\n(2) Connect songs:";
	cout << "\n(3) Display all Songs";
	cout << "\n(4) Display all Songs in same tuning:";
	cout << "\n(0) Quit!\n";
		cin >> choice; 
		cin.ignore(25,'\n');
		switch(choice){
		
			
			case '1':
				cout << "\nWhat is the song filename?(you may omit .mp3 extension):\n";
				cin.get(_song, 50); 
				cin.ignore(50,'\n');
				if(!setlist.insert(_song))
					cerr << "\nSONG NOT ADDED\n";
				else cout << "\nSong Added.\n";	
								
				break;
				
			case '2':
				cout << "\nWhat is the first related Song:\n";
				cin.get(_song, 50); 
				cin.ignore(50,'\n');
				cout << "\nWhat song is this related to:\n";
				cin.get(song2, 50); 
				cin.ignore(50,'\n');
				if(!setlist.insert_edge(_song, song2))
					cerr << "\nEdge not Added!!!\n";
				else if(!setlist.insert_edge(song2, _song))
					cerr << "\nEdge not Added!!!\n";
				else cout << "\nEdge Added!\n";
				break;	
			
			case '3':
				if(!setlist.display())
					cerr << "\nLIST IS EMPTY\n";
				
				break;	
			
			case '4':
			cout << "\nWhat song would you like to see the related songs: \n";
			cin.get(_song, 50); 
				cin.ignore(50,'\n');
			cout << "\n**********************************\n";
			cout << "Songs related to " << _song << " are:";			
			if(!setlist.display_adjacent(_song))
					cerr << "\nNo related songs to " << _song << endl;
				
				break;
				
			case '0':
				more = 0;
			default:
				break;
		};

	};
return 0;
};

