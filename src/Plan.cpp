#include "Plan.h"
#include <vector>
#include <iostream>
#include <sstream>

Plan::Plan(const int planId, const Settlement &settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
    : plan_id(planId), facilityOptions(facilityOptions), selectionPolicy(selectionPolicy), settlement(settlement), facilities(vector<Facility *>()),
      underConstruction(vector<Facility *>()), life_quality_score(0), economy_score(0), environment_score(0), status(PlanStatus::AVALIABLE)
{
}
Plan::Plan(const Plan &other):settlement(other.settlement),plan_id(other.plan_id),
{
    if (this != &other)
    {
        // clear();
        plan_id = other.plan_id;
        if (selectionPolicy)
            delete selectionPolicy;
        selectionPolicy = other.selectionPolicy ? other.selectionPolicy->clone() : nullptr;
        facilities = other.facilities;
        underConstruction = other.underConstruction;
    }
    return *this;
}

Plan::~Plan()
{
    delete selectionPolicy; // check if implement here or in the settelement class itself.
    for (Facility *instance : underConstruction)
    {
        delete instance;
    }
    for (Facility *instance : facilities)
    {
        delete instance;
    }
}

const int Plan::getlifeQualityScore() const
{
    return life_quality_score;
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
    selectionPolicy = selectionPolicy;
}
void Plan::step()
{
    if (status == PlanStatus::AVALIABLE)
    {
        // if selectionPolicy==
        addFacility(fac)
    }
    for (const auto &instance : underConstruction)
    { // Using auto& to avoid unnecessary copying
        instance->timeLeft -= 1;
        if (instance->getTimeLeft() == 0)
            std::cout << instance.value << std::endl;
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
const vector<Facility *> &Plan::getFacilities() const // returned by refernce to ebable modification
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
