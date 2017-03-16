// Brian Dunn
// CS 163
// Assignment #1
// Filename: travel.cpp
// Uses: travel.h
// Purpose:
/*	This is the implementation file for using and implementing the planner and trip classes.*/

#include "travel.h"
using namespace std;

//**********************************************//
//**********************************************//
// trip class
//**********************************************//
//**********************************************//

// Default Constructor
trip::trip()
{
	head = NULL;
}
// Destructor
trip::~trip()
{
	if(head){
		port_node * temp = head->port_of_departure;
		port_node * current = head->port_of_departure;
		delete[] head->origin;
		while(temp->next){
			current = temp;
			temp = temp->next;
			delete [] current->port;
			delete current;
		};
		delete head;	
	};
}
// Adds Admin node and fills in data. Returns 1 if added.
int trip::add_admin(char * origin, bool domestic_flight, bool customs_imm, bool tsa)
{
	head = new admin_node;
        head->domestic = domestic_flight;
        head->customs = customs_imm;
        head->pre_tsa = tsa;
        head->port_of_departure = NULL;
		head->origin = new char[strlen(origin)+1];
		strcpy(head->origin, origin);
        return 1;
}
// Add a port to the list and fills in given data; Returns 1 if added

int trip::add_port(char * name_of_airport, float depart_time, float arrive_time)
{
// case for empty list
    if(!head->port_of_departure){
		head->port_of_departure = new port_node;
        head->port_of_departure->port = new char[sizeof(name_of_airport)+1];
        strcpy(head->port_of_departure->port, name_of_airport);
        head->port_of_departure->departure_time = depart_time;
        head->port_of_departure->arrival_time = arrive_time;
        head->port_of_departure->next = NULL;
            return 1;
        }
// case for active list		
	else {
		airport_list airports;
		port_node * temp = head->port_of_departure;
		while(temp->next)
			temp=temp->next;
// Check If enough time in layover; return 0 if not			
			if(!(check_layover(airports, name_of_airport, depart_time, temp->arrival_time, head->domestic, head->customs, head->pre_tsa))){
			//	cerr << "Check Layover Failed!";
				return 0;
			};
			temp->next = new port_node;
			temp=temp->next;
			temp->port = new char[sizeof(name_of_airport)+1];
            strcpy(temp->port, name_of_airport);
            temp->departure_time = depart_time;
            temp->arrival_time = arrive_time;
            temp->next = NULL;
//	cout << "\nAnother port Added\n";	
	};
	
    return 1;
};
// Displays entire trip
void trip::display_all(){
	if(!head)
		return;
	admin_node* temp = head;
	cout << boolalpha;
	cout << "\n\n\n************Flight Leaving From:\t" << temp->origin;
	cout << "\nThis Segment is a domestic Flight:\t " << temp->domestic;
	cout << "\nThis Segment has custom/immigration checkpoints: \t" <<temp->customs;
	cout << "\nThis Segment has Pre-TSA Approval: \t" << temp->pre_tsa;
	port_node *temp1=temp->port_of_departure;
	while(temp1){
		cout << "\n\tFirst Class Flight to the city of:\t" << temp1->port;
		cout << "\n\tThis Flight's time of Departure is: \t" <<temp1->departure_time;
		cout << "\n\tThis Flight's time of Arrival is: \t" << temp1->arrival_time << endl;
		temp1=temp1->next;
	}
};
// Checks layover of given airport, admin info, and times.
// Returns 1 if enough time; 0 if not found; 0 if not enought time.
bool trip::check_layover(airport_list &airports, char * name_of_airport, float depart_time, float arrive_time, bool domestic_flight, bool customs_imm, bool tsa){
	int process_time, time_to_gate;
	if(!airports.find_airport(name_of_airport, process_time, time_to_gate))
		return 0;
	if((domestic_flight && tsa) || (!domestic_flight && !customs_imm))
		process_time = 0;
	if((depart_time - process_time - time_to_gate) >= arrive_time)
		return 1;
	else
		return 0;
		
};
// returns 1 if airport is origin of segment
int trip::is_origin(char * airport){
	if(!strcmp(airport, head->origin))
		return 1;
	else return 0;	
};	
//**********************************************//
//**********************************************//
//	Planner Class
//**********************************************//
//**********************************************//
// Default Constructor
planner::planner(){
	head = NULL;
};

// Constructor for multiple flights
planner::planner(int num_of_segments){
	head = new trip[num_of_segments];
	segments = num_of_segments;
};

// Destuctor
planner::~planner(){
	delete[] head;
};

// Displays entire trip
void planner::display(){
	for(int i =0; i < segments; ++i){
		head[i].display_all();
	}
};

// Displays Segment if arguement is Originating airport 1 if found
int planner::display_segment(char * airport){
	for(int i =0; i < segments; ++i){
		cout << "\nsegment #" << i;
		if(head[i].is_origin(airport)){
			head[i].display_all();
			return 1;
		}
	}
	return 0;	
};	
	
// Adds admin details; 1 if added.
int planner::add_admin(int segment, char * origin, bool domestic_flight, bool customs_imm, bool tsa){
	if(head[segment].add_admin(origin, domestic_flight, customs_imm, tsa))
		return 1;
	else 
		return 0;
};
// Adds a port to segment; 1 if added.
int planner::add_port(int segment, char * name_of_airport, float depart_time, float arrive_time){
	if (head[segment].add_port(name_of_airport, depart_time, arrive_time))
		return 1;
	else 
		return 0;
};