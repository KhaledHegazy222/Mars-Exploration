#include "statics.h"
int result::promotedNumber;
int result::executionTime;
int result::waitingTime;


void result::addPromotion(){
    promotedNumber++;
}

void result::addExecution(){
    executionTime += executionDays;
}
void result::addWaiting(){
    executionTime += waitingDays;
}


int  result::totalPromoted(){
    return promotedNumber;
}
int  result::totalExecution(){
    return executionTime;
}

int  result::totalWaiting(){
    return waitingTime;
}


bool result::operator<(const result& other){
    if(completeDay != other.completeDay)
        return completeDay < other.completeDay;
    return executionDays < other.executionDays;
}