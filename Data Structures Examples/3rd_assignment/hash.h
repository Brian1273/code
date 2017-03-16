// Brian Dunn
// CS 163
// assignment 3
// Filename: hash.h
// Purpose: This is the header file for the hash table class and notes class. The notes class has functions to add, retrieve, copy and display. The hash table class has functions for insert, retrieve, remove, edit info, display all, and display keyword by chapter. Data is read from and written to "cs163_notes.txt"

#include <cstdlib>
#include <cctype>
#include <cstring>
#include <iostream>
#include <fstream>
//////////////////////////////////////////////////////
// Notes Class //
/////////////////////////////////////////////////////
class notes
{
	public:
		notes();	// Default Constructor
		~notes();	// Destructor. Calls remove().
		int add_keyword(char * _keyword);	// Add Keyword to notes. Returns 1 if added.
		int get_keyword(char * _keyword);	// Copies keyword into bucket and returns 1.
		int add_info(char * _info);	// Add info to notes and appends additional notes. Returns 1 if added.
		int get_info(char * _info);	// Copies info into bucket and returns 1.
		int add_chapter(int _chapter);	// Add chapter info to notes. Returns 1 if added.
		int get_chapter(void);	// Returns chapter number for notes.
		int add_pages(char * _pages);	// Adds pages information to notes. returns 1 if added.
		int get_pages(char * _pages); // Copies pages information to bucket. Returns 1 if copied.
		int add_review(bool _review); // Adds review information to notes. Returns 1 if added.	
		bool get_review(void);	// Returns bool review
		int copy(notes &to_copy);	// Copies all fields of notes to bucket copy. Returns 1 if successfull.
		int is_key(char * _key);	// Compares supplied keyword to notes keyword. Returns 1 if a match.
		void display(void);	// Displays all information fields.
		void remove();	// Remove deletes all dynamically allocated memory and sets pointers to NULL
	private:	
		char * keyword;
		char * info;
		int chapter;
		char * pages;
		bool review;
	
};
// Struct for hash table node
struct node
{
	notes data;
	node * next;
};
// Struct for chapter table node
struct ch_node
{
	char * keyword;
	ch_node * next;
};
//////////////////////////////////////////////////////
// HASH TABLE //
/////////////////////////////////////////////////////
class hash
{
	public:
		// Constructor. Takes size argument to create array of SIZE. Creates Chapter keyword table intialized to 21 elements for each chapter.
		hash(int size);
		// Destructor. Removes all dynamicall allocated memory from hash table and key table.
		~hash();
		// Insert. Inserts notes into hash table and adds associated keywords to chapter list. Returns 1 if added.
		int insert(char * key,notes &to_add);
		
		int retrieve(char * key, notes &to_get);
		int remove(char * key);
		int edit_info(char * key, char * _info);
		int display_all();
		int ch_display(int chapt);

	private:
//data		
		node ** hash_table;	//Array of pointers
		ch_node ** chapter_list;	//Array of pointers
		int table_size;	//Hash table size
// Functions		
		// Hash Function. Generates index by parsing keyword string. Each character is multiplied then modded by table size to produce index. returns index
		int hash_func(char * key);
		int ch_insert(int ch_key, char * key);
		void write_all();
};
