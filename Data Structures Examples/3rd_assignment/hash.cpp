// Brian Dunn
// CS 163
// Filename: hash.cpp
// Includes hash.h
// Purpose: This File implements the hash table class and notes class. The notes class has functions to add, retrieve, copy and display. The hash table class has functions for insert, retrieve, remove, edit info, display all, and display keyword by chapter. Data is read from and written to "cs163_notes.txt"

#include "hash.h"
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
//////////////////////////////////////////////////////
// HASH TABLE //
/////////////////////////////////////////////////////

// Constructor. Takes size argument to create array of SIZE. Creates Chapter keyword table intialized to 21 elements for each chapter.
hash::hash(int size)
{
	table_size = size;
	hash_table = new node * [size];
	chapter_list = new ch_node * [21];
	for(int i = 0; i<size; ++i)
	{
		hash_table[i] = NULL;
	}	
	for(int i= 0; i < 21; ++i)
	{
		chapter_list[i] = NULL;
	}
}
// Destructor. Removes all dynamicall allocated memory from hash table and key table.
hash::~hash()
{
	// Write all data to text file
	write_all();
	node * temp, *current = NULL;
	// Hash_table destructor	
	for(int i = 0; i < table_size; ++i){
		if(hash_table[i]){
			temp = hash_table[i];
			while(temp){
				current = temp;
				temp = temp->next;
				delete current;	
			}	
		}
	}
	delete[] hash_table;
	// Chapter table destructor	
	ch_node * _temp = NULL;
	for(int i=0; i<21; ++i)
		if(chapter_list[i]){
			_temp = chapter_list[i];
			while(_temp){
				_temp = _temp->next;
				delete[] chapter_list[i]->keyword;
				delete chapter_list[i];
				chapter_list[i] = _temp;
			}
		}
	delete[] chapter_list;		
}
// Hash Function. Generates index by parsing keyword string. Each character is multiplied then modded by table size to produce index. returns index
int hash::hash_func(char * key)
{
	int length = strlen(key);
	int code = 0;
	for(int i = 0; i < length; ++i)
	{
		code += key[i];
	}
	return (code % table_size);
}
// Insert. Inserts notes into hash table and adds associated keywords to chapter list. Returns 1 if added.
int hash::insert(char * key, notes &to_add)
{
	int _chapter;
	int index = hash_func(key);
	cout << "\nindex is:" << index;
	node * temp = new node;
	if(!temp->data.copy(to_add))
        {
            delete temp;
            return 0;
        }
	temp->next = hash_table[index];
	hash_table[index] = temp;
	_chapter = to_add.get_chapter();
	if(!ch_insert(_chapter, key))
		return 0;
	return 1;
}
// Chapter insert. Adds keyword to associated chapter list. Returns 1 if added.
int hash::ch_insert(int ch_key, char * key)
{
	int index = ((ch_key -1) % 21);
	ch_node * temp = new ch_node;
	temp->keyword = new char[strlen(key)+1];
	strcpy(temp->keyword, key);
	temp->next = chapter_list[index];
	chapter_list[index] = temp;
	return 1;
}
// Retrieve. Searches for key and if found copies information to bucket and returns 1. Returns 0 if not found
int hash::retrieve(char * key, notes &to_get)
{
	int index = hash_func(key);
	node * temp = hash_table[index];
	if(!temp)
		return 0;
	while(temp){
		if(temp->data.is_key(key)){
			if(to_get.copy(temp->data))
			return 1;	
			else return 0;
		}	
		else temp = temp->next;	
	}
	return 0;
}
// Remove. Searches for key and if found removes notes from table and returns 1. Returns 0 if not found.
int hash::remove(char * key)
{
	int index = hash_func(key);
	node * temp = hash_table[index];
	node * previous = temp;
	if(!temp)
		return 0;
	while(temp){
		if(temp->data.is_key(key)){
			previous->next = temp->next;
			if(hash_table[index] == temp) {
				hash_table[index] = temp->next;
			}	
			delete temp;
			return 1;
		}else {
			previous = temp;
			temp = temp->next;	
		}	
	}		
	return 0;
}
// Edit info. Searches for key and if found, appends additional text to notes information and returns 1. Returns 0 if not found.
int hash::edit_info(char * key, char * _info)
{
	int index = hash_func(key);
	node * temp = hash_table[index];
	if(!temp)
		return 0;
	while(temp){
		if(temp->data.is_key(key)){
			if(temp->data.add_info(_info))
			return 1;	
			else return 0;
		}	
		else temp = temp->next;	
	}
	return 0;
}
// Display All. Displays all data for each entry Returns 0 if empty.
int hash::display_all()
{
	if(!hash_table)
		return 0;
	node * temp = NULL;	
	for(int i = 0; i < table_size; ++i){
		temp = hash_table[i];
		if(temp){
			while(temp){
				temp->data.display();
				temp = temp->next;
			}
		}
	}
	return 1;
}
// Chapter Display. Displays all keywords for given chapter. Returns 0 if chapter is empty.
int hash::ch_display(int chapt)
{
	if(!chapter_list[chapt - 1])
		return 0;
	cout << "\nChapter " << chapt << " KEYWORDS are :\n";
	ch_node * temp = chapter_list[chapt -1];
	while(temp)
	{	
		cout << "\n" << temp->keyword << endl;
		temp = temp->next;
	}
	return 1;
}
// Write all. Writes all data to text file("cs163_notes.txt").
void hash::write_all()
{
	ofstream out_file;
	out_file.open("cs163_notes.txt");
	if(out_file)
	{
		if(!hash_table)
			return;
		node * temp = NULL;	
		char _keyword[50];
		char _info[500];
		char _pages[50];
		for(int i = 0; i < table_size; ++i){
			temp = hash_table[i];
			if(temp){
				while(temp){
					temp->data.get_keyword(_keyword);
					out_file << _keyword << endl;
					temp->data.get_info(_info);
					out_file << _info << endl;
					out_file << temp->data.get_chapter() << endl;
					temp->data.get_pages(_pages);
					out_file << _pages << endl;
					out_file << boolalpha << temp->data.get_review() << endl;
					temp = temp->next;
				}
			}
		}
	}
	out_file.close();
}
