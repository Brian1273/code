// Brian Dunn
// CS163
// Assignment 5
// Filename: graph.h
// Purpose: This is the header file for an adjacency list of vertex objects. The purpose is to organize an .mp3 files into related tunings. Songs are entered and adjacency connections are made that reflect songs in the same tuning.
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
// Vertex Struct
struct vertex
{
	char * song;
	bool visited;
	struct node * head;
};
// Node Struct
struct node
{
	vertex * adjacent;
	node * next;
};

// graph Class.
class graph
{
	public:
	// Constructor. Input size sets vertex array size.
	graph(int size=128);
	// Destructor. Frees all dynamically allocated memory.
	~graph(void);
	// Insert. Inputs song info into vertex array. Returns 1 if added; 0 if not
	int insert(char * song);
	// Display. Displays all vertexes. Returns 0 if list is empty. 1 if successful.
	int display(void);
	// Find Location. Returns the index location of the song. Returns -1 if not found.
	int find_location(char * song);
	// Display_adjacent. Displays all songs connected to input song. Returns 0 if song not found. Calls dis_adj() recursively.
	int display_adjacent(char * song);
	// Insert Edge. Inserts an adjacency to current vertex. Returns 1 if added. 0 if not.
	int insert_edge(char * current_vertex, char * to_link);
	
	
	private:
	vertex * list;
	int list_size;	
	void clear_visited(void);
	// Recursive display adjacent. Displays all adjacencies to vertex.
	int dis_adj(node*head);
};
