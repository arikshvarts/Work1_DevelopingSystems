#include "Plan.h"

Plan::Plan(const int planId, const Settlement *settlement, SelectionPolicy *selectionPolicy, const vector<FacilityType> &facilityOptions)
:plan_id(planId),facilityOptions(facilityOptions),selectionPolicy(move(selectionPolicy)),settlement(settlement)
{
    underConstruction=underConstruction;
    life_quality_score=0;
    economy_score=0;
    environment_score=0;
    status=PlanStatus::AVALIABLE;
}  
const int getlifeQualityScore(
    return life_quality_score;
) const;
const int getEconomyScore() const;
const int getEnvironmentScore() const;
void setSelectionPolicy(SelectionPolicy *selectionPolicy)
{
    selectionPolicy=selectionPolicy;
};
void step();
void printStatus();
const vector<Facility*> &getFacilities() const;
void addFacility(Facility* facility)
{
    facilityOptions.
};
const string toString() const;
