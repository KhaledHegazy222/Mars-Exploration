#include "rover.h"

rover::rover(type roverType,int checkupDuration,int missionsLeftForCheckup,int daysLeftForMaintenance,double speed,int ID) : 
    roverType(roverType),
    checkupDuration(checkupDuration),
    missionsLeftForCheckup(missionsLeftForCheckup),
    daysLeftForMaintenance(daysLeftForMaintenance),
    speed(speed),
    ID(ID) {}
