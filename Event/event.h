#ifndef EVENT_H
    #define EVENT_H

    #include "../Configurations/configuration.h"


    enum eType {Formulation, Cancellation, Promotion};


    class event{
        public:
            eType eventType;
            union {  
                struct {
                    type missionType;
                    int formulationDay;
                    int ID;
                    double targetLocation;
                    int missionDuration;
                    int significance;
                }form;
                struct {
                    int cancellationDay;
                    int ID;
                }cancel;
                struct {
                    int promotionDay;
                    int ID;
                }promote;
                
            }data;



        int getDay();
    };
    

    #include "event.cpp"

#endif