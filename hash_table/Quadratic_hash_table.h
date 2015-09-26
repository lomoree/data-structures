/*
* Quadratic hash table by Eric Lomore
* March 25th, 2015
*
* This class implements the member functions and variables of a Quadratic Hash Table ADT.
* This particular hash table implementation uses quadratic hashing/polling to reduce
* the run times of insertion, erase, and member operations.
* The hash table is implemented using a circular array as its container.
*
* ---------------------------------------------------------
*                   Member Functions (Accessors):
*
* int hash(Type const &) const
*   The hash function that is used to find an initial bin. 
*   Reduces the given object to a range of 0 to M-1 by taking the
*   object modulo M.
*
* int size() const
*   Returns the number of elements in the hash table.
*
* int capacity() const
*   Returns the size of the array holding the hash table elements.
*   The size of the array represents the max number of elements that can 
*   be stored in the hash table.
*
* double load_factor() const
*   Returns the ratio of erased + occupied bins to the total number of bins
*   This is because both erased and occupied bins have the same detrimental effect
*   on the run times of the hash table.
*
* bool empty() const
*   Returns true if the hash table is empty (contains no elements), false otherwise
*
* bool member(Type const &) const
*  Iterates through the hash table to determine if an element is in the hash table.
*  Given that this is a quadratic hash table, it uses the concept of quadratic polling
*  to find elements.
*
* Type bin(int) const
*  Returns the element at the given index.
*
* ---------------------------------------------------------
*                   Member Functions (Mutators):
*
* void insert(Type const &)
*   Takes an object passed as a parameter. Inserts the object as an element in the hash table.
*   Uses the hash function to find an initial bin to place the element in, and subsequently uses
*   quadratic polling if necessary to find the appropriate index for insertion.
*
* bool erase(Type const &)
*   Attempts to erase a matching element in the hash table. If the element is found and removed, 
*   the function returns true, otherwise it returns false.
*   Uses the hash function to find an initial bin as a best guess, and then quadratic polling
*   if necessary to find the element. If the object isn't in the hash table, this will run in order n time.
*
* Type clear()
*    Pops the top node of the heap
*    Returns the element in that node.
*
* void clear();
*    Clears the hash table by resetting all counter member variables, and setting the state of
*    each index to unoccupied.
*
*
* References: Douglas Wilhelm Harder for the formatting of this comment block
*/

#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

#ifndef nullptr
#define nullptr 0
#endif

#include "Exception.h"
#include "ece250.h"

enum bin_state_t { UNOCCUPIED, OCCUPIED, ERASED };

template <typename Type>
class Quadratic_hash_table {
private:
	int count;                     // Counter of bins marked with OCCUPIED
	int erasedcount;               // Counter of bins marked with ERASED
	int power;                     // A value of m that determines the size of the hash table
	int array_size;                // Size of hash table such that M = 2^m
	int mask;                      // Mask: Size of the hash table less one (M-1)
	Type *array;                   // Pointer to a circular array which holds the contents of the hash table.
	bin_state_t *occupied;         // Enumerator list of bin states; unoccipued, occupied, erased

	int hash(Type const &) const;

public:		
	Quadratic_hash_table(int = 5);
	~Quadratic_hash_table();
	int size() const;
	int capacity() const;
	double load_factor() const;
	bool empty() const;
	bool member(Type const &) const;
	Type bin(int) const;

	void print() const;

	void insert(Type const &);
	bool erase(Type const &);
	void clear();

	// Friends

	template <typename T>
	friend std::ostream &operator<<(std::ostream &, Quadratic_hash_table<T> const &);
};

template <typename Type>
Quadratic_hash_table<Type>::Quadratic_hash_table(int m) :
count(0), power(m),
array_size(1 << power),
mask(array_size - 1),
array(new Type[array_size]),
occupied(new bin_state_t[array_size]) {
	// Creates a new hash table where each bin is presently unoccupied
	for (int i = 0; i < array_size; ++i) {
		occupied[i] = UNOCCUPIED;
	}
}

template <typename Type>
Quadratic_hash_table<Type>::~Quadratic_hash_table(){
	// Deallocates arrays from memory
	delete[] array;
	delete[] occupied;
}

template <typename Type>
int Quadratic_hash_table<Type>::hash(Type const &obj) const{
	// Takes the object and casts it to an int
	int index = static_cast<int>(obj);
	// Since the object is represented by a numerical value, we take it modulo capacity
	index = index % capacity();
	// Since the modulus operation in C++ can result in a negative
	// We check for this case, and add back the capacity to ensure the index is on [0, capacity-1]
	if (index < 0) { index += capacity(); }
	// Returns the index of the bin to which the object is associated
	return index;

}

