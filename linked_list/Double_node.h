/*
* Double_node by Eric Lomore
* January 28th, 2015
*
* This class implements doubly linked nodes
* for use by the Double_sentinel_list class
*
*/

#ifndef DOUBLE_NODE_H
#define DOUBLE_NODE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"

template <typename Type>
class Double_list;

template <typename Type>
class Cyclic_double_list;

template <typename Type>
class Double_sentinel_list;

template <typename Type>
class Cyclic_double_sentinel_list;

template <typename Type>
class Double_node {
private:
	Type       element;
	Double_node *previous_node;
	Double_node *next_node;

public:
	Double_node(Type const & = Type(), Double_node * = nullptr, Double_node * = nullptr);

	Type retrieve() const;
	Double_node *previous() const;
	Double_node *next() const;

	friend class Double_list<Type>;
	friend class Cyclic_double_list<Type>;
	friend class Double_sentinel_list<Type>;
	friend class Cyclic_double_sentinel_list<Type>;

	// if ptr is a pointer to a Double_node<Type> object
	// in one of the friendly classes, you should:
	//    use   ptr->next_node   to modify it
	//    use   ptr->next()      to access it
};

template <typename Type>
Double_node<Type>::Double_node(Type const &e, Double_node<Type> *p, Double_node<Type> *n) :
element(e),
previous_node(p),
next_node(n) {
	// empty constructor
}

// Return the element at the node
template <typename Type>
Type Double_node<Type>::retrieve() const {
	return element;
}

// Return a pointer to the previous node.
template <typename Type>
Double_node<Type> *Double_node<Type>::previous() const {
	return previous_node;
}

// Return a pointer to the next node
template <typename Type>
Double_node<Type> *Double_node<Type>::next() const {
	return next_node;
}

#endif
