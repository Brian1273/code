// Brian Dunn
// CS 163
// Filename: bst.cpp
// Uses: bst.h
// Purpose: This is the implementation file for the binary search tree class. The tree uses notes objects as it's data members and allows insertion, removal, edit and display functions.

#include "bst.h"

using namespace std;

// Constructor
bst::bst()
{
		root = NULL;
}
// Destructor
bst::~bst()
{
	if(!root)
		return;
	else remove_all(root);		
}
// Helper Function for Destructor.
void bst::remove_all(node * &root)
{
	if(!root)
		return;
	remove_all(root->left);
	remove_all(root->right);
	delete root;
	return;
}
// Insert. Calls recursive insert. Returns 1 if inserted. O if not.
int bst::insert(char * key, notes &to_add)
{
		return(insert(key, to_add, root));
}
// Insert. Copies notes to list by key. Returns 1 if added.
int bst::insert(char * key, notes &to_add, node * &root)
{
	if(!root){
		root = new node;
		if(root->data.copy(to_add)){
			return 1;
		}	
		else return 0;
	}
	char  _keyword[50];
	root->data.get_keyword(_keyword);
	if(strcmp(key, _keyword) < 0)
		return (insert(key, to_add, root->left));
	else return (insert(key, to_add, root->right));
}	
// Remove. Removes an entry by keyword. returns 1 if removed. 0 if not found.
int bst::remove(char * key)
{
	return remove(key, root);
}
int bst::remove(char * key, node * &root)
{
	// Case 1. (root is NULL)
	if(!root)
		return 0;
	char _keyword[50];
	root->data.get_keyword(_keyword);
	int compare = strcmp(key, _keyword);
	if(compare == 0){
		node * temp = root;
	// Case 2.(Node is Leaf)
		if((!root->left) & (!root->right)){
			root = NULL;
			delete temp;
			return 1;
		}
	// Case 3(a&b).(Node is parent with 1 child.)
		else if(!root->left){
			root = root->right;
			delete temp;
			return 1;
		}
		else if(!root->right){
			root = root->left;
			delete temp;
			return 1;
		}
	// Case 4. (Node is internal with 2 children)
		else{
			node * ios = get_ios(root->right);
			char _keyword[50];
			ios->data.get_keyword(_keyword);
			root->data.remove();
			if(!root->data.copy(ios->data))
				return 0;
			return(remove(_keyword, root->right));
			
		}
	}
	else if(compare < 0)	// If less than key go left. Otherwise go right.
		return(remove(key, root->left));
	else return(remove(key, root->right));	
}
// Display All. Displays all entries in Keyword Order. Returns 0 if nothing to display.
int bst::display()
{
	return (display(root));
}
int bst::display(node * root)
{
	if(!root)
		return 0;
	display(root->left);
	root->data.display();
	display(root->right);
	return 1;
}
// Get In Order Successor. Returns a pointer to the IOS.
node * bst::get_ios(node * root)
{
	if(!root->left)
		return root;
	else return(get_ios(root->left));	
}
// Display by Range. Takes a beginning and ending character as a range for search. Returns 1 if successful. Returns 0 if nothing to display.
int bst::display_range(char begin, char end)
{
	return(display_range(begin, end, root));
}
int bst::display_range(char begin, char end, node * root)
{
	if(!root)
	 return 0;
	display_range(begin, end, root->left);
	char _keyword[50];
	root->data.get_keyword(_keyword);
	if((begin <= _keyword[0]) & (_keyword[0] <= end))
		root->data.display();
	display_range(begin, end, root->right);
	return 1;	
}
// Retrieve. Searches by key for entry. If found, fills bucket notes and returns 1. Returns 0 if not found.
int bst::retrieve(char * key, notes &to_get)
{
	return(r_retrieve(key, to_get, root));
	return 0;
}
int bst::r_retrieve( char * key, notes &to_get,  node * root)
{
	if(!root)
		return 0;
	char  _keyword[50];
	root->data.get_keyword(_keyword);
	if(strcmp(key, _keyword) < 0)
		return(r_retrieve(key, to_get, root->left));
	else if(strcmp(key, _keyword) == 0){
		if(to_get.copy(root->data))
			return 1;
	}		
	else return(r_retrieve(key, to_get, root->right));		
	return 0;
}
// Edit Info. Finds matching entry by key and appends to existing notes. Returns 1 if successful. 0 if it fails.
int bst::edit_info(char * key, char * info)
{
	return (edit_info(key, info, root));
}
int bst::edit_info(char * key, char * info, node * root)
{
	if(!root)
		return 0;
	char  _keyword[50];
	root->data.get_keyword(_keyword);
	if(strcmp(key, _keyword) < 0)
		return(edit_info(key, info, root->left));
	else if(strcmp(key, _keyword) == 0){
		if(root->data.add_info(info))
			return 1;
	}		
	else return(edit_info(key, info, root->right));		
	return 0;
}
// Get Height. Returns the Height of the BST.
int bst::get_height()
{
	return(get_height(root));
}
int bst::get_height(node * root)
{
	if(!root)
		return 0;
	return(1 + (max(get_height(root->left), get_height(root->right))));	
}
// Get Count. Returns number of Entries in Tree.
int bst::get_count()
{
	return(get_count(root));
}
int bst::get_count(node * root)
{
	if(!root)
		return 0;
	// FOR TESTING and evaluation.
//	int left = get_count(root->left);
//	int right = get_count(root->right);
//	cout << "\nLeft Children: " << left << endl;
//	cout << "\nRight Children: " << right << endl;
//	return(1 + left + right);
	return(1 + (get_count(root->left)) + (get_count(root->right)));	
}
