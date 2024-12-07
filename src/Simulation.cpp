#include <string>
#include <vector>
#include "../include/Simulation.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"

#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

Simulation::Simulation(const string &configFilePath) : isRunning(false), planCounter(0),
 actionsLog(), plans(), settlements(), facilitiesOptions()
{
    std::ifstream inputFile(configFilePath);

    // Check if the file opened successfully
    if (!inputFile.is_open())
    {
        cerr << "Error: Could not open the file " << configFilePath << std::endl;
    }

    string line;
    while (getline(inputFile, line))
    {
        // Skip empty lines
        if (line.empty())
            continue;

        vector<string> arguments = Auxiliary::parseArguments(line);
        if (arguments[0] == "settlement")
        {
            Settlement *set = new Settlement(arguments[1], static_cast<SettlementType>(stoi(arguments[2])));
            addSettlement(set);
        }
        else if (arguments[0] == "facility")
        {
            addFacility(FacilityType(arguments[1], static_cast<FacilityCategory>(stoi(arguments[2])), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6])));
        }
        else if (arguments[0] == "plan")
        {
                        SelectionPolicy* policy;
            if (arguments[2] == "nve")
            {
               policy = new NaiveSelection();
            }
            else if (arguments[2] == "bal")
            {
                policy = new BalancedSelection(0, 0, 0);
            }
            else if (arguments[2] == "eco")
            {
                policy = new EconomySelection();
            }
            else if (arguments[2] == "env")
            {
                policy = new SustainabilitySelection();
            }   
                
            
          addPlan(getSettlement(arguments[1]), policy);
    }
    }
    inputFile.close(); // Close the file after processing
}
Simulation::Simulation(const Simulation &sim) : isRunning(sim.isRunning), planCounter(sim.planCounter), actionsLog(), plans(), settlements(), facilitiesOptions(sim.facilitiesOptions)
{
    for (BaseAction* ptr : sim.actionsLog)
    {
        actionsLog.push_back(ptr->clone()); // Allocate new object and copy data
    }
    for (Settlement*  ptr : sim.settlements)
    {
        settlements.push_back(ptr->clone());
    }
    for (Plan pl : sim.plans)
    {
        Settlement &sett = getSettlement(pl.getSettlement().getName());
        Plan planToInsert(pl.getPlanId(), sett, pl.getSelectionPolicyPtr()->clone(), pl.getPlanStatus(),
                          facilitiesOptions, pl.getlifeQualityScore(), pl.getEconomyScore(), pl.getEnvironmentScore());
        for (Facility *fac : pl.getFacilities())
            planToInsert.addFacility(fac->clone());
        for (Facility *facCon : pl.getUnderConstruction())
            planToInsert.addFacility(facCon->clone());
        plans.push_back(planToInsert);
    }

}
Simulation &Simulation::operator=(const Simulation &sim)
{
    if (this != &sim)
    {
        // Clean up current resources (actionsLog and settlements)
        for (BaseAction *ptr : actionsLog)
        {
            delete ptr;
        }
        actionsLog.clear();

        for (Settlement *ptr : settlements)
        {
            delete ptr;
        }
        settlements.clear();
        plans.clear();
        // Deep copy the actionsLog
        for (BaseAction *ptr : sim.actionsLog)
        {
            actionsLog.push_back(ptr->clone()); // Assuming clone() makes a deep copy
        }

        // Deep copy the settlements
        for (Settlement *ptr : sim.settlements)
        {
            settlements.push_back(ptr->clone()); // Assuming 'new' creates a deep copy
        }

        // Copy primitive members
        isRunning = sim.isRunning;
        planCounter = sim.planCounter;
        for (Plan pl : sim.plans)
        {
            Settlement &sett = getSettlement(pl.getSettlement().getName());
            Plan planToInsert(pl.getPlanId(), sett, pl.getSelectionPolicyPtr()->clone(), pl.getPlanStatus(),
                            facilitiesOptions, pl.getlifeQualityScore(), pl.getEconomyScore(), pl.getEnvironmentScore());
            for (Facility *fac : pl.getFacilities())
                planToInsert.addFacility(fac->clone());
            for (Facility *facCon : pl.getUnderConstruction())
                planToInsert.addFacility(facCon->clone());
            plans.push_back(planToInsert);
        }
        facilitiesOptions.clear();
        for (FacilityType facility : sim.facilitiesOptions)
        {
            facilitiesOptions.push_back(facility); // Uses copy constructor to add the facility
        }

        return *this;
    }
    return *this; // Ensures a return statement exists if this == &sim
}
Simulation::Simulation(Simulation &&sim) : isRunning(sim.isRunning), planCounter(sim.planCounter),  actionsLog(sim.actionsLog), plans(sim.plans), settlements(sim.settlements), facilitiesOptions(sim.facilitiesOptions)
{
    sim.actionsLog.clear();
    sim.settlements.clear();
}

