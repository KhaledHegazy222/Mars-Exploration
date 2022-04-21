

#include "Data_Structures/Dynamic_Array/dynamicArray.h"
#include "Data_Structures/Queue/queue.h"
#include "Data_Structures/Priority_Queue/priorityQueue.h"

#include "Configurations/configuration.h"

#include "Mission/mission.h"
#include "Rover/rover.h"

#include "Statics/statics.h"
#include "Event/event.h"

#include <iomanip>
#include <iostream>
#include <fstream>
#include <string.h>
#include <cstdlib>
#include <cmath>
#include <Windows.h>



using namespace std;

/* Functions Prototype */
void filesHandle(int,char**);
void readConfigurations();
void readEvents();
void initializeRovers(queue<int>&,queue<int>&,queue<int>&);
void outputFile();


/* Globals */
programMode Mode = Silent;
ifstream fin;
ofstream fout;
configuration setup;
queue<event>events;
dynamicArray<mission>totalMissions;
dynamicArray<rover>totalRovers;
dynamicArray<result>statics;



/* Helper Functions */
double completeMissionTime(int id){
    auto m1 = totalMissions[id];
    auto r1 = totalRovers[m1.assignedRover];
    return ceil(m1.startDay + m1.missionDuration + m1.targetLocation / r1.speed);
}


/* Comparing Function For Priority Queue */
int priorityCompare(int& a,int &b){
    return totalMissions[a].priority() > totalMissions[b].priority();
}

int endTimeCompare(int& a,int& b){

    return completeMissionTime(a) < completeMissionTime(b);
}

int maintainCompare(int& a,int& b){
    return totalRovers[a].maintenanceEndDate < totalRovers[b].maintenanceEndDate;
}

