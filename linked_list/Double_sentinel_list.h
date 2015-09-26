/*
* Double_sentinel_list by Eric Lomore
* January 28th, 2015
*
* This class implements the functions for a doubly linked list
* with sentinel nodes
* It includes methods to traverse, find, delete, pop, and push
* elements onto the list
*
*/

#ifndef DOUBLE_SENTINEL_LIST_H
#define DOUBLE_SENTINEL_LIST_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Double_node.h"
#include "Exception.h"

template <typename Type>
class Double_sentinel_list {
private:
	Double_node<Type> *list_head;
	Double_node<Type> *list_tail;
	int list_size;

public:
	Double_sentinel_list();
	Double_sentinel_list(Double_sentinel_list const &);
	~Double_sentinel_list();

	// Accessors

	int size() const;
	bool empty() const;

	Type front() const;
	Type back() const;

	Double_node<Type> *head() const;
	Double_node<Type> *tail() const;

	int count(Type const &) const;

	// Mutators

	void swap(Double_sentinel_list &);
	Double_sentinel_list &operator=(Double_sentinel_list const &);

	void push_front(Type const &);
	void push_back(Type const &);

	Type pop_front();
	Type pop_back();

	int erase(Type const &);

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Double_sentinel_list<T> const &);
};

// Constructor: Create an empty list with a sentinel head and tail that point to each other.
template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list() :
list_head(new Double_node<Type>()),
list_tail(new Double_node<Type>()),
list_size(0) {
	list_head->next_node = list_tail;
	list_tail->previous_node = list_head;
}

// Copy Constructor: Take in a list, and create a separate copy of it.
template <typename Type>
Double_sentinel_list<Type>::Double_sentinel_list(Double_sentinel_list<Type> const &list) :
list_head(new Double_node<Type>()),
list_tail(new Double_node<Type>()),
list_size(list.size()) {

	// Point the sentinel head and tail nodes to each other.
	list_head->next_node = list_tail;
	list_tail->previous_node = list_head;
	
	// Iterate through the list and continuously add nodes to the back of the list.
	for (
		Double_node<Type> *temp = list.head()->next();

		temp->next() != 0;
		temp = temp->next()
		) {
		push_back(temp->retrieve());
	}
}

// Destructor: Delete/deallocate all nodes from memory thus deleting the list.
template <typename Type>
Double_sentinel_list<Type>::~Double_sentinel_list() {
	Double_node<Type> *temp = list_head->next_node;
	
	// Iterate through the list and delete the nodes.
	while (temp != 0)
	{
		delete temp->previous_node;
		temp = temp->next_node;
	}
	delete list_tail;
	return;
}

// Returns the size of the list
template <typename Type>
int Double_sentinel_list<Type>::size() const {
	return list_size;
}

// Checks if the list is empty. Returns true if it is, otherwise false.
template <typename Type>
bool Double_sentinel_list<Type>::empty() const {
	return (list_size == 0);
}

// Returns the element of the node after the head
template <typename Type>
Type Double_sentinel_list<Type>::front() const {
	// Throw an underflow if the list is empty.
	if (list_size == 0)
		throw underflow();
	return list_head->next()->retrieve();
}

// Returns the element of the node before the head.
template <typename Type>
Type Double_sentinel_list<Type>::back() const {
	// Throw an underflow if the list is empty.
	if (list_size == 0)
		throw underflow();
	return list_tail->previous()->retrieve();
}

// Return the head node.
template <typename Type>
Double_node<Type> *Double_sentinel_list<Type>::head() const {
	return list_head;
}

// Return the tail node.
template <typename Type>
Double_node<Type> *Double_sentinel_list<Type>::tail() const {
	return list_tail;
}

/* Take in an object, and look for all matches within the list.
Return a count of the total number of matching objects. */
template <typename Type>
int Double_sentinel_list<Type>::count(Type const &obj) const {
	Double_node<Type> *temp = list_head->next();
	int count = 0;
	while (temp->next() != 0)
	{
		if (temp->retrieve() == obj)
			count++;
		temp = temp->next_node;
	}
	return count;
}

