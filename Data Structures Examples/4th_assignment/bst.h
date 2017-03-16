// Brian Dunn
// CS 163
// Filename: bst.h
// Includes: notes.h
// Purpose: This file implements the Binary Searh Tree class. THe class has functions to insert, remove, edit info, display all, and display by range.
//			Notes are inserted on sorted order by keyword.

#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "notes.h"

// Node Struct
struct node
{
    notes data;
    node * left = NULL;
    node * right = NULL;
};
// Binary Search Trees Class
class bst
{

		public:
// Constructor and Destructor
			bst();
			~bst();
// Insert. Key value inserts notes object. Calls insert recursively.
			int insert(char* key, notes &to_add);
// Retrieve
			int retrieve(char* key, notes &to_get);
// Remove
			int remove(char * key);
// Display All
			int display();
// Display by Range	
			int display_range(char begin, char end);
// Edit existing Info
			int edit_info(char * key, char * info);
// Tree Height. Returns the height of the tree.
			int get_height();
// Node count. Returns the number of entries.
			int get_count();
		
		private:
		
// Private Data members
			node * root;
// Private functions. Called recursively by public functions
			int insert(char* key, notes &to_add, node * &root);
			int r_retrieve(char* key, notes &to_get, node * root);
			int remove(char* key, node * &root);
			void remove_all(node * &root);
			int display(node * root);
			int display_range(char begin, char end, node * root);
			int edit_info(char * key, char * info, node * root);
// Get In Order Successor. Returns pointer to IOS.
			node * get_ios(node * root);
			int get_height(node * root);
			int get_count(node * root);
};