int main(int argc,char** argv){



    filesHandle(argc,argv);

    switch (Mode)
    {
    case Silent:
        cout << "Silent Mode\n";
        break;
    case Interacrtive:
        cout << "Interactive Mode\n";
        break;
    case StepByStep:
        cout << "Step By Step Mode\n";
        break;
    }

    readConfigurations();
    readEvents();

    cout << "Simulation Starts...\n";
    



    /* Rovers Data */
    int roverNum = rover::availableEmergencyRovers + rover::availableMountainousRovers + rover::availablePolarRovers + 1;
    totalRovers.resize(roverNum);
    queue<int>ERovers,MRovers,PRovers;
    priorityQueue<int>maintenanceRovers(maintainCompare);
    initializeRovers(ERovers,MRovers,PRovers);

    /* Mission Data */
    queue<int>pendMmissions,pendPmissions;
    queue<int>execMmissions,execPmissions;
    priorityQueue<int>pendEmissions(priorityCompare);
    priorityQueue<int>execEmissions(endTimeCompare);
    totalMissions.resize(events.size() + 1);
    dynamicArray<bool>promoted(totalMissions.size(),0);
    
    dynamicArray<int>completedEmissions;
    dynamicArray<int>completedMmissions;
    dynamicArray<int>completedPmissions;

    for(int currentDay = 1;;currentDay++){
        if(events.empty() && !pendEmissions.size() && !pendMmissions.size() && !pendPmissions.size() && !execEmissions.size() && !execMmissions.size() && !execPmissions.size())
            break;
        
        dynamicArray<int>cancel,promote;

        // Collecting Today's Events
        while(events.size() && events.front().getDay() == currentDay){
            
            if(events.front().eventType == Formulation){
                auto current = events.front().data.form;
                mission currentMission(current.missionType,current.formulationDay,current.targetLocation,current.missionDuration,current.significance,current.ID);
                currentMission.currentStatus = Pending;
                totalMissions[currentMission.ID] = currentMission;

                if(currentMission.missionType == Emergency){
                    pendEmissions.push(currentMission.ID);
                }
                else if(currentMission.missionType == Mountainous){
                    pendMmissions.push(currentMission.ID);
                }
                else if(currentMission.missionType == Polar){
                    pendPmissions.push(currentMission.ID);
                }
            }   
            else if(events.front().eventType == Cancellation){
                cancel.pushBack(events.front().data.cancel.ID);
            }
            else if(events.front().eventType == Promotion){
                promote.pushBack(events.front().data.promote.ID);
            }

            events.pop();
        }
        
        while(pendMmissions.size()){
            mission& ms = totalMissions[pendMmissions.front()];
            if(ms.formulationDay + setup.maxTimeToPromote){
                ms.missionType = Emergency;
                pendEmissions.push(ms.ID);
                promote[ms.ID] = true;
            }
            else{
                break;
            }
        }

        // Cancelling Pending & Executing Missions 
        for(int i = 0;i < cancel.size();i++){ 
            totalMissions[cancel[i]].currentStatus = Cancelled;
            if(totalMissions[cancel[i]].currentStatus == Executing){
                rover& rv = totalRovers[totalMissions[cancel[i]].assignedRover];
                rv.free();
                switch (rv.roverType)
                {
                case Emergency:
                    ERovers.push(rv.ID);
                    break;
                case Mountainous:
                    MRovers.push(rv.ID);
                    break;
                case Polar:
                    PRovers.push(rv.ID);
                    break;
                }
            }
        }

        // Promoting Pending Missions
        for(int i = 0;i < promote.size();i++){
            if(totalMissions[promote[i]].currentStatus == Pending && !promoted[promote[i]]){
                totalMissions[promote[i]].missionType = Emergency;
                promoted[promote[i]] = true;
                pendEmissions.push(promote[i]);
            }
        }

        // Compeleting Current Executing Missions (Emergency / Mountainious / Polar)
        while(execEmissions.size()){
            mission& EMission = totalMissions[execEmissions.top()];
            if(EMission.currentStatus == Cancelled){
                execEmissions.pop();
            }
            else if(completeMissionTime(EMission.ID) <= currentDay){
                EMission.currentStatus = Finished;
                completedEmissions.pushBack(EMission.ID);
                result rs;
                rs.completeDay = currentDay;
                rs.formulationDay = EMission.formulationDay;
                rs.waitingDays = EMission.startDay - EMission.formulationDay;
                rs.ID = EMission.ID;
                rs.executionDays = completeMissionTime(EMission.ID);
                rs.addExecution();
                rs.addWaiting();
                if(promoted[EMission.ID]){
                    rs.addPromotion();
                }
                statics.pushBack(rs);


                EMission.finishMission();
                rover& rv = totalRovers[EMission.assignedRover];
                rv.free();
                if(!rv.checkupDuration){
                    rv.maintain();
                    rv.maintenanceEndDate = currentDay + rv.checkupDuration;
                    maintenanceRovers.push(rv.ID);
                }
                else{
                    ERovers.push(rv.ID);
                }
                execEmissions.pop();
            }
            else{
                break;
            }

        }

        while(execMmissions.size()){
            mission& MMission = totalMissions[execMmissions.front()];
            
            if(MMission.currentStatus == Cancelled || promoted[MMission.ID]){
                execMmissions.pop();
            }
            else if(completeMissionTime(MMission.ID) <= currentDay){
                MMission.currentStatus = Finished;
                completedMmissions.pushBack(MMission.ID);
                result rs;
                rs.completeDay = currentDay;
                rs.formulationDay = MMission.formulationDay;
                rs.waitingDays = MMission.startDay - MMission.formulationDay;
                rs.ID = MMission.ID;
                rs.executionDays = completeMissionTime(MMission.ID);
                rs.addExecution();
                rs.addWaiting();
                if(promoted[MMission.ID]){
                    rs.addPromotion();
                }
                statics.pushBack(rs);
                MMission.finishMission();
                rover& rv = totalRovers[MMission.assignedRover];
                rv.free();
                if(!rv.checkupDuration){
                    rv.maintain();
                    rv.maintenanceEndDate = currentDay + rv.checkupDuration;
                    maintenanceRovers.push(rv.ID);
                }
                else{
                    MRovers.push(rv.ID);
                }
                execMmissions.pop();
            }
            else{
                break;
            }

        }

        while(execPmissions.size()){
            mission& PMission = totalMissions[execPmissions.front()];
            if(PMission.currentStatus == Cancelled){
                execPmissions.pop();
            }
            else if(completeMissionTime(PMission.ID) <= currentDay){
                PMission.currentStatus = Finished;
                completedPmissions.pushBack(PMission.ID);
                result rs;
                rs.completeDay = currentDay;
                rs.formulationDay = PMission.formulationDay;
                rs.waitingDays = PMission.startDay - PMission.formulationDay;
                rs.ID = PMission.ID;
                rs.executionDays = completeMissionTime(PMission.ID);
                rs.addExecution();
                rs.addWaiting();
                if(promoted[PMission.ID]){
                    rs.addPromotion();
                }
                statics.pushBack(rs);
                PMission.finishMission();
                rover& rv = totalRovers[PMission.assignedRover];
                rv.free();
                if(!rv.checkupDuration){
                    rv.maintain();
                    rv.maintenanceEndDate = currentDay + rv.checkupDuration;
                    maintenanceRovers.push(rv.ID);
                }
                else{
                    PRovers.push(rv.ID);
                }
                execPmissions.pop();
            }
            else{
                break;
            }

        }

        // Return Checked up Rovers After Maitenance
        while(maintenanceRovers.size() && totalRovers[maintenanceRovers.top()].maintenanceEndDate <= currentDay){
            rover& rv = totalRovers[maintenanceRovers.top()];
            rv.free();
            maintenanceRovers.pop();
            switch (rv.roverType)
            {
            case Emergency:
                ERovers.push(rv.ID);
                break;
            
            case Mountainous:
                MRovers.push(rv.ID);
                break;
            
            case Polar:
                PRovers.push(rv.ID);
                break;
            
            }
        }

        // Assign Emergency Missions First
        while(pendEmissions.size()){
            if(rover::availableEmergencyRovers){
                rover::availableEmergencyRovers--;

                rover& rv = totalRovers[ERovers.front()];                   
                mission& ms = totalMissions[pendEmissions.top()];
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                ERovers.pop();
                pendEmissions.pop();
                execEmissions.push(ms.ID);

            }
            else if(rover::availableMountainousRovers){
                
                rover::availableMountainousRovers--;

                rover& rv = totalRovers[MRovers.front()];                   
                mission& ms = totalMissions[pendMmissions.front()];
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                MRovers.pop();
                pendMmissions.pop();
                execMmissions.push(ms.ID);
                
            }
            else if(rover::availablePolarRovers){

                rover::availablePolarRovers--;

                rover& rv = totalRovers[PRovers.front()];                   
                mission& ms = totalMissions[pendPmissions.front()];
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                PRovers.pop();
                pendPmissions.pop();
                execPmissions.push(ms.ID);
            }
            else{
                break;
            }
        }

        // Assign Polar Missions Second
        while(pendPmissions.size()){
            if(rover::availablePolarRovers){

                rover::availablePolarRovers--;

                rover& rv = totalRovers[PRovers.front()];                   
                mission& ms = totalMissions[pendPmissions.front()];
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                PRovers.pop();
                pendPmissions.pop();
                execPmissions.push(ms.ID);
            }
            else{
                break;
            }
        }

        // Assign Mountainious Missions Last
        while(pendMmissions.size()){
             if(rover::availableMountainousRovers){
                
                rover::availableMountainousRovers--;

                rover& rv = totalRovers[MRovers.front()];                   
                mission& ms = totalMissions[pendMmissions.front()];
                if(promoted[ms.ID]){
                    rover::availableMountainousRovers++;
                    pendMmissions.pop();
                    continue;
                }
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                MRovers.pop();
                pendMmissions.pop();
                execMmissions.push(ms.ID);
                
            }
            
            else if(rover::availableEmergencyRovers){
                rover::availableEmergencyRovers--;

                rover& rv = totalRovers[ERovers.front()];                   
                mission& ms = totalMissions[pendEmissions.top()];
                
                ms.assignedRover = rv.ID;
                ms.currentStatus = Executing;
                ms.startDay = currentDay;

                ERovers.pop();
                pendEmissions.pop();
                execEmissions.push(ms.ID);

            }
            else{
                break;
            }
        }

        if(Mode != Silent){
            
            if(Mode == StepByStep){
                Sleep(1000);
            }
            else{
                char ch;
                while((ch = getchar()) != '\n');
            }

            cout << "Current Day: " << currentDay << '\n';
            int waiting = pendEmissions.size() + pendMmissions.size() + pendPmissions.size();
            cout << waiting << " Waiting Missions: ";
            auto temp = pendEmissions;
            if(temp.size()){
                cout << "[" << temp.top(),temp.pop();
                while(temp.size())
                    cout << ", " << temp.top(),temp.pop();
                cout << "] ";
            }
            auto temp2 = pendMmissions;
            if(temp2.size()){
                cout << "{" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << "} ";
            }
            temp2 = pendPmissions;
            if(temp2.size()){
                cout << "(" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << ") ";
            }
            cout << "\n-------------------------\n";


            int executing = execEmissions.size() + execMmissions.size() + execPmissions.size();
            cout << executing << " Executing Missions: ";
            temp = execEmissions;
            if(temp.size()){
                cout << "[" << temp.top(),temp.pop();
                while(temp.size())
                    cout << ", " << temp.top(),temp.pop();
                cout << "] ";
            }
            temp2 = execMmissions;
            if(temp2.size()){
                cout << "{" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << "} ";
            }
            temp2 = execPmissions;
            if(temp2.size()){
                cout << "(" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << ") ";
            }
            cout << "\n-------------------------\n";   


            
            int available = ERovers.size() + MRovers.size() + PRovers.size();
            cout << available << " Available Rovers: ";
            temp2 = ERovers;
            if(temp2.size()){
                cout << "[" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << "] ";
            }
            temp2 = MRovers;
            if(temp2.size()){
                cout << "{" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << "} ";
            }
            temp2 = PRovers;
            if(temp2.size()){
                cout << "(" << temp2.front(),temp2.pop();
                while(temp2.size())
                    cout << ", " << temp2.front(),temp2.pop();
                cout << ") ";
            }
            cout << "\n-------------------------\n";   


            cout << maintenanceRovers.size() << " In Checkup Rovers: ";
            temp = maintenanceRovers;
            if(temp.size()){
                cout << "[" << temp.top(),temp.pop();
                while(temp.size())
                    cout << ", " << temp.top(),temp.pop();
                cout << "] ";
            }
            cout << "\n-------------------------\n";   
        
            int completed = completedEmissions.size() + completedMmissions.size() + completedPmissions.size();
            cout << completed << " Completed Missions: ";
            if(completedEmissions.size()){
                cout << "[" << completedEmissions[0];
                for(int i = 1;i < completedEmissions.size();i++)
                    cout << ", " << completedEmissions[i];
                cout << "] ";
            }
            if(completedMmissions.size()){
                cout << "{" << completedMmissions[0];
                for(int i = 1;i < completedMmissions.size();i++)
                    cout << ", " << completedMmissions[i];
                cout << "} ";
            }
            if(completedPmissions.size()){
                cout << "(" << completedPmissions[0];
                for(int i = 1;i < completedPmissions.size();i++)
                    cout << ", " << completedPmissions[i];
                cout << ") ";
            }
            cout << "\n\n";   

                
        }

    
    
    }    
    
    
    outputFile();
    cout << "Simulation ends, Output file created\n";




}







