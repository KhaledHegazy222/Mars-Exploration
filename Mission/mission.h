#ifndef MISSION_H
    #define MISSION_H

    #include "../Configurations/configuration.h"

    


    class mission
    {   

        private:
            static int EmergencyMissions;
            static int MountainousMissions;
            static int PolarMissions;
            
        public:
            
            type missionType;
            int formulationDay;
            double targetLocation;
            int missionDuration;
            int significance;
            int ID;

        
            mission(type,int,double,int,int,int); 
            // Mission Type (enum missionType) - Formulation Day (int) - Target Location (double) - Mission Duration (int) - Significance (int) - ID (int)
            void finishMission();

            int completedEmergencyMissions();
            int completedMountainousMissions();
            int completedPolarMissions();
    };

    #include "mission.cpp"

#endif 
