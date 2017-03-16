// BRian DUnn
// CS163
// Assignment 3
// Filename: main.cpp
// Purpose: This is the main file that executes functionality and testing of the Hash Table.
#include "hash.h"
#include <fstream>
using namespace std;

int main(){
notes to_add;
notes to_get;
char choice;
int more = 1;
hash key_table(31);
ifstream in_file;



while(more){

	notes to_copy;
	cout << "\nEnter Choice: "<< endl;
	cout << "\n(1) Load from text file.";
	cout << "\n(2) Enter a new Topic:";
	cout << "\n(3) Remove a keyword";
	cout << "\n(4) Retrieve info for a keyword";
	cout << "\n(5) Edit info for topic:";
	cout << "\n(6) Display all keywords for given chapter:";
	cout << "\n(7) Display all Keywords and notes:";
	cout << "\n(0) Quit!\n";
		cin >> choice; 
		cin.ignore(25,'\n');
		switch(choice){
		
			case '1':
				char _keyword[50];
				char _info[250];
				int  _chapter;
				char _pages[50];
				bool _review;
				in_file.open("cs163_notes.txt");
				if(in_file)
				{
					in_file.get(_keyword, 50, '\n');
					in_file.ignore(50, '\n');
					while (!in_file.eof())
					{
						if(!to_add.add_keyword(_keyword))
								cerr << "\nKeyword not added!\n";
						in_file.get(_info, 250, '\n');
						in_file.ignore(250, '\n');
						if(!to_add.add_info(_info))
								cerr << "\nInfo Not Added!\n";
						in_file >> _chapter;
						in_file.ignore(3, '\n');
						if(!to_add.add_chapter(_chapter))
								cerr << "\nChapter Not Added\n";
						in_file.get(_pages, 50, '\n');
						in_file.ignore(50, '\n');	
						if(!to_add.add_pages(_pages))
								cerr << "\n Pages Not Added\n";
						in_file >> boolalpha >>_review;
						in_file.ignore(5, '\n');
						if(!to_add.add_review(_review))
								cerr << "\nReview not Added\n";
						if(!key_table.insert(_keyword, to_add))
								cerr << "\nNotes not Added\n";
						to_add.remove();		
						in_file.get(_keyword, 50, '\n');
								in_file.ignore(50, '\n');
					}
			}
			in_file.close();
				break;

			case '2':
				cout << "\nWhat is the KEYWORD associated with this topic? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
				cout << "\nEnter the notes for this topic? :\n";
				cin.get(_info, 250); 
				cin.ignore(250,'\n');
				cout << "\nWhat Chapter is this topic covered? :\n";
				cin >> _chapter; 
				cin.ignore(3,'\n');
				cout << "\nWhat Pages are associated with this topic?:\n";
				cin.get(_pages, 50);
				cin.ignore(50, '\n');
				cout << "\nIs further review necessary(true or false)?\n";
				cin >> boolalpha >> _review;
				cin.ignore(10,'\n');
				if(!to_add.add_keyword(_keyword))
					cerr << "\nKeyword not added!\n";
				if(!to_add.add_info(_info))
					cerr << "\nInfo Not Added!\n";
				if(!to_add.add_chapter(_chapter))
					cerr << "\nChapter Not Added\n";
				if(!to_add.add_pages(_pages))
					cerr << "\n Pages Not Added\n";
				if(!to_add.add_review(_review))
					cerr << "\nReview not Added\n";
				if(!key_table.insert(_keyword, to_add))
					cerr << "\nNotes not Added\n";
				to_add.remove();	
				break;
				
			case '3':
				cout << "\nWhat is the KEYWORD to Remove? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
				if(!key_table.remove(_keyword))
					cerr << "\n KEYWORD " <<_keyword << " NOT FOUND!\n";
				else cout << "\nKeyword: " <<_keyword << " Removed!!!\n"	;
				break;
			case '4':
				cout << "\nWhat is the KEYWORD to SEARCH for? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
				if(!key_table.retrieve(_keyword, to_get))
					cerr << "\n KEYWORD NOT FOUND!\n";
				else to_get.display();
				to_get.remove();
				break;

			case '5':
				cout << "\nWhat is the KEYWORD to EDIT? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
				cout << "\nEnter the notes for this topic? :\n";
				cin.get(_info, 250); 
				cin.ignore(250,'\n');
				if(!key_table.edit_info(_keyword, _info))
					cerr << "\nKEYWORD " << _keyword << " NOT FOUND!!!\n";
				else cout << "\nKEYWORD " << _keyword << " UPDATED!!!\n";	
				break;

			case '6':
				cout << "\nWhat CHAPTER would you like to Display the KEYWORDS for(1-21)? :\n";
				cin >> _chapter;
				cin.ignore(3, '\n');
				if(!key_table.ch_display(_chapter))
					cerr << "\nChapter " << _chapter << " is EMPTY!!!\n";
				break;	
			default:
				break;
				
			case '7':
				key_table.display_all();
				break;
				
			case '0':
				more = 0;
		};

	};
return 0;
};
