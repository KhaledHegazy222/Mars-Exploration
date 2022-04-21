#ifndef STATICS_H
    #define STATICS_H

    
    class result
    {
        private:
            static int promotedNumber;
            static int executionTime;
            static int waitingTime;
        public:

            int completeDay;
            int ID;
            int formulationDay;
            int waitingDays;
            int executionDays;

            bool operator<(const result&);
            void addPromotion();
            void addExecution();
            void addWaiting();

            
            static int totalPromoted();
            static int totalExecution();
            static int totalWaiting();

    };


    



    #include "statics.cpp"



#endif 