Simulation &Simulation::operator=(Simulation &&sim)
{
    // Check for self-assignment
    if (this != &sim)
    {
        for (BaseAction *ptr : actionsLog)
        {
            delete ptr;
        }
        actionsLog = sim.actionsLog;
        sim.actionsLog.clear(); // Clear the source object's actionsLog to avoid dangling references

        for (Settlement *ptr : settlements)
        {
            delete ptr;
            
        }
        settlements = sim.settlements;
        sim.settlements.clear(); // Clear the current object's settlements

        plans.clear();
        for (Plan curr : sim.plans)
        {
            plans.push_back(curr);
        }
        sim.plans.clear();

        facilitiesOptions.clear(); // Clear existing facility options in the current object
        for (FacilityType curr : sim.facilitiesOptions)
        {
            facilitiesOptions.push_back(curr);
        }
        sim.facilitiesOptions.clear(); // Clear the source object's facility options
    }

    // Return a reference to the current object
    return *this;
}

Simulation::~Simulation()
{
    for (BaseAction *ptr : actionsLog)
    {
        delete ptr;
    }

    for (Settlement *ptr : settlements)
    {
        delete ptr;
    }
}
void Simulation::start()
{
    cout << "Simulation has started: " << endl;
    open();
    while (isRunning)
    {
        string input;
        getline(cin, input);
        vector<string> args = Auxiliary::parseArguments(input);
        if (args[0] == "plan")
        {
            AddPlan *action = new AddPlan(args[1], args[2]);
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "planStatus")
        {
            PrintPlanStatus *action = new PrintPlanStatus(stoi(args[1]));
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "facility")
        {
            AddFacility *action = new AddFacility(args[1], static_cast<FacilityCategory>(stoi(args[2])), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "backup")
        {
            BackupSimulation *action = new BackupSimulation();
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "log")
        {
            PrintActionsLog *action = new PrintActionsLog();
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "settlement")
        {
            AddSettlement *action = new AddSettlement(args[1], static_cast<SettlementType>(stoi(args[2])));
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "step")
        {
            SimulateStep *action = new SimulateStep(stoi(args[1]));
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "restore")
        {
            RestoreSimulation *action = new RestoreSimulation();
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "close")
        {
            Close *action = new Close();
            action->act(*this);
            addAction(action);
        }
        else if (args[0] == "changePolicy")
        {
            ChangePlanPolicy *action = new ChangePlanPolicy(stoi(args[1]), args[2]);
            action->act(*this);
            addAction(action);
        }
        else
            break;
    }
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter += 1;
}
void Simulation::addAction(BaseAction *action)
{
    if (action != nullptr)
        actionsLog.push_back(action);
}
bool Simulation::addSettlement(Settlement *settlement)
{
    if (!isSettlementExists(settlement->getName()))
    {
        settlements.push_back(settlement);
        return true;
    }
    return false;
}
bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
    return true;
}
bool Simulation::isSettlementExists(const string &settlementName)
{
    // Loop through each Sett in the vector and check their name
    for (const auto &sett : settlements)
    {
        if (sett->getName() == settlementName)
        {
            return true; // Found the Sett with the matching name
        }
    }
    return false; // Name not found
}
Settlement &Simulation::getSettlement(const string &settlementName)
{
    for (auto &sett : settlements)
    {
        if (sett->getName() == settlementName)
        {
            return *sett; // Found the Sett with the matching name
        }
    }
    throw runtime_error("settlemnt dowsn't exist");

}

Plan &Simulation::getPlan(const int planID)
{
    if (planID >= this->planCounter)
        throw runtime_error("plan doesn't exist");
    return plans[planID];
}
void Simulation::step()
{
    for (Plan &pl : plans)
    {
        pl.step();
    }
}
void Simulation::close()
{
    isRunning = false;
    for (Plan& curr: plans) {
        string str = "Plan Id: " + std::to_string(curr.getPlanId()) + " \n";
        cout << "Settlement Name: " + curr.getSettlement().getName() + " \n";
        cout << "Life quality score: " + std::to_string(curr.getlifeQualityScore()) + " \n";
        cout << "Economy score: " + std::to_string(curr.getEconomyScore()) + " \n";
        cout << "Environment score: " + std::to_string(curr.getEnvironmentScore()) + " \n";
    }
}
void Simulation::open()
{
    isRunning = true;
}

vector<Plan> &Simulation::getPlansVec()
{
    return plans;
}
vector<BaseAction *> Simulation::getActionsLog() const
{
    return actionsLog;
}
const bool Simulation::IsPlanExist(const int planID) const
{
    bool to_ret = false;
    for (Plan pl : plans)
    {
        if (pl.getPlanId() == planID)
        {
            return true;
        }
    }
    return to_ret;
}

const bool Simulation::IsFacilityExist(const string fac_Name) const
{
    bool to_ret = false;
    for (FacilityType fac : facilitiesOptions)
    {
        if (fac.getName() == fac_Name)
        {
            return true;
        }
    }
    return to_ret;
}
