#include <string>
#include <vector>
#include "../include/Action.h"
#include "../include/SelectionPolicy.h"

#include <iostream>

extern Simulation *backUp;

ActionStatus BaseAction ::getStatus() const
{
    return status; // returns 0/1 and not COMPLETED/ERROR
}
void BaseAction ::complete()
{
    status = ActionStatus::COMPLETED;
}
void BaseAction ::error(string errorMsg)
{
    status = ActionStatus ::ERROR;
    _errorMsg = errorMsg;
}
const string &BaseAction ::getErrorMsg() const
{
    return _errorMsg;
}

SimulateStep ::SimulateStep(const int numOfSteps) : _numOfSteps(numOfSteps) {}
void SimulateStep ::act(Simulation &simulation)
{
    for (Plan p : simulation.getPlansVec())
    {
        p.step();
    }
}
const string SimulateStep ::toString() const
{
    return "Simulated" + std::to_string(_numOfSteps) + " steps";
}
SimulateStep *SimulateStep ::clone() const
{
    return new SimulateStep(*this);
    // this method return a pointer to a new SimulateStep object with the same details
}

AddPlan ::AddPlan(const string &settlementName, const string &selectionPolicy) : _settlementName(settlementName), _selectionPolicy(selectionPolicy) {}
void AddPlan ::act(Simulation &simulation)
{
    if (simulation.isSettlementExists(_settlementName) == true)
    {
        SelectionPolicy *policy; // declare policy without initalize, initalize only when we
        if (_selectionPolicy == "nve")
        {
            policy = new NaiveSelection();
        }
        // else if(_selectionPolicy == "bal") {policy = new BalancedSelection(simulation.getPlan().getlifeQualityScore(),simulation.getPlan().getEconomyScore(),simulation.getPlan().getEnvironmentScore());}
        // livdok how to enter tho 3scores to the  new BalancedSelection
        else if (_selectionPolicy == "eco")
        {
            policy = new EconomySelection();
        }
        else if (_selectionPolicy == "env")
        {
            policy = new SustainabilitySelection();
        }
        else
        {
            error("no such a Selection Policy");
            cout << "ERROR: " + getErrorMsg();
            return; // breaks the function, doesnt continue
        }
        simulation.addPlan(simulation.getSettlement(_settlementName), policy);
    }
    else
    { // if there is no such settlement name
        error("no such a Settlement name");
        cout << "ERROR: " + getErrorMsg();
        return; // breaks the function, doesnt continue
    }
    complete();
}
const string AddPlan ::toString() const
{

    return "Added a new Plan to " + _settlementName + "settlemnt, with " + _selectionPolicy + " Policy";
}
AddPlan *AddPlan ::clone() const
{
    return new AddPlan(*this);
    // this method return a pointer to a new Addplan object with the same details
}

AddSettlement ::AddSettlement(const string &settlementName, SettlementType settlementType) : _settlementName(settlementName), _settlementType(settlementType) {}
void AddSettlement ::act(Simulation &simulation)
{
    simulation.addSettlement(new Settlement(_settlementName, _settlementType)); // asks for pointer
    complete();
}
AddSettlement *AddSettlement ::clone() const
{
    return new AddSettlement(*this);
    // this method return a pointer to a new AddSettlement object with the same details
}
const string AddSettlement ::toString() const
{
    string types[3] = {"Village", "City", "Metropolin"};
    return "Added a new Settlement: " + _settlementName + " from type: " + types[int(_settlementType)];
}

AddFacility ::AddFacility(const string &facilityName, const FacilityCategory facilityCategory, const int price, const int lifeQualityScore, const int economyScore, const int environmentScore)
    : _facilityName(facilityName), _facilityCategory(facilityCategory), _price(price), _lifeQualityScore(lifeQualityScore), _economyScore(economyScore), _environmentScore(environmentScore) {}
void AddFacility ::act(Simulation &simulation)
{
    FacilityType facToAdd(_facilityName, _facilityCategory, _price, _lifeQualityScore, _economyScore, _environmentScore);
    simulation.addFacility(facToAdd); // asks for object
    complete();
}
AddFacility *AddFacility ::clone() const
{
    return new AddFacility(*this);
    // this method return a pointer to a new AddSettlement object with the same details
}
const string AddFacility ::toString() const {}

// class PrintPlanStatus: public BaseAction {
//     public:
//         PrintPlanStatus(int planId);
//         void act(Simulation &simulation) override;
//         PrintPlanStatus *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
// };

// class ChangePlanPolicy : public BaseAction {
//     public:
//         ChangePlanPolicy(const int planId, const string &newPolicy);
//         void act(Simulation &simulation) override;
//         ChangePlanPolicy *clone() const override;
//         const string toString() const override;
//     private:
//         const int planId;
//         const string newPolicy;
// };

// class PrintActionsLog : public BaseAction {
//     public:
//         PrintActionsLog();
//         void act(Simulation &simulation) override;
//         PrintActionsLog *clone() const override;
//         const string toString() const override;
//     private:
// };

// class Close : public BaseAction {
//     public:
//         Close();
//         void act(Simulation &simulation) override;
//         Close *clone() const override;
//         const string toString() const override;
//     private:
// };

BackupSimulation::BackupSimulation() {}
void BackupSimulation::act(Simulation &simulation)
{
    if (backUp)
    {
        delete backUp;
    }
    backUp = new Simulation(simulation);
    complete();
}
BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}
const string BackupSimulation::toString() const
{
    return "simulation backed up Sucsesfully";
}

RestoreSimulation::RestoreSimulation()
{
    if (backUp == nullptr)
    {
        error("no backup available");
        cout << "ERROR: " + getErrorMsg();
        return; // breaks the function, doesnt continue
    }
}
void RestoreSimulation::act(Simulation &simulation)
{
    simulation = *backUp;
    delete backUp;
    complete();
}
RestoreSimulation *RestoreSimulation::clone() const
{
    return new RestoreSimulation(*this);
}
const string RestoreSimulation::toString() const
{
    return "simulation restored";
}