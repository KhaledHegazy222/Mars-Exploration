#include <algorithm>
#include "priorityQueue.h"


template<typename var>
int priorityQueue<var>::leftChild(int idx){
    return idx * 2 + 1;
}

template<typename var>
int priorityQueue<var>::rightChild(int idx){
    return idx * 2 + 2;
}

template<typename var>
int priorityQueue<var>::parent(int idx){
    return (idx - 1) / 2;
}



template<typename var>
void priorityQueue<var>::shiftUp(int idx){
    while(idx > 0 && heap[idx] > heap[parent(idx)]){
        std::swap(heap[idx],heap[parent(idx)]);
        idx = parent(idx);
    }

}

template<typename var>
void priorityQueue<var>::shiftDown(int idx){
    int left = leftChild(idx);
    int right = rightChild(idx);
    int largest = idx;
    if(left < heap.size() && heap[left] > heap[largest])
        largest = left;

    if(right < heap.size() && heap[right] > heap[largest])
        largest = right;

    if(largest != idx){
        std::swap(heap[largest],heap[idx]);
        shiftDown(largest);
    }
}


template<typename var>
void priorityQueue<var>::push(var elem){
    heap.pushBack(elem);
    shiftUp(heap.size() - 1);   
}

template<typename var>
var& priorityQueue<var>::top(){
    if(!heap.size())
        throw "Exception";

    return heap[0];
}


template<typename var>
var priorityQueue<var>::pop(){
    if(!heap.size())
        throw "Exception";
    var ret = heap[0];
    heap[0] = heap.back();
    heap.popBack();
    shiftDown(0);
    return ret;
}


template<typename var>
int priorityQueue<var>::size(){
    return heap.size();
}





