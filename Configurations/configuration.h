#ifndef CONFIGURATIONS_H
    #define CONFIGURATIONS_H




    enum type {Emergency,Mountainous,Polar}; 
    enum missionStatus {Pending,Executing,Finished,Cancelled};
    enum programMode {Interacrtive,StepByStep,Silent};
    
    class configuration
    {
    
    
        
    public:

        int mountainRoverSpeed;
        int polarRoverSpeed;
        int emergencyRoverSpeed;


        int missionsBeforeCheckup;
        int mountainousRoverCheckupDuration;
        int polarRoverCheckupDuration;
        int emergencyRoverCheckupDuration;

        int maxTimeToPromote;

    };
    

#endif 