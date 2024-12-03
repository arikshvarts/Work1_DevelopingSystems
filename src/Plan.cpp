#include "../include/Plan.h"
#include "../include/Facility.h"

#include <vector>
#include <iostream>
#include <sstream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), facilityOptions(facilityOptions), selectionPolicy(selectionPolicy), settlement(settlement), facilities(vector<Facility *>()),
      underConstruction(vector<Facility *>()), life_quality_score(0), economy_score(0), environment_score(0), status(PlanStatus::AVALIABLE)
{
}

Plan::Plan(const Plan &other)
    : plan_id(other.plan_id),
      settlement(other.settlement),
      selectionPolicy(),
      status(other.status),
      facilities(),
      underConstruction(),
      facilityOptions(other.facilityOptions),
      life_quality_score(other.life_quality_score),
      economy_score(other.economy_score),
      environment_score(other.environment_score)
{
    if (other.selectionPolicy != nullptr)
        selectionPolicy = other.selectionPolicy->clone();
    else
        selectionPolicy = nullptr;

    for (Facility *curr : other.facilities)
        facilities.push_back(curr->clone());

    for (Facility *curr : other.underConstruction)
        underConstruction.push_back(curr->clone());
}

Plan::Plan(Plan &&other) : settlement(other.settlement), plan_id(other.plan_id), facilities(other.facilities), underConstruction(other.underConstruction),
                           facilityOptions(other.facilityOptions), life_quality_score(other.life_quality_score), economy_score(other.economy_score), environment_score(other.environment_score), status(other.status), selectionPolicy(other.selectionPolicy)
{
    other.selectionPolicy = nullptr;
    other.facilities.clear();
    other.underConstruction.clear();
}

Plan::~Plan()
{
    delete selectionPolicy;
    for (Facility *instance : underConstruction)
    {
        delete instance; // deletes the object pointed to by the pointer
    }
    for (Facility *instance : facilities)
    {
        delete instance; // deletes the object pointed to by the pointer
    }
    // facilities.clear();// Removes all pointers from the vector
    // underConstruction.clear();// Removes all pointers from the vector
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
}

vector<Facility *> Plan::deepCopyFacilities(const vector<Facility *> &facilities)
{
    vector<Facility *> copiedFacilities;
    for (Facility *facility : facilities)
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
    // delete selectionPolicy;
}
void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        while (this->status == PlanStatus::AVALIABLE)
        {
            const FacilityType &fac = selectionPolicy->selectFacility(facilityOptions);
            Facility *tempFacility = new Facility(fac, settlement.getName());
            addFacility(tempFacility);
        }
    }
    for (int it = 0; it < underConstruction.size();)
    {
        underConstruction[it]->step();
        if (underConstruction[it]->getTimeLeft() == 0)
        {
            economy_score += underConstruction[it]->getEconomyScore();
            environment_score += underConstruction[it]->getEnvironmentScore();
            life_quality_score += underConstruction[it]->getLifeQualityScore();
            Plan::addFacility(underConstruction[it]);
            underConstruction[it]->setStatus(FacilityStatus::OPERATIONAL);
            // Move the instance to the facilities vector
            facilities.push_back(underConstruction[it]);
            // Remove the instance from underConstruction
            underConstruction.erase(underConstruction.begin() + it);
        }
        else
        {
            it += 1;
        }
    }
    if (underConstruction.size() == int(settlement.getType()) + 1)
    {
        status = PlanStatus::BUSY;
    }
    else
    {
        status = PlanStatus::AVALIABLE;
    }
}
void Plan::printStatus()
{
    if (int(status) == 0)
    {
        cout << "AVALIABLE";
    }
    if (int(status) == 1)
    {
        cout << "Busy";
    }
}
vector<Facility *> &Plan::getFacilities() // returned by refernce to ebable modification
{
    return facilities;
}
void Plan::addFacility(Facility *facility)
{
    if (facility->getStatus() == FacilityStatus::OPERATIONAL)
        facilities.push_back(facility);
    else
        underConstruction.push_back(facility);

    if (static_cast<int>(underConstruction.size()) == static_cast<int>(settlement.getType()) + 1)
        this->status = PlanStatus::BUSY;
}

const string Plan::toString() const
{
    string statusString;
    string totalString = "Plan ID: " + to_string(plan_id) + "\n";
    if (status == PlanStatus::BUSY)
        statusString = "Busy";
    else
        statusString = "Available";
    totalString += "Settlement name:" + settlement.getName() + "\n";
    totalString += "Plan Status: " + statusString;

    totalString += "Selection Policy: ";
    totalString += selectionPolicy->toStringSimple() + "\n";
    totalString += "LifeQualityScore: " + to_string(life_quality_score) + "\n" + " EconomyScore: " + to_string(economy_score) + "\n"
                                                                                                                                " EnviromentScore: " +
                   to_string(environment_score);
    for (Facility *curr : facilities)
    {
        totalString += "Facility Name: " + curr->getName() + "\n";
        totalString += "Facility Status: OPERATIONAL \n";
    }

    for (Facility *curr : underConstruction)
    {
        totalString += "Facility Name: " + curr->getName() + "\n";
        totalString += "status: CONSTRUCTIONS \n";
    }
    return totalString;
}

const Settlement &Plan ::getSettlement() const
{
    return settlement;
}

string Plan ::getSelectionPolicy() const
{
    return selectionPolicy->toStringSimple();
}
