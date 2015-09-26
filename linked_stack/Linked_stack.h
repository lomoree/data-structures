#ifndef LINKED_STACK_H
#define LINKED_STACK_H

#ifndef nullptr
#define nullptr 0
#endif

#include "ece250.h"
#include "Double_sentinel_list.h"
#include "Exception.h"
#include <iostream>

template <typename Type>
class Linked_stack {
private:
	static int const ARRAY_CAPACITY = 8;

	Double_sentinel_list<Type *> list;
	int itop;
	int stack_size;

public:
	Linked_stack();
	Linked_stack(Linked_stack const &);
	~Linked_stack();

	bool empty() const;
	int size() const;
	int list_size() const;

	Type top() const;

	void swap(Linked_stack &);
	Linked_stack &operator=(Linked_stack);
	void push(Type const &obj);
	Type pop();

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Linked_stack<T> const &);
};

template <typename Type>
Linked_stack<Type>::Linked_stack() :
stack_size(0){
	itop = 7;
}

// Copy Constructor: Creates an identical copy of the pre-existing stack
template <typename Type>
Linked_stack<Type>::Linked_stack(Linked_stack const &stack) :
itop(7), 			// Set initial value for itop
stack_size(0) { 	// Set initial value for stack size
	
	// Iterate from the back of the list, pushing new items over from the old stack to the new stack
	for (Double_node<Type *> * temp = stack.list.tail()->previous(); temp->previous() != nullptr; temp = temp->previous()) {
		// For each array, copy each individual element over
		for (int i = 0; i < 8; i++) { 
			push(temp->retrieve()[i]);
		}
	}
	itop = stack.itop; 				// Set final value for itop
	stack_size = stack.stack_size; 	// Set final value for stack
}

// Destructor: Deallocates the linked stack from memory
template <typename Type>
Linked_stack<Type>::~Linked_stack() {
	Double_node < Type*> * temp = list.head()->next();
	// Iterates through the entire list deleting each array
	while (temp->next() != nullptr){
		delete [] temp->retrieve();
		temp = temp->next();
	}
	// Since the destructor deletes this instance of the class,
	// the list is autmatically deallocated from memory
}

// empty() function: Returns true if the stack is empty, false otherwise
template <typename Type>
bool Linked_stack<Type>::empty() const {
	return (stack_size == 0);
}

// Returns the current number of elements on the stack
template <typename Type>
int Linked_stack<Type>::size() const {
	return stack_size;
}

// Returns the size of the list
template <typename Type>
int Linked_stack<Type>::list_size() const {
	return list.size();
}

// Returns the element at the top of the stack
template <typename Type>
Type Linked_stack<Type>::top() const {
	if (empty()) // if the list is empty, throws an underflow error
		throw underflow();
	return list.head()->next()->retrieve()[itop];
}

template <typename Type>
void Linked_stack<Type>::swap(Linked_stack<Type> &stack) {
	std::swap(list, stack.list);
	std::swap(stack_size, stack.stack_size);
	std::swap(itop, stack.itop);
}

template <typename Type>
Linked_stack<Type> &Linked_stack<Type>::operator=(Linked_stack<Type> rhs) {
	swap(rhs);

	return *this;
}

// Pushes a new element to the top of the stack
template <typename Type>
void Linked_stack<Type>::push(Type const &obj) {
	// If the current array is full, push a new array to the front, then add the object to that array
	if (itop == 7){
		list.push_front(new Type[8]);
		list.front()[0] = obj;
		itop = 0; // Reset itop
	}
	// If there is room left in the current array, assign it to the next index of itop
	else{
		itop++;
		list.head()->next()->retrieve()[itop] = obj;
	}
	stack_size++;
}

template <typename Type>
Type Linked_stack<Type>::pop() {
	if (empty()) // Throw an underflow if the stack is empty
		throw underflow();
	// Retrieve the element to be popped
	Type temp = list.head()->next()->retrieve()[itop];
	// If there is only one element left in the current array,
	// delete the element, then deallocate that array from memory
	if (itop == 0){
		delete [] list.head()->next()->retrieve();
		list.pop_front();
		itop = 7;	
	}
	// If there is more than one element left in the current array,
	// Simply decrement itop
	else
		itop--;
	stack_size--; // Decrement size of the stack

	return temp; // Return the popped element
}

// You will be required to modify this function in order to accomodate
// your implementation of a singly linked list in Project 1.

template <typename T>
std::ostream &operator<<(std::ostream &out, Linked_stack<T> const &stack) {
	if (stack.list.size() == 0) {
		out << "->0";
	}
	else if (stack.list.size() == 1) {
		out << "->[ ";

		for (int i = 0; i <= stack.itop; ++i) {
			out << stack.list.front()[i] << " ";
		}

		out << "]->0";
	}
	else {
		out << "->";

		for (Double_node<T *> *ptr = stack.list.head()->next(); ptr->next() != nullptr; ptr = ptr->next()) {
			out << "[ ";

			if (ptr == stack.list.head()->next()) {
				for (int i = 0; i <= stack.itop; ++i) {
					out << ptr->retrieve()[i] << " ";
				}
			}
			else {
				for (int i = 0; i <= Linked_stack<T>::ARRAY_CAPACITY - 1; ++i) {
					out << ptr->retrieve()[i] << " ";
				}
			}

			out << "]->";
		}

		out << "0";
	}

	return out;
}
`
#endif
