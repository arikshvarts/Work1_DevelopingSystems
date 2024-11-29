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

        // Getters
        bool getIsRunning() const;
        int getPlanCounter() const;
        std::vector<BaseAction*> getActionsLog() const;
        std::vector<Plan> getPlans() const;
        std::vector<Settlement*> getSettlements() const;
        std::vector<FacilityType> getFacilitiesOptions() const;

        // Setters
        void setIsRunning(bool running);
        void setPlanCounter(int counter);
        void setActionsLog(const std::vector<BaseAction*>& log);
        void setPlans(const std::vector<Plan>& planList);
        void setSettlements(const std::vector<Settlement*>& settlementList);
        void setFacilitiesOptions(const std::vector<FacilityType>& options);
    private:
        bool isRunning;
        int planCounter; //For assigning unique plan IDs
        vector<BaseAction*> actionsLog;
        vector<Plan> plans;
        vector<Settlement*> settlements;
        vector<FacilityType> facilitiesOptions;

};