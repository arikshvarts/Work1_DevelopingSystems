#include <string>
#include <vector>
#include "../include/Action.h"
#include <iostream>
using namespace std;

extern Simulation *backup;

BaseAction::BaseAction() : _errorMsg(" "), status(ActionStatus::ERROR) {}

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

SimulateStep ::SimulateStep(const int numOfSteps) : BaseAction(), _numOfSteps(numOfSteps) {}
void SimulateStep ::act(Simulation &simulation)
{
    for (int i=0; i<_numOfSteps; i+=1)
    {
        simulation.step();
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

AddPlan ::AddPlan(const string &settlementName, const string &selectionPolicy) : BaseAction(), _settlementName(settlementName), _selectionPolicy(selectionPolicy) {}
void AddPlan ::act(Simulation &simulation)
{
    if (simulation.isSettlementExists(_settlementName) == true)
    {
        SelectionPolicy *policy; // declare policy without initalize, initalize only when we
        if (_selectionPolicy == "nve")
        {
            policy = new NaiveSelection();
        }
        else if(_selectionPolicy == "bal") {policy = new BalancedSelection(0,0,0);}
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

AddSettlement ::AddSettlement(const string &settlementName, SettlementType settlementType) : BaseAction(), _settlementName(settlementName), _settlementType(settlementType) {}
void AddSettlement ::act(Simulation &simulation)
{
    if (simulation.isSettlementExists(_settlementName) == false)
    {
        simulation.addSettlement(new Settlement(_settlementName, _settlementType)); // asks for pointer
        complete();
    }
    else
    {
        error("Settlement already exists");
        cout << "ERROR: " + getErrorMsg();
        return; // breaks the function, doesnt continue
    }
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
    : BaseAction(), _facilityName(facilityName), _facilityCategory(facilityCategory), _price(price), _lifeQualityScore(lifeQualityScore), _economyScore(economyScore), _environmentScore(environmentScore) {}
void AddFacility ::act(Simulation &simulation)
{
    if (simulation.IsFacilityExist(_facilityName) == false)
    {
        FacilityType facToAdd(_facilityName, _facilityCategory, _price, _lifeQualityScore, _economyScore, _environmentScore);
        simulation.addFacility(facToAdd); // asks for object
        complete();
    }
    else
    {
        error("Facility already exists ");
        cout << "ERROR: " + getErrorMsg();
        return; // breaks the function, doesnt continue
    }
}
AddFacility *AddFacility ::clone() const
{
    return new AddFacility(*this);
    // this method return a pointer to a new AddSettlement object with the same details
}
const string AddFacility ::toString() const {
        string statusAct;
    return  "Facility added - " + _facilityName;
}

PrintPlanStatus ::PrintPlanStatus(int planId) :BaseAction(), _planId(planId) {}
void PrintPlanStatus ::act(Simulation &simulation)
{
    if (simulation.IsPlanExist(_planId) == true)
    {
        cout<<simulation.getPlan(_planId).toString(); 

    }
    else
    {
        error("Plan ID doesn't exist");
        cout << "ERROR: " + getErrorMsg();
        return; // breaks the function, doesnt continue
    }
    complete();
}
PrintPlanStatus *PrintPlanStatus ::clone() const
{
    return new PrintPlanStatus(*this);
    // this method return a pointer to a new PrintPlanStatus object with the same details
}
const string PrintPlanStatus ::toString() const
{
    string statusAct;
    return  "Printed Plan Status"+to_string(_planId);
}

PrintActionsLog::PrintActionsLog()
{
}
void PrintActionsLog::act(Simulation &simulation)
{
    for (BaseAction *s : simulation.getActionsLog())
    {
        cout << s->toString();
    }
    complete();
}
PrintActionsLog *PrintActionsLog::clone() const
{
    return new PrintActionsLog(*this);
}
const string PrintActionsLog::toString() const
{
    return "actions log printed";
}

ChangePlanPolicy::ChangePlanPolicy(const int planId, const string &newPolicy) : BaseAction(), planId(planId), newPolicy(newPolicy)
{
}
void ChangePlanPolicy::act(Simulation &simulation)
{
    Plan &curr_plan = simulation.getPlan(planId);
    string prevPolicy = curr_plan.getSelectionPolicy();
    SelectionPolicy *policy=nullptr;

    if (simulation.IsPlanExist(planId) )
    {
        if (newPolicy != simulation.getPlan(planId).getSelectionPolicy()) 
        {
            if (newPolicy == "nve") 
            {
                policy = new NaiveSelection();
            }
            else if (newPolicy == "bal") {
                int lifeQual_kolel = curr_plan.getlifeQualityScore();
                int eco_kolel = curr_plan.getEconomyScore();
                int env_kolel = curr_plan.getEnvironmentScore();
                //we want to send the scores to the balance selectionPolicy considering also the facilities that under construction 
                for (Facility* fac : curr_plan.getUnderConstruction()){
                    lifeQual_kolel += fac->getLifeQualityScore();
                    eco_kolel += fac->getEconomyScore();
                    env_kolel += fac->getEnvironmentScore();                
                }
                policy = new BalancedSelection(lifeQual_kolel, eco_kolel, env_kolel);
            }
            else if (newPolicy == "eco")
            {
                policy = new EconomySelection();
            }
            else if (newPolicy == "env")
            {
                policy = new SustainabilitySelection();
            }
            else{
                error("Cannot change selection policy");
                cout << "ERROR: " + getErrorMsg();
                delete policy;
            }
        }
        else{
            error("Tried to change to the same selection policy");
            cout << "ERROR: " + getErrorMsg();
            delete policy;
            // delete policy;
            }
         simulation.getPlan(planId).setSelectionPolicy(policy);
        
        return; // breaks the function, doesnt continue
    }        
    else
    {
        error("There is no such a plan id");
        cout << "ERROR: " + getErrorMsg();
        delete policy;
        return; // breaks the function, doesnt continue
    }
    delete policy;
}
ChangePlanPolicy *ChangePlanPolicy::clone() const
{
    return new ChangePlanPolicy(*this);
}
const string ChangePlanPolicy::toString() const
{
    return "pland ID:" + to_string(planId) + "previousPolicy:" + "new policy" + newPolicy;
}

Close::Close() : BaseAction() {}
void Close::act(Simulation &simulation)
{
    for (Plan &plan : simulation.getPlansVec())
    {
        {
            cout << plan.toString();
        }
        simulation.close();
    }
}
Close *Close::clone() const
{
    return new Close(*this);
}
const string Close::toString() const
{
    return "closed";
}

BackupSimulation::BackupSimulation() : BaseAction() {}
void BackupSimulation::act(Simulation &simulation)
{
    if (backup)
    {
        delete backup;
    }
    backup = new Simulation(simulation);
    complete();
}
BackupSimulation *BackupSimulation::clone() const
{
    return new BackupSimulation(*this);
}
const string BackupSimulation::toString() const
{
    return "backup COMPLETED";
}

RestoreSimulation::RestoreSimulation() : BaseAction()
{
}
void RestoreSimulation::act(Simulation &simulation)
{
    if (backup == nullptr)
    {
        error("No backup available");
        cout << "ERROR: " + getErrorMsg();
        return;
    }
    simulation = *backup;
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