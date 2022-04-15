#ifndef QUEUE_H
    #define QUEUE_H

    template<typename var> 
    class queue
    {

    private:

        var* q;
        int currentSize,capacity,head;

        void updateSize();


    public:
        queue();
        ~queue();
        queue(const queue<var>&);

        void push(var);
        var pop();
        
        var& front();
        int size();
        bool empty();
        void clear();



        queue<var>& operator=(queue<var>&);


    };
    



    #include "queue.cpp"

#endif