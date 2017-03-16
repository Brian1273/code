// Brian Dunn
// CS 163
// Assignment #2
// Filename: pod.h
// Uses: 
// Purpose:
/*	This is the header file for the stack and queue classes. packages are structs with a destination sender and size. Nodes for the queue(q_node) contain a package and a next pointer. Queues class has functions for Enque, Dequeue, Peek and Display.Queue keeps the rear pointer private to protect the list. The stack node (s_node) has an array of packages and a next pointer. THe stack has functions for Push, Pop, Peek, and Display.*/


#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>

// Package Struct
struct package
{
	char * destination = NULL;
	char * sender = NULL;
	char * size = NULL;
};

// Queue node
struct q_node
{
	package  _package;
	q_node * next;
};

// Queue class
class queue
{
	public:
			//Constructor
			queue();
			//Destructor
			~queue();
			// input: blank package &_add, Output: int 1 if success and copied info into $_add, 0 if not copied.
			int enqueue(package &_add);
			// Output: int 1 if removed, 0 if not
			int dequeue();
			// input: blank package &_add, Output: int 1 if success and copied info into $_add, 0 if not copied.
			int peek(package &to_get);
			void display();
	private:
			q_node * rear;
};			

// stack node
struct s_node
{
	package * pkgs = NULL;
	s_node * next = NULL;
};

// Contents of the trailer	

class manifest
{
	public:
		// Default constructor
		manifest();
		// Constructor with size of array as arguement. Input: int max
		manifest(int max);
		// Destructor
		~manifest();
		// input: blank package &_add, Output: int 1 if success and copied info into $_add, 0 if not copied.
		int push(package &_add);
		// Output: int 1 if removed, 0 if not
		int pop();
		// input: blank package &_add, Output: int 1 if success and copied info into $_add, 0 if not copied.
		int peek(package &to_get);
		void display();
	private:
		// MAX is the array size
		int MAX; 
		s_node * head;	
		// top_index keeps track of array position
		int top_index;
};			