void filesHandle(int argc,char **argv){
    if(argc != 6){
        cout << "Bad File Parameters";
        exit(1);
    }

    char *input = NULL;
    char *output = NULL;

    for(int i = 1;i < argc;i++){
        if(strcmp(argv[i],"-i") == 0){
            input = argv[i + 1];
            i++;
        }
        else if(strcmp(argv[i],"-o") == 0){
            output = argv[i + 1];
            i++;
        }
        else if(strcmp(argv[i],"--interactive") == 0){
            Mode = Interacrtive;
        }
        else if(strcmp(argv[i],"--silent") == 0){
            Mode = Silent;
        }
        else if(strcmp(argv[i],"--step_by_step") == 0){
            Mode = StepByStep;
        }
        else{
            cout << "Bad Arguments";
            exit(1);
        }
        

    }

    if(input == NULL || output == NULL){
        cout << "Bad File Formats";
        exit(1);
    }

    fin.open(input);
    fout.open(output);


}

void readConfigurations(){

    fin >> rover::availableMountainousRovers >>
    rover::availablePolarRovers >> 
    rover::availableEmergencyRovers >>

    setup.mountainRoverSpeed >>
    setup.polarRoverSpeed >>
    setup.emergencyRoverSpeed >>


    setup.missionsBeforeCheckup >>
    setup.mountainousRoverCheckupDuration >>
    setup.polarRoverCheckupDuration >>
    setup.emergencyRoverCheckupDuration >>

    setup.maxTimeToPromote;
}

