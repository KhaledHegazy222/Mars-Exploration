#include <iostream>
#include <exception>
#include "dynamicArray.h"



#pragma region CONSTRUCTOR_DESTRUCTOR

// Constructors
template <typename var>
dynamicArray<var>::dynamicArray(){
	vec = new var[1];
	capacity = 1;
	currentSize = 0;
}


template <typename var>
dynamicArray<var>::dynamicArray(int size){

	if(size < 0)
		throw std::length_error("Bad Length Exception");


	vec = new var[size];
	capacity = size;
	currentSize = size;
	for(int i = 0;i < size;i++)
		vec[i] = 0;
}


template <typename var>
dynamicArray<var>::dynamicArray(int size,var value){

	if(size < 0)
		throw std::length_error("Bad Length Exception");

	capacity = 1;
	while(capacity < size)
		capacity <<= 1;

	vec = new var[capacity];
	currentSize = size;
	for(int i = 0;i < size;i++)
		vec[i] = value;
}



// Copy Constructor
template <typename var>
dynamicArray<var>::dynamicArray(const dynamicArray<var>& other){
    capacity = other.capacity;
    currentSize = other.currentSize;
	vec = new var[capacity];
    for(int i = 0;i < currentSize;i++)
        vec[i] = other.vec[i];
}


// Destructor
template <typename var>
dynamicArray<var>::~dynamicArray(){
    delete[]vec;
}

#pragma endregion


#pragma region METHODS


// Private Methods 
template <typename var>
void dynamicArray<var>::updateSize(){

	if(capacity != currentSize)
		return;

	capacity *= 2;

	var* temp = new var[capacity];
	for(int i = 0;i < currentSize;i++)
		temp[i] = vec[i];
    delete[] vec;
	vec = temp;
    
}

// Public Methods

template <typename var>
var& dynamicArray<var>::front(){
	if(!currentSize)
		throw std::runtime_error("Empty Dynamic Array");
	return vec[0];
}

template <typename var>
var& dynamicArray<var>::back(){
	if(!currentSize)
		throw std::runtime_error("Empty Dynamic Array");
	return vec[currentSize - 1];
}

template <typename var>
void dynamicArray<var>::pushBack(var elem){
	updateSize();
	vec[currentSize++] = elem;
}


template <typename var>
void dynamicArray<var>::popBack(){
	if(!currentSize)
		throw std::runtime_error("Empty Dynamic Array");
	currentSize--;
}

template <typename var>
int dynamicArray<var>::size(){
	return currentSize;
}


template <typename var>
void dynamicArray<var>::clear(){
	delete[]vec;
	vec = new var[1];
	capacity = 1;
	currentSize = 0;
}

template <typename var>
void dynamicArray<var>::resize(int size){
	delete[]vec;
	vec = new var[size];
	capacity = size;
	currentSize = size;

	
}



template <typename var>
var* dynamicArray<var>::begin(){
	return vec;
}

template <typename var>
var* dynamicArray<var>::end(){
	return vec + currentSize;
}


#pragma endregion


#pragma region OPERATORS

// Operators 
template<typename var>
var& dynamicArray<var>::operator[](int idx){
	if(idx >= currentSize)
		throw std::	out_of_range("Out of Range Exception");
	return vec[idx];	
}

template<typename var>
dynamicArray<var> dynamicArray<var>::operator=(dynamicArray<var> other){
	

	currentSize = other.currentSize;
	capacity = other.capacity;

    delete[] vec;
	vec = new var[capacity];
	
	for(int i = 0;i < currentSize;i++)
		vec[i] = other[i];

	return *this;
}

#pragma endregion