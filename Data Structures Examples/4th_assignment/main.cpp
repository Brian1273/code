// Brian Dunn
// CS163
// Assignment 4
// Filename: main.cpp
// Uses: bst.h
// Purpose: This is the main file that executes functionality and testing of the Hash Table.

#include <fstream>
#include "bst.h"
using namespace std;

int main(){
// BUckets	
notes to_add;
notes to_get;
// VAR for switch
char choice;
int more = 1;
ifstream in_file;
// BST
bst btree;

// Choice MEnu
while(more){

	notes to_copy;
	cout << "\nEnter Choice: "<< endl;
	cout << "\n(1) Load from text file.";
	cout << "\n(2) Enter a new Topic:";
	cout << "\n(3) Remove a keyword";
	cout << "\n(4) Retrieve info for a keyword";
	cout << "\n(5) Edit info for topic:";
	cout << "\n(6) Display all keywords in Order:";
	cout << "\n(7) Display all Keywords Within range:";
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
						if(!btree.insert(_keyword, to_add))
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
				if(!btree.insert(_keyword, to_add))
					cerr << "\nNotes not Added\n";
				to_add.remove();	
				break;
				
			case '3':
				cout << "\nWhat is the KEYWORD to Remove? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
// add remove here				
				if(!btree.remove(_keyword))
					cerr << "\n KEYWORD " <<_keyword << " NOT FOUND!\n";
				else cout << "\nKeyword: " <<_keyword << " Removed!!!\n"	;
				break;
			case '4':
				cout << "\nWhat is the KEYWORD to SEARCH for? :\n";
				cin.get(_keyword, 50); 
				cin.ignore(50,'\n');
// Add retrieve here
				if(!btree.retrieve(_keyword, to_get))	
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
				if(!btree.edit_info(_keyword, _info))
					cerr << "\nKEYWORD " << _keyword << " NOT FOUND!!!\n";
				else cout << "\nKEYWORD " << _keyword << " UPDATED!!!\n";	
				break;

			case '6':
				if(!btree.display())
					cerr << "\n Tree is empty!!!\n";
				cout << "\nTree Height is: " << btree.get_height() << endl;
				cout << "\nNode Count is: " << btree.get_count() << endl;
				break;	
			
			default:
				
				break;
				
			case '7':
				char begin, end;
				cout << "\nWhat is the beginning letter to start search from?: \n";
				cin >> begin; 
				cin.ignore(25,'\n');
				cout << "\nWhat is the letter to search up to inclusive?: \n";
				cin >> end; 
				cin.ignore(25,'\n');
				if(!btree.display_range(begin, end))
					cerr << "\nSearch Did NOT Find Anything in range!\n";
				break;
				
			case '0':
				more = 0;
		};

	};
return 0;
};

