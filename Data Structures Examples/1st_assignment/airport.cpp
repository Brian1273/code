// Brian Dunn
// CS 163
// Assignment #1
// Filename: airport.cpp
// Includes: airport.h, airport.txt
//Purpose:
/*This is the implementation file for building and accessing a list of airports from a text file (airport.txt).*/

#include "airport.h"
#include <fstream>

using namespace std;
// File stream
ifstream in_file;

// Constructor
airport_list::airport_list()
{
	head = new node;
// open filestream for input
	in_file.open("airport.txt");
// Temp variable for input	
	char input[50] = "carrots";
// Read and Parse file into data members
	if (in_file)
	{
		node * current = head;
		in_file.get(input, 50, '\t');
		in_file.ignore(50, '\t');
		while (!in_file.eof())
		{
			current->name = new char[(sizeof(input)+1)];	
			strcpy(current->name, input);
			in_file >> current->check_points;
                	in_file.ignore(50, '\t');
			in_file >> current->process_time;
                        in_file.ignore(50, '\t');
                        in_file >> current->time_to_gate;
			in_file.ignore(50, '\n');
			current->next = new node;
			current = current->next;
			in_file.get(input, 50, '\t');
                	in_file.ignore(50, '\t');

		}
	}
	else 
	{
//	cout << endl << "ERROR FILE NOT FOUND!!!";
	};
	in_file.close();
}
// Destructor
airport_list::~airport_list()
{
	node * temp = head;
	node * current = head;
	while(current->next){
		temp = current;
		delete[] current->name;
		current = current->next;
		delete temp;
	}
}
// Displays the entire list
void airport_list::display_all()
{
	node * current = head;
	while(current->next){
	cout << endl << "Airport Name: " << current->name << endl;
	cout << "Number of Check Points: " << current->check_points << endl;
	cout << "Process time(minutes): " << current->process_time << endl;
	cout << "Time to Gate(minutes): " << current->time_to_gate << endl;
	current = current->next;
	}
}

// Searches list for airport 
// Returns 1 If airport found and copies info to vars
// Returns 0 if not found
int airport_list::find_airport(char * port_tofind, int &_process_time, int &_time_to_gate)
{
	node * current = head;
        while(current->next){
		if(!strcmp(current->name, port_tofind)){
			_process_time = current->process_time;
			_time_to_gate = current->time_to_gate;
			return 1;
		}
		current = current->next;
	}
		return 0;
}
