// Brian Dunn
// CS 163
// Assignment #1
// Filename: airport.h
// Uses: airport.txt
//Purpose:
/*This is the header file for building and accessing a list of airports from a text file (airport.txt).*/

#include <cstring>
#include <cctype>
#include <iostream>
#include <cstdlib>

// Node with airport information
struct node
{
	char * name;
	int check_points;
	int process_time;
	int time_to_gate;
	node * next;
};

// class Prototypes
class airport_list
{
	public:
		airport_list();
		~airport_list();
		void display_all();
		int find_airport(char * port_tofind, int &_process_time, int &_time_to_gate);
	private:
		node * head;
};
