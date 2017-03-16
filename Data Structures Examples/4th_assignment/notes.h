// Brian Dunn
// CS 163
// assignment 3
// Filename: notes.h
// Purpose: This is the header file for the notes class. The notes class has functions to add, retrieve, copy and display. 
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