// Brian Dunn
// CS 163
// Assignment #1
// Filename: main.cpp
// Uses: travel.h
// Purpose:
/*	This is the main file for using and implementing the planner and trip classes.*/

#include "travel.h"
using namespace std;

int main(){
// Variables
airport_list airports;
planner planner1(5);
// Temp Variables for input
int  segments;
char choice;
char port[25];
bool domestic, customs, tsa;
float arrive, depart;

// Start
cout << "\n**************************";
cout << "\nWelcome To Trip Planner" << endl;
cout << "Enter Choice: "<< endl;
cout << "\n(1) Display a list of airports.";
cout << "\n(2) Start a new Plan.\n";
cin >> choice; 
cin.ignore(25,'\n');

// Switch for Displaying airports or starting a new list.
switch(choice){
	case '1':
			airports.display_all();
			cout << "\n\n\nLet's Get Started!";
	case '2':
			cout << "\nHow many Segments are there in this trip? :";
			cin >> (segments);
			planner1.segments =(segments);
			cout << "\nPlanner Created!";
		break;
};
// Switch to build the list or see airports list
cout << "Enter Choice: "<< endl;
cout << "\n(1) Display a list of airports.";
cout << "\n(2) Add details to trip and check layovers.";
cout << "\n(0) Quit!\n";
cin >> choice; 
cin.ignore(25,'\n');

switch(choice){
	case '1':
			airports.display_all();
			cout << "\n\n\nLet's Get Started!";
	case '2':
		for(int z = 0; z < segments; ++z){
			cout << "\nWhat is the Port of Departure for this segment#" << z+1 << "? :" << endl;
				cin.get(port, 25); 
				cin.ignore(25,'\n');
			cout << "\nEnter 1 if this is a Domestic Flight:";
				cin >> domestic; 
				cin.ignore(1,'\n');
			cout << "\nEnter 1 if this flight will include Customs or Immigration:";
				cin >> customs; 
				cin.ignore(1,'\n');
			cout << "\nEnter 1 if this flight has PRE-TSA approval: ";
				cin >> tsa; 
				cin.ignore(1,'\n');
			if(!(planner1.add_admin(z, port, domestic, customs, tsa)))
					cerr << "Admin information for port: " << port << "not added!";
				int again = 1;
			while(again){
				cout << "\nEnter flight#" << z+1 << " destination: ";
					cin.get(port, 25); 
					cin.ignore(25,'\n');
				cout << "\nWhat is this flight's time of Departure(24hr(1:45PM=1375)): ";
					cin >> depart; 
					cin.ignore(25,'\n');
				cout << "\nEnter flight's time of Arrival(24hr): ";
					cin >> arrive; 
					cin.ignore(25,'\n');
				if(!(planner1.add_port(z, port, depart, arrive)))
						cerr << "\nFlight#" << z+1 << " to " << port << " not added!";
				cout << "\nEnter 1 to add another Flight to this segment or 0 if done: " ;
					cin >> again; 
					cin.ignore(1,'\n');
			};	
		};
	default :
		break;
};	

int more = 1;
while(more){
	cout << "Enter Choice: "<< endl;
	cout << "\n(1) Display a list of airports.";
//	cout << "\n(2) Add details to trip and check layovers.";
	cout << "\n(3) Display Entire Trip.";
	cout << "\n(4) Display Segment by Port of Departure.";
	cout << "\n(0) Quit!\n";
		cin >> choice; 
		cin.ignore(25,'\n');
	switch(choice){
	
		case '1':
			airports.display_all();
			break;
		case '3':
			planner1.display();
			break;
		case '4':
			cout << "\nEnter the Point of Origin to search for" << endl;
				cin.get(port, 25);
				cin.ignore(25, '\n');
			if(!(planner1.display_segment(port)))
					cerr << "\nPort " << port << " not found!" << endl;
			break;
	
		default:
			break;
		case '0':
			more = 0;
	};
};
	return 0;
};

