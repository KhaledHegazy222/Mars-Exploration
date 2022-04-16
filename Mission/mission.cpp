#include "mission.h"

int mission::EmergencyMissions;
int mission::MountainousMissions;
int mission::PolarMissions;




mission::mission(type missionType,int formulationDay,double targetLocation,int missionDuration,int significance,int ID) : 
    missionType(missionType), 
    formulationDay(formulationDay), 
    targetLocation(targetLocation), 
    missionDuration(missionDuration), 
    significance(significance), 
    ID(ID) {}



void mission::finishMission(){
    switch (missionType)
    {
    case Emergency:
        EmergencyMissions++;    
        break;
    case Mountainous:
        MountainousMissions++;
        break;

    case Polar:
        PolarMissions++;
        break;
    }
}


int mission::completedEmergencyMissions(){
    return mission::EmergencyMissions;
}

int mission::completedMountainousMissions(){
    return mission::MountainousMissions;
}

int mission::completedPolarMissions(){
    return mission::PolarMissions;
}