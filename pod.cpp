// Brian Dunn
// CS 163
// Assignment #2
// Filename: pod.cpp
// Uses: pod.h
// Input: struct package
// Output Displays package details, package
// Purpose:The purpose of this file is to implement the stack and queue functions. The queue has functions for Enqueue, Dequeue, Peek, and Display. The Stack function (manifest::) has functions for Push, Pop, Peek, and Display. The stack has an overloaded constructor allowing the size (max) of the array to be passed in as an arguement. Destructors free all dynamically allocated memory. All functions except display provide error reporting through return values.

#include "pod.h"
using namespace std;
/*************************************************
Queue is a circular linked list of packages.
*************************************************/

// Queue Constructor
queue::queue(){
	rear = NULL;
}
// Queue Destructor
queue::~queue(){
	if(rear){
		q_node * temp = rear->next;
		q_node * current = temp;
		rear->next = NULL;
		while(temp){
			delete[] temp->_package.destination;
			delete[] temp->_package.sender;
			delete[] temp->_package.size;
			current = temp->next;
			delete temp;
			temp = current;
		};
	}
}
// Queue Enqueue. 
//Copies _add to a new node. Returns 1 if added, 0 if not.
int queue::enqueue(package &_add){
	// Case for Rear = NULL
	if(!rear){
		rear = new q_node;
		rear->_package.destination = new char [strlen(_add.destination)+1];
		strcpy(rear->_package.destination, _add.destination);
		rear->_package.sender = new char [strlen(_add.sender)+1];
                strcpy(rear->_package.sender, _add.sender);
		rear->_package.size = new char [strlen(_add.size)+1];
                strcpy(rear->_package.size, _add.size);
		rear->next = rear;
		return 1;
	}	
	// Case for adding to an existing list
	else{
		q_node * temp = rear->next;
		rear->next = new q_node;
		rear = rear->next;
		rear->next = temp;
		rear->_package.destination = new char [strlen(_add.destination)+1];
                strcpy(rear->_package.destination, _add.destination);
                rear->_package.sender = new char [strlen(_add.sender)+1];
                strcpy(rear->_package.sender, _add.sender);
                rear->_package.size = new char [strlen(_add.size)+1];
                strcpy(rear->_package.size, _add.size);
		return 1;
	};
	return 0;
}
// Queue dequeue. 
//Removes item at front of list. Returns 1 if removed, 0 if not.
int queue::dequeue(){
	// case for rear = NULL.
	if(!rear)
		return 0;
	// Case for 1 node
	if(rear == rear->next){
		delete[] rear->_package.destination;
		delete[] rear->_package.sender;
		delete[] rear->_package.size;
		delete rear;
		rear = NULL;
		return 1;
	}
	// Remove node at front of list.
	else{
		q_node * temp = rear->next;
		delete[] temp->_package.destination;
                delete[] temp->_package.sender;
                delete[] temp->_package.size;
		rear->next = temp->next;
                delete temp;
		return 1;
	};
	return 0;
}
// Queue peek. 
//Returns a copy of the item at front of list to the supplied arguement. Returns 1 if found, 0 if not.
int queue::peek(package &to_get){
	if(!rear)
		return 0;
	else{
		q_node * temp = rear->next;
		to_get.destination = new char [strlen(temp->_package.destination)+1];
		strcpy(to_get.destination, temp->_package.destination);
		to_get.sender = new char [strlen(temp->_package.sender)+1];
	        strcpy(to_get.sender, temp->_package.sender);
		to_get.size = new char [strlen(temp->_package.size)+1];
	        strcpy(to_get.size, temp->_package.size);
		return 1;
	};
	return 0;
}
//Queue display all. 
//Displays the entire list.
void queue::display(){
	if(!rear){
		cout << "\nThe List is Empty!!!\n";
		return;
	};
	if(rear == rear->next){
		cout << "\nDestination: " << rear->_package.destination;
		cout << "\nSender: " << rear->_package.sender;
		cout << "\nSize: "	<< rear->_package.size << endl;
	}
	else{
		q_node * temp = rear->next;
		do{
			cout << "\nDestination: " << temp->_package.destination;
	                cout << "\nSender: " << temp->_package.sender;
        	        cout << "\nSize: "      << temp->_package.size << endl;
			temp = temp->next;
		}while(temp != rear->next);	
	};
}

