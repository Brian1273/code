// Brian Dunn
// CS 163
// Filename: notes.cpp
// Includes notes.h
// Purpose: This File implements the notes class. The notes class has functions to add, retrieve, copy and display.  Data is read from "cs163_notes.txt"

#include "notes.h"
using namespace std;

//////////////////////////////////////////////////////
// Notes Class //
/////////////////////////////////////////////////////

// Default Constructor
notes::notes()
{
	keyword = NULL;
	info = NULL;
	chapter = 0;
	pages = NULL;
	review = false;
}
// Destructor. Calls remove().
notes::~notes()
{
	remove();
}
// Remove deletes all dynamically allocated memory and sets pointers to NULL
void notes::remove()
{
	if(keyword)
		delete[] keyword;
		keyword = NULL;
	if(info)
		delete[] info;
		info = NULL;
	if(pages)
		delete[] pages;
		pages = NULL;
}	
// Add Keyword to notes. Returns 1 if added.
int notes::add_keyword(char * _keyword)
{
	keyword = new char[strlen(_keyword)+1];
	strcpy(keyword, _keyword);
	return 1;
}
// Copies keyword into bucket and returns 1.
int notes::get_keyword(char * _keyword)
{
	strcpy(_keyword, keyword);
	return 1;
}
// Add info to notes and appends additional notes. Returns 1 if added.
int notes::add_info(char * _info)
{
	// Case for new notes
	if(!info){
	info = new char[strlen(_info)+1];
	strcpy(info, _info);
	return 1;
	}
	// Case to append to existing notes
	else{
		char * temp = info;
		info = new char[strlen(_info) + strlen(info) + 1];
		strcpy(info, temp);
		strcat(info, _info);
		delete[] temp;
		return 1;
	}
	return 0;
}
// Copies info into bucket and returns 1.
int notes::get_info(char * _info)
{
	strcpy(_info, info);
	return 1;
}
// Add chapter info to notes. Returns 1 if added.
int notes::add_chapter(int _chapter)
{
	chapter = _chapter;
	return 1;
}
// Returns chapter number for notes.
int notes::get_chapter(void)
{
	return chapter;
}
// Adds pages information to notes. returns 1 if added.
int notes::add_pages(char * _pages)
{
	pages = new char[strlen(_pages)+1];
	strcpy(pages, _pages);
	return 1;
}
// Copies pages information to bucket. Returns 1 if copied.
int notes::get_pages(char * _pages)
{
	strcpy(_pages, pages);
	return 1;
}
// Adds review information to notes. Returns 1 if added.
int notes::add_review(bool _review)
{
	review = _review;
	return 1;
}
// Returns bool review
bool notes::get_review(void)
{
	return review;
}
// Copies all fields of notes to bucket copy. Returns 1 if successfull.
int notes::copy(notes &to_copy)
{
	if(!add_keyword(to_copy.keyword))
		return 0;
	if(!add_info(to_copy.info))	
		return 0;
	if(!add_chapter(to_copy.chapter))
		return 0;
	if(!add_pages(to_copy.pages))
		return 0;
	if (!add_review(to_copy.review))
		return 0;
	return 1;	
}
// Compares supplied keyword to notes keyword. Returns 1 if a match.
int notes::is_key(char * _key)
{
	if(!strcmp(_key, keyword))
		return 1;
	else return 0;	
}
// Displays all information fields.
void notes::display(void)
{
	cout << "\n*****************************************";
	cout << "\nKeyword:\t" << keyword << endl;
	cout << "\nInfo:   \t" << info << endl;
	cout << "\nChapter:\t" << chapter << endl;
	cout << "\nPages:  \t" << pages << endl;
	cout << boolalpha << "\nNeeds further review: \t" << review;
	cout << "\n*****************************************" << endl;
}