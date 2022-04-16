#ifndef ROVER_H
    #define ROVER_H

    #include "../Configurations/configuration.h"




    class rover
    {
        private:
        

        public:

            type roverType;
            int checkupDuration;
            int missionsLeftForCheckup;
            int daysLeftForMaintenance;
            double speed;
            int ID;



            rover(type,int,int,int,double,int);




    };
    


    
    





#endif 