// Swaps two existing lists with each other, including all nodes inside.
template <typename Type>
void Double_sentinel_list<Type>::swap(Double_sentinel_list<Type> &list) {
	std::swap(list_head, list.list_head);
	std::swap(list_tail, list.list_tail);
	std::swap(list_size, list.list_size);
}

template <typename Type>
Double_sentinel_list<Type> &Double_sentinel_list<Type>::operator=(Double_sentinel_list<Type> const &rhs) {
	Double_sentinel_list<Type> copy(rhs);

	swap(copy);

	return *this;
}

// Insert a new node at the beginning of the list (after the list head sentinel node).
template <typename Type>
void Double_sentinel_list<Type>::push_front(Type const &obj) {
	Double_node<Type> *temp = new Double_node<Type>(obj, list_head, list_head->next());

	// Set next and previous node pointers of the surrounding nodes.
	list_head->next_node = temp;
	temp->next()->previous_node = temp;

	// Increment the size of the list
	list_size++;
}

// Insert a new node at the end of the list (before the list tail sentinel node).
template <typename Type>
void Double_sentinel_list<Type>::push_back(Type const &obj) {
	Double_node<Type> *temp = new Double_node<Type>(obj, list_tail->previous(), list_tail);

	// Set next and previous node pointers of the surrounding nodes.
	list_tail->previous_node = temp;
	temp->previous()->next_node = temp;

	// Increment the size of the list
	list_size++;
}

// Remove the node at the front of the list (after the list head sentinel node).
template <typename Type>
Type Double_sentinel_list<Type>::pop_front() {
	if (list_size == 0)
		throw underflow();
	Double_node<Type> *temp = list_head->next();

	// Fix next and previous node pointers before deletion
	list_head->next_node = temp->next();
	list_head->next()->previous_node = list_head;

	// Set the return element, delete the node, and return the element.
	Type tempreturn = temp->retrieve();
	delete temp;
	list_size--;
	return tempreturn;
}

// Remove the node at the end of the list (before the list tail sentinel node).
template <typename Type>
Type Double_sentinel_list<Type>::pop_back() {
	if (list_size == 0)
		throw underflow();
	Double_node<Type> *temp = list_tail->previous();

	// Fix next and previous node pointers before deletion
	list_tail->previous_node = temp->previous();
	list_tail->previous()->next_node = list_tail;

	// Set the return element, delete the node, and return the element.
	Type tempreturn = temp->retrieve();
	delete temp;
	list_size--;
	return tempreturn;
}

// Erase the first node that contains the matching object passed to the method
template <typename Type>
int Double_sentinel_list<Type>::erase(Type const &obj) {

	// Iterare through the list
	for (Double_node<Type> *temp = list_head->next();
		temp->next() != 0;
		temp = temp->next()) 
	{
		// Look for a match
		if (obj == temp->retrieve())
		{
			// If a match is found, fix the next and previous node pointers
			temp->previous()->next_node = temp->next();
			temp->next()->previous_node = temp->previous();
			// Delete the node, decrement the size of the list
			delete temp;
			list_size--;
			// Return 1, indicating a match was found
			return 1;
		}
	}
	return 0;
}


template <typename T>
std::ostream &operator<<(std::ostream &out, Double_sentinel_list<T> const &list) {
	out << "head";

	for (Double_node<T> *ptr = list.head(); ptr != nullptr; ptr = ptr->next()) {
		if (ptr == list.head() || ptr == list.tail()) {
			out << "->S";
		}
		else {
			out << "->" << ptr->retrieve();
		}
	}

	out << "->0" << std::endl << "tail";

	for (Double_node<T> *ptr = list.tail(); ptr != nullptr; ptr = ptr->previous()) {
		if (ptr == list.head() || ptr == list.tail()) {
			out << "->S";
		}
		else {
			out << "->" << ptr->retrieve();
		}
	}

	out << "->0";

	return out;
}

#endif
