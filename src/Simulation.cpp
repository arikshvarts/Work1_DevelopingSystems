#pragma once
#include <string>
#include <vector>
#include "Facility.h"
#include "Plan.h"
#include "Settlement.h"
#include "Simulation.h"
#include <algorithm> // For std::find
#include <iostream>

using namespace std;

using std::string;
using std::vector;

//    private:
//   bool isRunning;
//  int planCounter; //For assigning unique plan IDs
//  vector<BaseAction*> actionsLog;
//   vector<Plan> plans;
//   vector<Settlement*> settlements;
//   vector<FacilityType> facilitiesOptions;
// Simulation::Simulation(const string &configFilePath);
// Simulation::Simulation(const Simulation &sim);
// Simulation::Simulation operator=(const Simulation &sim);
// Simulation::Simulation(const Simulation &&sim);
// Simulation::Simulation operator=(const Simulation &&sim);
// ~Simulation::Simulation();
void Simulation::start()
{
    cout << "Simulation has started";
}
void Simulation::addPlan(const Settlement &settlement, SelectionPolicy *selectionPolicy)
{
    plans.push_back(Plan(planCounter, settlement, selectionPolicy, facilitiesOptions));
    planCounter-=1;
}
void Simulation::addAction(BaseAction *action)
{
    actionsLog.push_back(action);
}
bool Simulation::addSettlement(Settlement *settlement)
{
    settlements.push_back(settlement);
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
    for (auto &pl : plans)
    {
        cout << pl.toString();
    }
    isRunning = false;
}
void Simulation::open()
{
    isRunning=true;
}

// void Simulation::printInitialState() const
// {
//     std::cout << "Simulation Initial State:" << std::endl;
//     std::cout << "isRunning: " << (isRunning ? "true" : "false") << std::endl;
//     std::cout << "planCounter: " << planCounter << std::endl;

//     std::cout << "Settlements:" << std::endl;
//     if (settlements.empty())
//     {
//         std::cout << "  None" << std::endl;
//     }
//     else
//     {
//         for (const auto &settlement : settlements)
//         {
//             std::cout << settlement->toString() << std::endl;
//         }
//     }

//     std::cout << "Facilities Options:" << std::endl;
//     if (facilitiesOptions.empty())
//     {
//         std::cout << "  None" << std::endl;
//     }
//     else
//     {
//         for (const auto &facility : facilitiesOptions)
//         {
//             std::cout << "  Name: " << facility.getName()
//                       << " , Category: " << int(facility.getCategory()) << "\n"
//                       << ", Price: " << facility.getCost()
//                       << ", Life Quality Score: " << facility.getLifeQualityScore()
//                       << ", Economy Score: " << facility.getEconomyScore()
//                       << ", Environment Score: " << facility.getEnvironmentScore() << std::endl;
//                       }
//                       }
// }
