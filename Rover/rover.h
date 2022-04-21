#ifndef ROVER_H
    #define ROVER_H

    #include "../Configurations/configuration.h"




    class rover
    {
            

        public:

            static int availableEmergencyRovers;
            static int availableMountainousRovers;
            static int availablePolarRovers;

            type roverType;
            int checkupDuration;
            int missionsLeftForCheckup;
            int maintenanceEndDate;
            double speed;
            int ID;


            
            void free();
            void maintain();

        



    };
    


    
    

    #include "rover.cpp"



#endif 