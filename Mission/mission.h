#ifndef MISSION_H
    #define MISSION_H

    #include "../Configurations/configuration.h"
    #include "../Rover/rover.h"

    class mission
    {   

        private:
            static int EmergencyMissions;
            static int MountainousMissions;
            static int PolarMissions;
            
        public:
            
            type missionType;
            missionStatus currentStatus;
            int ID;
            int formulationDay;
            int missionDuration;
            int significance;
            int startDay;
            int assignedRover;
            double targetLocation;



            
            mission();

            mission(type,int,double,int,int,int);



            void finishMission();

            static int completedEmergencyMissions();
            static int completedMountainousMissions();
            static int completedPolarMissions();

            double priority();
            

    };

    #include "mission.cpp"

#endif 
