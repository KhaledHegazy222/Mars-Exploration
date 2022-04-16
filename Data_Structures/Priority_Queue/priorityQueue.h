#ifndef PRIORITY_QUEUE_H
    #define PRIORITY_QUEUE_H


    #include "../Dynamic_Array/dynamicArray.h"


    template<typename var>
    class priorityQueue
    {   
        private:
            dynamicArray<var>heap;



            int leftChild(int);
            int rightChild(int);
            int parent(int);
            void shiftUp(int);
            void shiftDown(int);


        public:

            void push(var);
            var& top();
            var pop();              
            int size();

            
        

    };



    #include "priorityQueue.cpp"


#endif