/*
* Leftist_heap by Eric Lomore
* March 10th, 2015
*
* This class implements the functions of a Leftist Heap ADT
*
* ---------------------------------------------------------
*                           Member Variables:
*
*  Leftist_node<Type> *root_node     This variable is a pointer to the root node of the heap
*						             If the heap is empty, this variable should have value nullptr
*
*  int                heap_size      This variable contains the size of the heap
*                                    Restrictions: can never be negative
*
* ---------------------------------------------------------
*                   Member Functions (Accessors):
*
* bool empty() const       
*   This function checks if the heap is empty.
*   Returns true if it is, otherwise false.
*
* int size() const
*   This function returns the size of the heap using member variable heap_size.
*
* int null_path_length() const
*   Returns the null path length of the heap 
*
* Type top() const
*   Returns the root node, or top, of the heap using member variable root_node.
*    
* int count(Type const &) const
*   Takes in a variable of type Type and looks for total matches in the heap.
*   The total number of matches is returned as an integer.
*
* ---------------------------------------------------------
*                   Member Functions (Mutators):
*
* void postorder_push(Leftist_node<Type>*);
*    Acts as a helper method of the copy constructor.
*    Takes in a node, and does a post order traversal starting at that node.
*    Each node in the traversal is pushed onto the heap using the push mutator function
* 
* void push(Type const &);
*    Pushes the object passed onto the heap
*
* Type pop();
*    Pops the top node of the heap
*    Returns the element in that node.
*
* void clear();
*    Clears the heap, resets the size of the heap, and sets the root node to a nullptr
*
*
* References: Douglas Wilhelm Harder for the formatting of this comment block
*/

#ifndef LEFTIST_HEAP_H
#define LEFTIST_HEAP_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Leftist_node.h"

template <typename Type>
class Leftist_heap {
private:
	// Member variables
	Leftist_node<Type> *root_node;
	int heap_size;

public:
	// Constructors/Destructor
	Leftist_heap();
	Leftist_heap(Leftist_heap const &);
	~Leftist_heap();

	// Supplementary functions
	void swap(Leftist_heap &heap);
	Leftist_heap &operator=(Leftist_heap);

	// Accessors
	bool empty() const;
	int size() const;
	int null_path_length() const;
	Type top() const;
	int count(Type const &) const;

	// Mutators
	void postorder_push(Leftist_node<Type>*);
	void push(Type const &);
	Type pop();
	void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Leftist_heap<T> const &);
};

template <typename Type>
Leftist_heap<Type>::Leftist_heap() :
root_node(nullptr),
heap_size(0) {
	// does nothing
}

template <typename Type>
Leftist_heap<Type>::Leftist_heap(Leftist_heap const &heap) :
root_node(nullptr),
heap_size(0) {
	// If the heap is empty, we need not copy any elements over
	if (heap.empty()) return;
	postorder_push(heap.root_node);
}

template <typename Type>
void Leftist_heap<Type>::postorder_push(Leftist_node<Type>* node) {
	// Complete a depth first post order order traverse recursively
	if (node->left() != nullptr) postorder_push(node->left());
	if (node->right() != nullptr) postorder_push(node->right());
	// Push the element at the node onto the heap
	push(node->retrieve());
}

template <typename Type>
Leftist_heap<Type>::~Leftist_heap() {
	clear();  // might as well use it...
}

template <typename Type>
void Leftist_heap<Type>::swap(Leftist_heap<Type> &heap) {
	// Swaps the root node and heap size of the current instance
	// with the heap passed as an argument
	std::swap(root_node, heap.root_node);
	std::swap(heap_size, heap.heap_size);
}

template <typename Type>
Leftist_heap<Type> &Leftist_heap<Type>::operator=(Leftist_heap<Type> rhs) {
	swap(rhs);

	return *this;
}

// Accessor Functions
template <typename Type>
bool Leftist_heap <Type>::empty() const{
	return heap_size == 0;
}

template <typename Type>
int Leftist_heap<Type>::size() const{
	return heap_size;
}

template <typename Type>
int Leftist_heap<Type>::null_path_length() const{
	return root_node->null_path_length();
}

template <typename Type>
Type Leftist_heap<Type>::top() const{
	// If the stack is empty, throw an underflow
	if (empty()) throw underflow();
	// Otherwise, return the element at the root node
	return root_node->retrieve();
}

template <typename Type>
int Leftist_heap<Type>::count(Type const &obj) const {
	// Count the instances of the object within the heap
	// Call the count() method of the Leftist_node class
	// as it will begin the conting, traversing downwards from the root node
	return root_node->count(obj);
}

// Mutators
template <typename Type>
void Leftist_heap<Type>::push(Type const &obj) {
	root_node->push(new Leftist_node<Type>(obj), root_node);
	heap_size++;
}

template <typename Type>
Type Leftist_heap<Type>::pop() {
	// The tree is empty: throw an underflow exception
	if (empty()) { throw underflow(); };

	// Grab the current root node
	Leftist_node<Type> *temp = root_node;

	// Make the left tree the new root node, and push the right tree onto the new root
	root_node = root_node->left();
	root_node->push(temp->right(), root_node);

	// Cleanup: get the return value, and then delete the popped node
	Type returnval = temp->retrieve();
	delete temp;
	temp = nullptr;

	heap_size--;
	return returnval;

}

template <typename Type>
void Leftist_heap<Type>::clear(){
	// Clear the root node using the clear() function of Leftist_node
	root_node->clear();
	// Cleanup: set the root node to a nullptr and change the heap size to 0 (empty)
	root_node = nullptr;
	heap_size = 0;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, Leftist_heap<T> const &heap) {
	return out;
}

#endif
