#include <string>
#include <vector>
#include "../include/Simulation.h"
#include "../include/Auxiliary.h"
#include "../include/Action.h"

#include <algorithm>
#include <iostream>
#include <fstream>
using namespace std;

Simulation::Simulation(const string &configFilePath) : planCounter(0),
                                                       isRunning(false)
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
            addFacility(FacilityType(arguments[1],static_cast<FacilityCategory>(stoi(arguments[2])), stoi(arguments[3]), stoi(arguments[4]), stoi(arguments[5]), stoi(arguments[6])));
        }
        else if (arguments[0] == "plan")
        {
            for (int i = 0; i < settlements.size(); ++i)
            {
                {
                    if ((*settlements[i]).getName() == arguments[1])
                    {
                        if (arguments[2] == "nve")
                        {
                            addPlan(*settlements[i], new NaiveSelection());
                        }
                        else if (arguments[2] == "bal")
                        {
                            addPlan(*settlements[i], new BalancedSelection(0, 0, 0));
                        }
                        else if (arguments[2] == "eco")
                        {
                            addPlan(*settlements[i], new EconomySelection());
                        }
                        else if (arguments[2] == "env")
                        {
                            addPlan(*settlements[i], new SustainabilitySelection());
                        }
                        else
                        {
                            throw invalid_argument("Invalid policy type: " + arguments[2]);
                        }
                    }
                }
            }
        }
    }
    inputFile.close(); // Close the file after processing
}
Simulation::Simulation(const Simulation &sim) : isRunning(sim.isRunning), planCounter(sim.planCounter), plans(plans), facilitiesOptions(facilitiesOptions)
{
    for (auto ptr : sim.actionsLog)
    {
        actionsLog.push_back(ptr->clone()); // Allocate new object and copy data
    }

    // Then, deep copy the second vector (vec2)
    for (auto ptr : sim.settlements)
    {
        settlements.push_back(new Settlement(*ptr));
    }
    for (const FacilityType& facility : sim.facilitiesOptions) {
    facilitiesOptions.push_back(facility); // Uses copy constructor
    }
}
Simulation &Simulation::operator=(const Simulation &sim)
{
    if (this != &sim)
    {
        // Clean up current resources (actionsLog and settlements)
        for (auto ptr : actionsLog)
        {
            delete ptr;
        }
        actionsLog.clear();

        for (auto ptr : settlements)
        {
            delete ptr;
        }
        settlements.clear();

        // Deep copy the actionsLog
        for (auto ptr : sim.actionsLog)
        {
            actionsLog.push_back(ptr->clone()); // Assuming clone() makes a deep copy
        }

        // Deep copy the settlements
        for (auto ptr : sim.settlements)
        {
            settlements.push_back(new Settlement(*ptr)); // Assuming 'new' creates a deep copy
        }

        // Copy primitive members
        isRunning = sim.isRunning;
        plans.clear();

        // Copy each Plan from the other Simulation
        for (const Plan &plan : sim.plans)
        {
            plans.push_back(plan);
        }
        // facilitiesOptions = sim.facilitiesOptions;
        for (const FacilityType &facility : sim.facilitiesOptions)
        {
            facilitiesOptions.push_back(facility); // Uses copy constructor to add the facility
        }

    }
}
Simulation::Simulation(Simulation &&sim) : isRunning(sim.isRunning), planCounter(sim.planCounter), plans(sim.plans), facilitiesOptions(sim.facilitiesOptions), settlements(sim.settlements), actionsLog(sim.actionsLog)
{
    sim.actionsLog.clear();
    sim.settlements.clear();
}

Simulation &Simulation::operator=(Simulation &&sim)
{
    if (this != &sim)
    {
        // Clean up current resources (actionsLog and settlements)
        for (auto ptr : actionsLog)
        {
            delete ptr;
        }
        actionsLog.clear();

        for (auto ptr : settlements)
        {
            delete ptr;
        }
        settlements.clear();

        // Transfer ownership of resources from 'sim' to 'this'
        actionsLog = move(sim.actionsLog);               // Move the vector of actions (no deep copy, just ownership transfer)
        settlements = move(sim.settlements);             // Move the vector of settlements (ownership transfer)
        plans = move(sim.plans);                         // Move the plans vector
        facilitiesOptions = move(sim.facilitiesOptions); // Move the facilities options vector

        // Transfer other data members
        isRunning = sim.isRunning;
        planCounter = sim.planCounter;

        sim.actionsLog.clear();
        sim.settlements.clear();
        sim.plans.clear();
        sim.facilitiesOptions.clear();
    }
}

