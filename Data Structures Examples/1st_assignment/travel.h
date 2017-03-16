// BRian Dunn
// CS 163
// Assignment #1
// Filename: travel.h
// Uses: airport.h
// Purpose:
/*	This is the header file for the planner and trip classes.*/
#include "airport.h"
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>

// struct port_node
struct port_node
{
	char * port;	
	float departure_time; 
	float arrival_time;
	port_node * next ;	
};
// admin node
struct admin_node
{
	bool domestic;
	bool customs;
	bool pre_tsa;
	char * origin;
	port_node * port_of_departure;
};

// travel class
class trip
{
	public:
		trip();
		~trip();
	//Displays entire list	
		void display_all() ; 
	// Add admin info to the flight	
		int add_admin(char * origin, bool domestic_flight, bool customs_imm, bool tsa);
	
	// Add Destination information	
		int add_port(char * name_of_airport, float depart_time, float arrive_time);
		
	// Checks flight times against airport delay times
	// Returns 1 if enough layover time between flights
	// Returns 0 if airport not found or not enough time
		bool check_layover(airport_list &airports, char * name_of_airport, float depart_time, float arrive_time, bool domestic_flight, bool customs_imm, bool tsa);
	
	// Returns 1 if airport is segments point of origin
		int is_origin(char * airport);
	
	private:
			admin_node * head;
};
// planner class
class planner
{
	public:
	// Default Constructor	
		planner();
	
	// Contructor with variable segments	
		planner(int num_of_segments);
	
	// Destructor
		~planner();
	
	// Display entire trip	
		void display();
	
	// Display segment by point	of origin
	// returns 1 if found and displays segment
	// returns 0 if not found
		int display_segment(char * airport);
	
	// Add flight details
	// returns 1 if added
		int add_admin(int segment, char * origin, bool domestic_flight, bool customs_imm, bool tsa);
		int add_port(int segment, char * name_of_airport, float depart_time, float arrive_time);
		int segments;
	
	private:
		trip* head;
		
}; 