/***********************************************
Manifest is linear linked list of package arrays
Used as a stack.
************************************************/
//Default constructor
manifest::manifest(){
	head = NULL;
	MAX = 2;
}
//Constructor with variable array size(max).
manifest::manifest(int max){
	head = NULL;
	MAX = max;
}
// Destructor.
manifest::~manifest(){
	if(!head)
		return;
	while(head){
		pop();
	};	
}
//Stack Push. 
//Copies supplied package to stack. Returns 1 if added, 0 if not.
int manifest::push(package &_add){
	// Case for empty list. Creates new node and array of packages of size MAX.
	if(!head){
		head = new s_node;
		head->pkgs = new package[MAX];
		head->next = NULL;
		top_index = 0;
		head->pkgs->destination = new char [strlen(_add.destination)+1];
                strcpy(head->pkgs->destination, _add.destination);
                head->pkgs->sender = new char [strlen(_add.sender)+1];
        	strcpy(head->pkgs->sender, _add.sender);
                head->pkgs->size = new char [strlen(_add.size)+1];
	        strcpy(head->pkgs->size, _add.size);
                ++top_index;
		++head->pkgs;
		return 1;
	}
	// Case for a full array. creates new node and new array of size MAX
	else if(top_index == MAX){
		s_node * temp = head;
		head = new s_node;
		head->pkgs = new package[MAX];
		top_index = 0;
		head->next = temp;
		head->pkgs->destination = new char [strlen(_add.destination)+1];
                strcpy(head->pkgs->destination, _add.destination);
                head->pkgs->sender = new char [strlen(_add.sender)+1];
                strcpy(head->pkgs->sender, _add.sender);
                head->pkgs->size = new char [strlen(_add.size)+1];
                strcpy(head->pkgs->size, _add.size);
                ++top_index;
                ++head->pkgs;
		return 1;
	}
	// Case for Not full array
	else if(top_index < MAX){
		head->pkgs->destination = new char [strlen(_add.destination)+1];
                strcpy(head->pkgs->destination, _add.destination);
                head->pkgs->sender = new char [strlen(_add.sender)+1];
                strcpy(head->pkgs->sender, _add.sender);
                head->pkgs->size = new char [strlen(_add.size)+1];
                strcpy(head->pkgs->size, _add.size);
                ++top_index;
                ++head->pkgs;
		return 1;	
	}
	return 0;
}
// Stack pop. 
//Removes the last item added. Returns 1 if removed, 0 if not.
int manifest::pop(){
	//IF the list is empty return 0
	if(!head)
		return 0;
	//Case for 1 item in array. If only node then sets head to Null
	if(top_index == 1){
		s_node * temp = head;
		--temp->pkgs;
		delete[] temp->pkgs->destination;
		delete[] temp->pkgs->sender;
		delete[] temp->pkgs->size;
		if(!(head->next)){
			delete[] head->pkgs;
			delete head;
			head = NULL;
			return 1;
		}
	// if another node then move head to next node	
		head = head->next;
		delete[] temp->pkgs;
		delete temp;
		top_index = MAX;
		return 1;
	}
	// Case for more than 1 item in array
	else{
		--head->pkgs;
        	delete[] head->pkgs->destination;
        	delete[] head->pkgs->sender;
	        delete[] head->pkgs->size;
		--top_index;
		return 1;
	};
}
// Stack peek. 
//Returns/copies package at top of stack to supplied arguement(to_get). Returns 1 if copied, 0 if not.
int manifest::peek(package &to_get){
	if(!head)
		return 0;
	package * temp = head->pkgs;
	--temp;
	to_get.destination = new char [strlen(temp->destination)+1];
        strcpy(to_get.destination, temp->destination);
        to_get.sender = new char [strlen(temp->sender)+1];
        strcpy(to_get.sender, temp->sender);
        to_get.size = new char [strlen(temp->size)+1];
        strcpy(to_get.size, temp->size);	
	return 1;
}
// Stack display all.
void manifest::display(){
// Case for an empty list
	if(!head){
		cout << "\nTHE LIST IS EMPTY!\n";
		return;
	}	
// Starts at head and continues until the end of the list
	else{
		s_node * temp = head;	
		int indx = top_index ;
		package * timp = temp->pkgs;
		// Cycle through the array in reverse order
		cout << "\n*****Truck/POD Loading Order List****\n";
		while(--indx > -1){
				--timp;
				cout << "\nDestination : \t" << timp->destination;
				cout << "\nSender :\t" << timp->sender;
				cout << "\nSize :\t" << timp->size;
		}
		// if no other nodes then return
		if(!temp->next)
			return;
		// while there are more nodes
		while(temp->next){
			temp = temp->next;
			indx = MAX;
			timp = temp->pkgs;
			while(--indx > -1){
				--timp;					
				cout << "\n";
				cout << "\nDestination : \t" << timp->destination;
				cout << "\nSender :\t" << timp->sender;
				cout << "\nSize :\t" << timp->size;
			}
		}	
	};	
}

