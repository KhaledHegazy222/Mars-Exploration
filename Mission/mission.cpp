#include "mission.h"
#include "../Configurations/configuration.h"
int mission::EmergencyMissions;
int mission::MountainousMissions;
int mission::PolarMissions;




mission::mission(){}

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
    return EmergencyMissions;
}

int mission::completedMountainousMissions(){
    return MountainousMissions;
}

int mission::completedPolarMissions(){
    return PolarMissions;
}

double mission::priority(){
    if(missionType == Mountainous || missionType == Polar)
        return 0;
    return (1.0 / formulationDay) + 0.5 * (1.0 / targetLocation ) + 0.5 * (1.0 / missionDuration) + 3 * significance;
    
}


