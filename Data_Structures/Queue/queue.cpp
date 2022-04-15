#include "queue.h"





#pragma region CONSTRUCTOR_DESTRUCTOR



// Constructors
template<typename var>
queue<var>::queue(){
    q = new var[1];
    capacity = 1;
    currentSize = 0;
    head = 0;
}



// Copy Constructor
template <typename var>
queue<var>::queue(const queue<var>& other){
    capacity = other.capacity;
    currentSize = other.currentSize;
    head = 0;
	q = new var[capacity];
    for(int i = 0;i < currentSize;i++)
        q[i] = other.q[other.head + i];
}




// Destructor
template<typename var>
queue<var>::~queue(){
    delete[] q;
}

#pragma endregion


#pragma region METHODS


// Private Functions
template <typename var>
void queue<var>::updateSize(){
    if(capacity != currentSize)
		return;

	var* temp = new var[capacity * 2];
    int tempHead = head;
    head = 0;

	for(int i = 0;i < currentSize;i++)
		temp[i] = q[(tempHead + i) % capacity];

    capacity *= 2;
    delete[] q;
    
	q = temp;
}
	




// Interfacing Functions 

template<typename var>
void queue<var>::push(var elem){
    updateSize();
    q[(head + (currentSize++)) % capacity] = elem;
}


template<typename var>
var queue<var>::pop(){
    
	if(!currentSize)
		throw std::runtime_error("Empty Queue");
    

    var ret = q[head];
    head = (head + 1) % capacity;
    currentSize--;
    return ret;
}


template<typename var>
var& queue<var>::front(){
    
	if(!currentSize)
		throw std::runtime_error("Empty Queue");
    
    return q[head];
}


template<typename var>
int queue<var>::size(){
    return currentSize;
}


template<typename var>
bool queue<var>::empty(){
    return !currentSize;
}


template<typename var>
void queue<var>::clear(){
    delete[] q;
    q = new var[1];
    capacity = 1;
    currentSize = 0;
    head = 0;
}




#pragma endregion

#pragma region OPERATORS


template<typename var>
queue<var>& queue<var>::operator=(queue<var>& other){

    


    currentSize = other.currentSize;
    capacity = other.capacity;

    delete[] q;
    q = new var[capacity];

    head = 0;
    for(int i = 0;i < currentSize;i++){
        q[i] = other.q[(other.head + i) % capacity];
    }

    return *this;
}

#pragma endregion