void readEvents(){

    int eventsNum;
    fin >> eventsNum;
    while(eventsNum--){
        event current;
        char eventType;
        fin >> eventType;
        if(eventType == 'F'){
            current.eventType = Formulation;

            char mType;
            fin >> mType;
            switch (mType)
            {
            case 'E':
                current.data.form.missionType = Emergency;
                break;
            
            case 'M':
                current.data.form.missionType = Mountainous;
                break;
            case 'P':
                current.data.form.missionType = Polar;    
                break;
            }

            fin >> current.data.form.formulationDay >>
            current.data.form.ID >> 
            current.data.form.targetLocation >>
            current.data.form.missionDuration >>
            current.data.form.significance;  
        }   
        else if(eventType == 'X'){
            current.eventType = Cancellation;
            fin >> current.data.cancel.cancellationDay >> current.data.cancel.ID;
        }
        else if(eventType == 'P'){
            current.eventType = Promotion;
            fin >> current.data.promote.promotionDay >> current.data.cancel.ID;
        }

        events.push(current);

    }

}


void initializeRovers(queue<int>&ERovers,queue<int>&MRovers,queue<int>&PRovers){
    int lastRovers = 0;
    for(int id = 1;id <= rover::availableEmergencyRovers;id++){
        totalRovers[lastRovers + id].ID = lastRovers + id;
        totalRovers[lastRovers + id].roverType = Emergency;
        totalRovers[lastRovers + id].checkupDuration = setup.emergencyRoverCheckupDuration;
        totalRovers[lastRovers + id].missionsLeftForCheckup = setup.missionsBeforeCheckup;
        totalRovers[lastRovers + id].speed = setup.emergencyRoverSpeed;  
        ERovers.push(lastRovers + id);
    }

    lastRovers += rover::availableEmergencyRovers;

    for(int id = 1;id <= rover::availableMountainousRovers;id++){
        totalRovers[lastRovers + id].ID = lastRovers + id;
        totalRovers[lastRovers + id].roverType = Mountainous;
        totalRovers[lastRovers + id].checkupDuration = setup.mountainousRoverCheckupDuration;
        totalRovers[lastRovers + id].missionsLeftForCheckup = setup.missionsBeforeCheckup;
        totalRovers[lastRovers + id].speed = setup.mountainRoverSpeed;  
        MRovers.push(lastRovers + id);
    }

    lastRovers += rover::availableMountainousRovers;

    for(int id = 1;id <= rover::availablePolarRovers;id++){
        totalRovers[lastRovers + id].ID = lastRovers + id;
        totalRovers[lastRovers + id].roverType = Polar;
        totalRovers[lastRovers + id].checkupDuration = setup.polarRoverCheckupDuration;
        totalRovers[lastRovers + id].missionsLeftForCheckup = setup.missionsBeforeCheckup;
        totalRovers[lastRovers + id].speed = setup.polarRoverSpeed;  
        PRovers.push(lastRovers + id);
    }


}
void outputFile(){
    sort(statics.begin(),statics.end());
    
    fout << right << setw(4) << "CD" << " ";
    fout << right << setw(4) << "ID" << " ";
    fout << right << setw(4) << "FD" << " ";
    fout << right << setw(4) << "WD" << " ";
    fout << right << setw(4) << "ED" << "\n";
    for(int i = 0;i < statics.size();i++){
        fout << right << setw(4) << statics[i].completeDay << " ";
        fout << right << setw(4) << statics[i].ID << " ";
        fout << right << setw(4) << statics[i].formulationDay << " ";
        fout << right << setw(4) << statics[i].waitingDays << " ";
        fout << right << setw(4) << statics[i].executionDays << "\n";
    }
    
    fout << "-------------------------\n";
    fout << "-------------------------\n";
    int totalCompletedMissions = mission::completedEmergencyMissions() + mission::completedMountainousMissions() + mission::completedPolarMissions();
    fout << "Missions: " << totalCompletedMissions << " [M: " << mission::completedMountainousMissions() << ", P: " << mission::completedPolarMissions() << ", E: " << mission::completedEmergencyMissions << "]\n";
    int totalAvailableRovers = rover::availableEmergencyRovers + rover::availableMountainousRovers + rover::availablePolarRovers;
    fout << "Rover: " << totalAvailableRovers << " [M: " << rover::availableMountainousRovers << ", P: " << rover::availablePolarRovers << ", E: " << rover::availableEmergencyRovers << "]\n";
    fout << "Avg Wait = " << result::totalWaiting() / statics.size() << ", Avg Execution = " << result::totalExecution() / statics.size()  << endl;
    fout <<  "Auto-Promoted = " << result::totalPromoted() *  100.0 / mission::completedMountainousMissions() << "%\n";

}