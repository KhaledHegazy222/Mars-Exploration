#include "rover.h"


int rover::availableEmergencyRovers;
int rover::availableMountainousRovers;
int rover::availablePolarRovers;




void rover::free(){
    switch (roverType)
    {
    case Emergency:
        availableEmergencyRovers++;
        break;
    case Mountainous:
        availableMountainousRovers++;
        break;
    case Polar:
        availablePolarRovers++;
        break;
    }
    missionsLeftForCheckup--;
}


void rover::maintain(){
    switch (roverType)
    {
    case Emergency:
        availableEmergencyRovers--;
        break;
    case Mountainous:
        availableMountainousRovers--;
        break;
    case Polar:
        availablePolarRovers--;
        break;
    }
}










