#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"


using namespace std;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        Simulation(const string &configFilePath);
        Simulation(const Simulation &sim);
        Simulation& operator=(const Simulation& sim);
        Simulation(Simulation &&sim);
        Simulation& operator=(Simulation &&sim);
        ~Simulation();
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void printInitialState() const;
        vector<BaseAction *> getActionsLog() const;
        Simulation* clone() const;
        void step();
        void close();
        void open();
        vector<Plan>& getPlansVec(); //we added new func for accessing the Plans vector in SimulateStep Action
        const bool IsPlanExist(const int planID) const;
        const bool IsFacilityExist(const string Fac_Name) const;        
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};