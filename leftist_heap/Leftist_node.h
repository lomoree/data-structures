/*
* Leftist_node by Eric Lomore
* March 10th, 2015
*
* This class implements the functions of for a Leftist node
* This class will primarily be used by the Leftist_heap class
* The objects in this class act as nodes, or storage elements
* for the Leftist_heap
*
* ---------------------------------------------------------
*                           Member Variables:
*
*  Type element;                     The element or data stored within the node
*
*  Leftist_node *left_tree           Pointer to the left child of this node (left tree)
*  Leftist_node *right_tree          Pointer to the right child of this node (right tree)
*
*  int heap_null_path_length         Stores the null path length of the node
*                                    This element is always >= 0
*
* ---------------------------------------------------------
*                   Member Functions (Accessors):
*
* Type retrieve() const;
*   Retrieves the element member variable
* bool empty() const;
*   Returns true if the node is a nullptr
*
* Leftist_node *left() const;
*   Returns a pointer to the left sub tree of the node
* Leftist_node *right() const;
*   Returns a pointer to the right sub tree of the node
*
* int count(Type const &) const;
*   Searches through the current node and its sub trees
*   to find the total matches between the passed object and
*   the member variable, element.
*
* int null_path_length() const;
*   Returns the null path length of the current node
*
* void inorder_traversal(Leftist_node *) const;
*   Testing function, completes an inorder traversal
*   and prints the element member variable for each node
*
* ---------------------------------------------------------
*                   Member Functions (Mutators):
*	
* void push(Leftist_node *, Leftist_node *&);
*   Push the tree in argument 1 onto the right side of the tree in argument 2.
*   Given the restrictions of a leftist heap, if this is not possible, adjust the 
*   structure of the heap and attempt the push again
*
* void clear();
*   Clear the node and all of its descendents. Set all cleared nodes to nullptr.
*

*/
#ifndef LEFTIST_NODE_H
#define LEFTIST_NODE_H

#include <algorithm>

#ifndef nullptr
#define nullptr 0
#endif

template <typename Type>
class Leftist_node {
private:
	// Member variables
	Type element;
	Leftist_node *left_tree;
	Leftist_node *right_tree;
	int heap_null_path_length;

public:
	// Constructor
	Leftist_node(Type const &);

	// Accessors
	Type retrieve() const;
	bool empty() const;
	Leftist_node *left() const;
	Leftist_node *right() const;
	int count(Type const &) const;
	int null_path_length() const;
	void inorder_traversal(Leftist_node *) const;

	// Mutators
	void push(Leftist_node *, Leftist_node *&);
	void clear();
};

template <typename Type>
Leftist_node<Type>::Leftist_node(Type const &obj) :
element(obj),
left_tree(nullptr),
right_tree(nullptr),
heap_null_path_length(0) {
	// does nothing
}

template <typename Type>
Type Leftist_node<Type>::retrieve() const{
	return element;
}

template <typename Type>
bool Leftist_node<Type>::empty() const {
	return (this == nullptr);
}

template <typename Type>
Leftist_node<Type>* Leftist_node<Type>::left() const{
	return left_tree;
}

template <typename Type>
Leftist_node<Type>* Leftist_node<Type>::right() const{
	return right_tree;
}

template <typename Type>
int Leftist_node<Type>::count(Type const &obj) const{
	int count = 0;		// Initialize counter variable
	if (element == obj) // If the current node's element matches the object passed, increment count
		count++;

	if (left_tree != nullptr)            // If the left tree exists, execute conditional
		count += left_tree->count(obj);  // Recursively look for more matches in left sub tree
	if (right_tree != nullptr)			 // If the right tree exists, execute conditional
		count += right_tree->count(obj); // Recursively look for more matches in the right sub tree

	return count;	// Return the total matches for the passed object.
}

template <typename Type>
int Leftist_node<Type>::null_path_length() const{
	if (empty()) { return -1; }		// If the node is empty, return -1.
	return heap_null_path_length;	// Otherwise, return the null path length as desired
}

template <typename Type>
void Leftist_node<Type>::push(Leftist_node *new_heap, Leftist_node *&ptrtothis){

	// If new heap is empty, exit. (precondition exit)
	if (new_heap->empty()) return;					
	// If the current node is empty, new heap is now the new node. (recursive exit)
	if (ptrtothis->empty()){ ptrtothis = new_heap; return; }

	// If the new heap is greater than or equal to the root node
	// then the new heap should just be pushed onto the right sub tree, recursively.
	if (new_heap->retrieve() >= ptrtothis->retrieve()){
		push(new_heap, ptrtothis->right_tree);

		// Update the null path length of the min of the right and left trees,
		// plus one to account for the new element being pushed onto the tree.
		ptrtothis->heap_null_path_length = std::min(ptrtothis->left()->null_path_length(), ptrtothis->right()->null_path_length()) + 1;
		
		// If the left tree is shorter than the right tree, swap them
		if (ptrtothis->left()->null_path_length() < ptrtothis->right()->null_path_length()){
			std::swap(ptrtothis->left_tree, ptrtothis->right_tree);
		}
	}
	// If the new heap is less than the root node,
	// then the root node should be pushed onto the right sub tree of the new node.
	else{
		Leftist_node *temp = ptrtothis;
		ptrtothis = new_heap;
		push(temp, ptrtothis);

	}
	return;
}

template <typename Type>
void Leftist_node<Type>::inorder_traversal(Leftist_node *node) const{
	if (node->left_tree != nullptr) inorder_traversal(node->left_tree);
	std::cout << "Nullpath: " << node->null_path_length() << "    Node:" << node->retrieve() << std::endl;
	if (node->right_tree != nullptr) inorder_traversal(node->right_tree);
}
template <typename Type>
void Leftist_node<Type>::clear(){
	// If the node we are attempting to clear doesn't exist, return
	if (this == nullptr)
		return;

	// Recursively clear the subtrees and set them to null
	left_tree->clear();
	left_tree = nullptr;
	right_tree->clear();
	right_tree = nullptr;

	// Delete the root node.
	// The function calling this method should set the root node to null
	delete this;
	
}



#endif
