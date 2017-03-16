// Brian Dunn
// CS163
// Assignment 5
// Filename: graph.cpp
// Uses: graph.h
// Purpose: This is the implementation file of an adjacency list of vertex objects. The purpose is to organize an .mp3 files into related tunings. Songs are entered and adjacency connections are made that reflect songs in the same tuning.

#include "graph.h"

using namespace std;

// Constructor. Input size sets vertex array size.
graph::graph(int size)
{
	list = new vertex[size];
	list_size = 0;
	for(int i=0; i<size; ++i){
		list[i].head = NULL;
	}
};

// Destructor. Frees all dynamically allocated memory.
graph::~graph()
{
	for(int i=0; i<list_size; ++i){
		node * temp = list[i].head;
		while(temp){
			list[i].head = temp->next;
			delete temp;
			temp = list[i].head;
		}
		delete[] list[i].song;
	}
	delete[] list;
};

// Insert. Inputs song info into vertex array. Returns 1 if added; 0 if not
int graph::insert(char * song)
{
	list[list_size].head = NULL;
	list[list_size].song = new char[strlen(song)+1];
	strcpy(list[list_size].song, song);
	list[list_size].visited = false;
	++list_size;
	return 1;
};

// Display. Displays all vertexes. Returns 0 if list is empty. 1 if successful.
int graph::display(void)
{
	for(int i=0; i<list_size; ++i)
		cout << "\nSong: " << list[i].song << endl;
	return 1;
};
// Find Location. Returns the index location of the song. Returns -1 if not found.
int graph::find_location(char * song)
{
	for(int i=0; i<list_size; ++i){
		if(!strcmp(song, list[i].song))
			return (i);
	}
	return -1;
};	

// Display_adjacent. Displays all songs connected to input song. Returns 0 if song not found. Calls dis_adj() recursively.
int graph::display_adjacent(char * song)
{
	int index = find_location(song);
	if(index < 0)
		return 0;
	else if(!list[index].head)
		return 0;
	else{
		clear_visited();
		return (dis_adj(list[index].head));
	}
};
// Recursive display adjacent. Displays all adjacencies to vertex.
int graph::dis_adj(node * head){
	if(!head)
		return 0;
	cout << "\nSong: " << head->adjacent->song;
	head->adjacent->visited = true;
	dis_adj(head->next);
	return 1;
};

// Insert Edge. Inserts an adjacency to current vertex. Returns 1 if added. 0 if not.
int graph::insert_edge(char * current_vertex, char * to_link)
{
		int current = find_location(current_vertex);
		int link = find_location(to_link);
		if(current<0||link<0)
			return 0;
		node * temp = new node;
		temp->adjacent = &list[link];
		temp->next = list[current].head;
		list[current].head = temp;
		return 1;
};
// Clear vistited vertexes.
void graph::clear_visited(void)
{
	for(int i=0; i<list_size; ++i){
		list[i].visited = false;
	}
};