Simulation::~Simulation()
{
    for (auto ptr : actionsLog)
    {
        delete ptr;
    }
    actionsLog.clear();

    for (auto ptr : settlements)
    {
        delete ptr;
    }
    settlements.clear();
}
void Simulation::start()
{
    cout << "Simulation has started";
    open();
    while (isRunning)
    {
        string input;
        getline(cin, input);
        vector<string> args = Auxiliary::parseArguments(input);
        if (args[0] == "plan")
        {
            AddPlan action = AddPlan(args[1], args[2]);
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "planStatus")
        {
            PrintPlanStatus action = PrintPlanStatus(stoi(args[1]));
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "facility")
        {
            AddFacility action = AddFacility(args[1], static_cast<FacilityCategory>(stoi(args[2])), stoi(args[3]), stoi(args[4]), stoi(args[5]), stoi(args[6]));
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "backup")
        {
            BackupSimulation action = BackupSimulation();
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "log")
        {
            PrintActionsLog action = PrintActionsLog();
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "settlement")
        {
            AddSettlement action = AddSettlement(args[1], static_cast<SettlementType>(stoi(args[2])));
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "step")
        {
            SimulateStep action = SimulateStep(stoi(args[1]));
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "restore")
        {
            RestoreSimulation action = RestoreSimulation();
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "close")
        {
            Close action = Close();
            action.act(*this);
            addAction(&action);
        }
        else if (args[0] == "changePolicy")
        {
            ChangePlanPolicy action = ChangePlanPolicy(stoi(args[1]),args[2]);
            action.act(*this);
            addAction(&action);
        }
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
    }
}
bool Simulation::addFacility(FacilityType facility)
{
    facilitiesOptions.push_back(facility);
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
}

Plan &Simulation::getPlan(const int planID)
{
    for (auto &pl : plans)
    {
        if (pl.getPlanId() == planID)
        {
            return pl; // Found the Sett with the matching name
        }
    }
}
void Simulation::step()
{
    for (Plan pl : plans)
    {
        pl.step();
    }
}
void Simulation::close()
{
    isRunning = false;
}
void Simulation::open()
{
    isRunning = true;
}

vector<Plan> &Simulation::getPlansVec() 
{
    return plans;
}

void Simulation::printInitialState() const
{
    cout << "Simulation Initial State:" << endl;
    cout << "isRunning: " << (isRunning ? "true" : "false") << endl;
    cout << "planCounter: " << planCounter << endl;

    cout << "Settlements:" << endl;
    if (settlements.empty())
    {
        cout << "  None" << endl;
    }
    else
    {
        for (const auto &settlement : settlements)
        {
            cout << settlement->toString() << endl;
        }
    }

    cout << "Facilities Options:" << endl;
    if (facilitiesOptions.empty())
    {
        cout << "  None" << endl;
    }
    else
    {
        for (const auto &facility : facilitiesOptions)
        {
            cout << "  Name: " << facility.getName()
                      << " , Category: " << int(facility.getCategory()) << "\n"
                      << ", Price: " << facility.getCost()
                      << ", Life Quality Score: " << facility.getLifeQualityScore()
                      << ", Economy Score: " << facility.getEconomyScore()
                      << ", Environment Score: " << facility.getEnvironmentScore() << endl;
        }
    }
}
Simulation *Simulation::clone() const
{
    return new Simulation(*this); // Use copy constructor to create a deep copy
}
vector<BaseAction *>  Simulation::getActionsLog() const
{
    return actionsLog;
}
const bool Simulation:: IsPlanExist(const int planID) const{
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

const bool Simulation:: IsFacilityExist(const string fac_Name) const{
    bool to_ret = false;
    for(FacilityType fac : facilitiesOptions){
        if(fac.getName() == fac_Name){return true;}
    }
    return to_ret;
}       
