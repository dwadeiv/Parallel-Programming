#include "array.hpp"

Array::Array() {}

Array::~Array() {}

int* Array::resize(int* arrayPtr, int &capacity)
{
	// Increase the capacity by two times
	 int newCapacity = capacity * 2;

	// Dynamically allocate an array of size newCapacity
	 int *newArray = new int[newCapacity];

	// Copy all data from oldArray to newArray
	for(int i = 0; i < capacity; i ++){
		newArray[i] = arrayPtr[i];
	}

	// Free the memory associated with oldArray
	delete arrayPtr;

	capacity = newCapacity;

	// Return the newArray
	return newArray;

}