template <typename Type>
int Quadratic_hash_table<Type>::size() const {
	return count;

}

template <typename Type>
int Quadratic_hash_table<Type>::capacity() const {
	return array_size;

}

template <typename Type>
double Quadratic_hash_table<Type>::load_factor() const {
	// Returns the ratio of occupied and erased bins to the total
	// capacity of the hash table.
	return static_cast<double>(count+erasedcount)/capacity();

}

template <typename Type>
bool Quadratic_hash_table<Type>::empty() const {
	return (size() == 0);
}

template <typename Type>
bool Quadratic_hash_table<Type>::member(Type const &input) const{
	// Uses the hashing function to find a starting bin
	int index = hash(input);
	int i = 0;

	// Iterate through the array
	while (i < array_size){
		// If the index is occupied, its a candidate for a match
		if (occupied[index] == OCCUPIED){
			// If the value at the index is equivalent, return true
			if(array[index] == input){ return true; }
		}
		// Iterate using quadradic polling, then take the modulo
		// to utilize the circular nature of the array
		index += ++i;
		index = index % capacity();
		// Handle the edge case where the modulo results in a negative
		if (index < 0){ index += capacity(); }
	}
	// There were no matches found in the array, return false
	return false;

}


template <typename Type>
Type Quadratic_hash_table<Type>::bin(int i) const{
	// Returns the element at the given index
	return array[i];

}

template <typename Type>
void Quadratic_hash_table<Type>::print() const{
	return;
}

template <typename Type>
void Quadratic_hash_table<Type>::insert(Type const &obj) {
	// If table is full, throw overflow
	// If the object is already in the table, do nothing
	// Object can go into an empty bin, or deleted bin
	if (count == capacity()){ throw overflow(); }
	int index = hash(obj);
	int i = 0;

	// Look to see if the entry is already in the array
	while (i < array_size){
		if (array[index] == obj) {
			// If the object was erased, fix the counters
			if (occupied[index] == ERASED){
				erasedcount--;
				count++;
			}
			// If the object is in an unoccupied bin, fix the count
			if (occupied[index] == UNOCCUPIED){
				count++;
			}
			// In all cases, this bin will now be occupied with the object
			occupied[index] = OCCUPIED;
			return;
		}
		// Iterate using quadradic polling, then take the modulo
		// to utilize the circular nature of the array
		index += ++i;
		index = index % capacity();
		if (index < 0){ index += capacity(); }
	}
	
	// Reset index and count variables for second iteration
	index = hash(obj);
	i = 0;
	
	// We now know that the element isnt in the array
	// We must find a corresponding spot to place the element
	while (i < array_size){
		// If the bin is unoccupied we have a place for it
		if (occupied[index] != OCCUPIED){
			// If the bin is erased, erasedcount must be updated
			if (occupied[index] == ERASED){ 
				erasedcount--;
			}
			// Update the bin with the object, and in all cases
			// the bin should now be makred occupied.
			array[index] = obj;
			occupied[index] = OCCUPIED;
			count++;
			return;
		}
		// Iterate using quadradic polling, then take the modulo
		// to utilize the circular nature of the array
		index += ++i;
		index = index % capacity();
		if (index < 0){ index += capacity(); }
	}
}

template <typename Type>
bool Quadratic_hash_table<Type>::erase(Type const &obj){
	int index = hash(obj);
	int i = 0;

	// Iterate through the list, examine all occupied bins
	while (i < array_size){
		if (occupied[index] == OCCUPIED){
			// At each occupied bin, check if the contents match our object
			if (array[index] == obj){
				// If the contents match, the bin should be erased and counters updated
				occupied[index] = ERASED;
				erasedcount++;
				count--;
				// Since a match was found, return true
				return true;
			}
		}
		// Iterate using quadradic polling, then take the modulo
		// to utilize the circular nature of the array
		index += ++i;
		index = index % capacity();
		if (index < 0){ index += capacity(); }
	}

	// If we make it to this point no match was found in the entire hash table, return false
	return false;

}

template <typename Type>
void Quadratic_hash_table<Type>::clear(){
	count = 0;
	erasedcount = 0;
	for (int i = 0; i < array_size; ++i){
		occupied[i] = UNOCCUPIED;
	}
}


template <typename T>
std::ostream &operator<<(std::ostream &out, Quadratic_hash_table<T> const &hash) {
	for (int i = 0; i < hash.capacity(); ++i) {
		if (hash.occupied[i] == UNOCCUPIED) {
			out << "- ";
		}
		else if (hash.occupied[i] == ERASED) {
			out << "x ";
		}
		else {
			out << hash.array[i] << ' ';
		}
	}

	return out;
}

#endif
