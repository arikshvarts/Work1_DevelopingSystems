#include "../include/Plan.h"
#include "../include/Facility.h"
#include <vector>
#include <iostream>
#include <sstream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), facilityOptions(facilityOptions), selectionPolicy(selectionPolicy), settlement(settlement), facilities(vector<Facility *>()),
      underConstruction(vector<Facility *>()), life_quality_score(0), economy_score(0), environment_score(0), status(PlanStatus::AVALIABLE)
{}

Plan::Plan(const Plan &other):settlement(other.settlement),plan_id(other.plan_id),facilities(deepCopyFacilities(other.facilities)),underConstruction(deepCopyFacilities(other.underConstruction)),
facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), status(other.status),selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr)
{
}

Plan::Plan(Plan &&other):settlement(other.settlement),plan_id(other.plan_id),facilities(deepCopyFacilities(other.facilities)),underConstruction(deepCopyFacilities(other.underConstruction)),
facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), status(other.status),selectionPolicy(other.selectionPolicy ? other.selectionPolicy->clone() : nullptr)
{
other.selectionPolicy=nullptr;
other.facilities.clear();
other.underConstruction.clear();
}

Plan::~Plan()
{
    delete selectionPolicy; 
    for (Facility *instance : underConstruction)
    {
        delete instance;//deletes the object pointed to by the pointer
    }
    for (Facility *instance : facilities)
    {
        delete instance;//deletes the object pointed to by the pointer
    }
    facilities.clear();// Removes all pointers from the vector
    underConstruction.clear();// Removes all pointers from the vector
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

vector<Facility*> Plan::deepCopyFacilities(const vector<Facility*>& facilities)
{
    vector<Facility*> copiedFacilities;
    for (Facility* facility : facilities)
    {
        copiedFacilities.push_back(facility ? facility->clone() : nullptr);
    }
    return copiedFacilities;
}
const int Plan::getPlanId()
{
    return plan_id;
}

const int Plan::getEconomyScore() const
{
    return economy_score;
}

const int Plan::getEnvironmentScore() const
{
    return environment_score;
}
void Plan::setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    this->selectionPolicy = selectionPolicy;
}
void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        while(status == PlanStatus::AVALIABLE)
        {
            FacilityType fac=this->selectionPolicy->selectFacility(facilityOptions);
            Facility tempFacility=Facility(fac, settlement.getName());
            addFacility(&tempFacility);
        }
    }
    for (int it = 0; it < underConstruction.size();) 
    { 
        // Using auto& to avoid unnecessary copying
        underConstruction[it]->step();
        if (underConstruction[it]->getTimeLeft() == 0)
        {
            economy_score+=underConstruction[it]->getEconomyScore();
            environment_score+=underConstruction[it]->getEnvironmentScore();
            life_quality_score+=underConstruction[it]->getLifeQualityScore();
            underConstruction[it]->setStatus(FacilityStatus::OPERATIONAL);
            // Move the instance to the facilities vector
            facilities.push_back(underConstruction[it]);
            // Remove the instance from underConstruction
            underConstruction.erase(underConstruction.begin() + it);
        }
        else
        {
            it+=1;
        }
    }
    if (underConstruction.size()==int(settlement.getType()))
    {
    status =PlanStatus::BUSY;
    }
    else{
        status=PlanStatus::AVALIABLE;
    }
}
void Plan::printStatus()
{
    switch (status)
    {
    case PlanStatus::AVALIABLE:
        cout << "AVALIABLE";
    case PlanStatus::BUSY:
        cout << "BUSY";
    }
}
vector<Facility *> &Plan::getFacilities() // returned by refernce to ebable modification
{
    return facilities;
}
void Plan::addFacility(Facility *facility)
{
    underConstruction.push_back(facility);
    if ((int)(underConstruction.size() - 1) == (int)settlement.getType())
    {
        status = PlanStatus::BUSY;
    }
}

const string Plan::toString() const
{
    std::ostringstream oss;
    oss << "Name: " << plan_id << ", SettlementName: " << settlement.getName() << ", getEnvScore: " << getEnvironmentScore() << "getEconScore: " << getEconomyScore() << "getlifeQualScore: " << getlifeQualityScore();
    return oss.str();
}

const Settlement& Plan :: getSettlement() const{
    return settlement;
}

const SelectionPolicy& Plan :: getSelectionPolicy() const{
    return *selectionPolicy;
}
