#ifndef DYNAMIC_QUEUE_H
#define DYNAMIC_QUEUE_H

#ifndef nullptr
#define nullptr 0
#endif

#include <algorithm>
#include "ece250.h"
#include "Exception.h"

template <typename Type>
class Dynamic_queue {
private:
	int initial_capacity;
	int array_capacity;
	Type *array;
	int ihead;
	int itail;
	int entry_count;
	// other integer member variables, as necessary

public:
	Dynamic_queue(int = 10);
	Dynamic_queue(Dynamic_queue const &);
	~Dynamic_queue();

	Type head() const;
	int size() const;
	bool empty() const;
	int capacity() const;

	void swap(Dynamic_queue &);
	Dynamic_queue &operator=(Dynamic_queue);
	void enqueue(Type const &);
	Type dequeue();
	void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Dynamic_queue<T> const &);
};

template <typename Type>
Dynamic_queue<Type>::Dynamic_queue(int n) :
initial_capacity(std::max(n, 1)),
array_capacity(initial_capacity),
array(new Type[initial_capacity]),
ihead(0),
itail(initial_capacity - 1),
entry_count(0) {
}

template <typename Type>
Dynamic_queue<Type>::Dynamic_queue(Dynamic_queue const &queue) :
initial_capacity(queue.initial_capacity),
array_capacity(queue.array_capacity),
array(new Type[array_capacity]),
ihead(queue.ihead),
itail(queue.itail),
entry_count(queue.entry_count) {
	// The above initializations copy the values of the appropriate
	// member variables and allocate memory for the data structure;
	// however, you must still copy the stored objects.
	for (int i = 0; i < array_capacity; ++i)
		array[i] = queue.array[i];
}

// Destructor: Deallocates memory for the queue and its contents
template <typename Type>
Dynamic_queue<Type>::~Dynamic_queue() {
	// Iterates through the array
	// Deallocates memory of each element in the array (deletes each element)
	delete [] array;
}

// Returns the number of elements currently in the queue
template <typename Type>
int Dynamic_queue<Type>::size() const {
	return entry_count;
}

// Returns the queue's current capacity
template <typename Type>
int Dynamic_queue<Type>::capacity() const {
	return array_capacity;
}

// Returns true if the queue is empty
// and false if the queue has any elements
template <typename Type>
bool Dynamic_queue<Type>::empty() const {
	return entry_count == 0;
}

// Returns the element at the top of the queue
template <typename Type>
Type Dynamic_queue<Type>::head() const {
	if (entry_count == 0) // Throws an underflow if the queue is empty
		throw underflow();
	return array[ihead];
}

template <typename Type>
void Dynamic_queue<Type>::swap(Dynamic_queue<Type> &queue) {
	std::swap(initial_capacity, queue.initial_capacity);
	std::swap(array_capacity, queue.array_capacity);
	std::swap(array, queue.array);
	std::swap(ihead, queue.ihead);
	std::swap(itail, queue.itail);
	std::swap(entry_count, queue.entry_count);
}

template <typename Type>
Dynamic_queue<Type> &Dynamic_queue<Type>::operator=(Dynamic_queue<Type> rhs) {
	swap(rhs);

	return *this;
}

// Adds an element to the back of the queue
template <typename Type>
void Dynamic_queue<Type>::enqueue(Type const &obj) {
	// If the array is full, double the array size and copy all elements over
	if (entry_count == array_capacity){
		Type *temp_array = new Type[array_capacity * 2];
		// If the array is perfectly aligned, copy over.
		if (ihead == 0){
			for (int i = 0; i < array_capacity * 2; ++i)
				temp_array[i] = array[i];
		}
		// If the array is not perfectly aligned, we have to reindex elements
		// Since we know the array is full, and unaligned, itail < ihead.
		else{
			// Increment from the head to the end of the array.
			for (int i = ihead + array_capacity; i < array_capacity * 2; ++i){
				temp_array[i] = array[i - array_capacity];
			}
			// Increment from 0 to the tail. Leaving a gap in the middle.
			for (int i = 0; i <= itail; ++i){
				temp_array[i] = array[i];
			}
			ihead = ihead + array_capacity;
		}

		delete[] array;			// Deallocate the old array from memory
		array = temp_array;		// Assign the array to the newly created array
		array_capacity *= 2;	// The array is now double it's old capacity
	}
	if (itail != array_capacity - 1){
		array[itail + 1] = obj;
		itail++;
	}
	else{
		array[0] = obj;
		itail = 0;
	}
	entry_count++;
}

// Removes the element from the front of the queue
template <typename Type>
Type Dynamic_queue<Type>::dequeue() {
	if (empty()) // If the queue is empty, throw an underflow
		throw underflow();
	Type temp = array[ihead]; // Retrieve element at the head of the queue
	
	// If the head is not the last element in the array, increment
	if (ihead != array_capacity - 1)
		ihead++;
	// If the head is the last element in the array, the head will be back at the beginning of the array
	else
		ihead = 0;
	// Since we are dequeueing, the total number of entires decreases by one
	entry_count--;
	// If the number of entries is less than or equal to 4, and array capacity > initial capacity
	if (entry_count <= array_capacity / 4 && array_capacity > initial_capacity){
		// Create a new array of half the size
		Type *temp_array = new Type[array_capacity / 2];
		int k = 0;
		// Starting from the original head position, 
		// iterate through the old array and copy elements over til
		// the end of the array is reached
		for (int i = ihead; i < array_capacity; ++i){
			temp_array[k] = array[i];
			k++;
			}
		if (itail < ihead){
			for (int i = 0; i <= itail; ++i)
			{
				temp_array[k] = array[i];
				k++;
			}
		}
		itail = k;
		array_capacity /= 2;
	}
	return temp;
}


template <typename Type>
void Dynamic_queue<Type>::clear() {
	array_capacity = initial_capacity;
	delete[] array;
	array = new Type[array_capacity];
	ihead = 0;
	itail = 0;
	entry_count = 0;
}

template <typename Type>
std::ostream &operator<<(std::ostream &out, Dynamic_queue<Type> const &queue) {
	// TODO
	return out;
}

#endif
