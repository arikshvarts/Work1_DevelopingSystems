#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
using std::string;
using std::vector;
using namespace std;

class BaseAction;
class SelectionPolicy;

class Simulation {
    public:
        // Simulation(const string &configFilePath);
        // Simulation(const Simulation &sim);
        // Simulation operator=(const Simulation &sim);
        // Simulation(const Simulation &&sim);
        // Simulation operator=(const Simulation &&sim);
        // ~Simulation();
        void start();
        void addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy);
        void addAction(BaseAction *action);
        bool addSettlement(Settlement *settlement);
        bool addFacility(FacilityType facility);
        bool isSettlementExists(const string &settlementName);
        Settlement &getSettlement(const string &settlementName);
        Plan &getPlan(const int planID);
        void printInitialState() const;
        void step();
        void close();
        void open();

    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};