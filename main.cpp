// Brian Dunn
// CS 163
// Filename main.cpp
// includes "pod.h"
// Purpose: This is the implementation file for stack and queue classes in pod.h. This program allows the user to test all functions of the stack and queue classes. Reports success or failure of functions.
#include "pod.h"
#include <iostream>
#include <cstring>
#include <cstdlib> 
#include <cctype> 
using namespace std;
// main()
// Variables
int main(){
char choice;
int more = 1;
// Array size variable
int ARRAY_SIZE = 3;
//packages to use as arguements for functions
package _add, to_get;
// Creates the stack with an array size
manifest truck1(ARRAY_SIZE);
queue orders;
while(more){
	cout << "\nEnter Choice: "<< endl;
	cout << "\n(1) Add Package to Deliver.(PUSH & ENQUEUE)";
	cout << "\n(2) Display Truck Loading List.(STACK)";
	cout << "\n(3) Display List of Deliveries.(QUEUE)";
	cout << "\n(4) Display Next Package to Deliver.(QUEUE PEEK)";
	cout << "\n(5) Display Next Package to Load.(STACK PEEK)";
	cout << "\n(6) Remove Delivered Item.(DEQUEUE)";
	cout << "\n(7) Remove Item From Loading List.(POP)";
	cout << "\n(0) Quit!\n";
		cin >> choice; 
		cin.ignore(25,'\n');
	switch(choice){
	
		case '1':
			char dest[50];
			char send[50];
			char sz [50];
			cout << "\nWhat is the Destination? :\n";
			cin.get(dest, 50); 
			cin.ignore(50,'\n');
			cout << "\nWho is the Sender? :\n";
			cin.get(send, 50); 
			cin.ignore(50,'\n');
			cout << "\nWhat Size is the Package? :\n";
			cin.get(sz, 50); 
			cin.ignore(50,'\n');
			_add.destination = new char[strlen(dest)+1];
			strcpy(_add.destination, dest);
			_add.sender= new char[strlen(send)+1];
			strcpy(_add.sender, send);
			_add.size = new char[strlen(sz)+1];
			strcpy(_add.size, sz);
			if(!(orders.enqueue(_add)))
				cerr << "\nPackage not Added!!\n";
			truck1.push(_add);
			delete[] _add.sender;
			delete[] _add.destination;
			delete[] _add.size;
			break;
		case '2':
			truck1.display();
			break;
		case '3':
			orders.display();
			break;
		case '4':
			if(!(orders.peek(to_get))){
				 cerr << "\nPackage not Found!\n";
				break;
			}
                        cout << "\n*******NEXT PACKAGE TO DELIVER***************";
			cout << "\n Destination: " << to_get.destination;
                        cout << "\n Sender: " << to_get.sender;
                        cout << "\n Size: " << to_get.size << endl;
			if(to_get.destination)
                                delete[] to_get.destination;
                        if(to_get.sender)
                                delete[] to_get.sender;
                        if(to_get.size)
                                delete[] to_get.size;

                        break;

		case '5':
			if(!(truck1.peek(to_get))){
				cerr << "\nPackage not Found!\n";
				break;
			}
			cout << "\n*******NEXT PACKAGE TO LOAD***************";
			cout << "\n Destination: " << to_get.destination;
			cout << "\n Sender: " << to_get.sender;
			cout << "\n Size: " << to_get.size << endl;
			if(to_get.destination)
				delete[] to_get.destination;
			if(to_get.sender)
				delete[] to_get.sender;
			if(to_get.size)
				delete[] to_get.size;
			break;
		case '6':
			if(!(orders.dequeue()))
				cerr << "\nPackage NOT Removed\n";
			break;
		case '7':
			if(!(truck1.pop()))
				cerr << "\nPackage NOT Removed\n";
		default:
			break;
		case '0':
			more = 0;
	};
};

return 0;
};

