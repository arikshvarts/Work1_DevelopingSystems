#include "SelectionPolicy.h"
#include <sstream>
#include <iostream>
#include <algorithm>

BalancedSelection::BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore):
LifeQualityScore(LifeQualityScore),EconomyScore(EconomyScore),EnvironmentScore(EnvironmentScore)
{
}

const FacilityType &BalancedSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    int counter=0;
    int max_Facility_index=-999;
    int currBiggest=0;
    for (FacilityType instance : facilitiesOptions)
    {
        double ec=EconomyScore+instance.getEconomyScore();
        double env=EnvironmentScore+instance.getEnvironmentScore();
        double lifeQual=LifeQualityScore+instance.getLifeQualityScore();
        if(max({ec,env,lifeQual})-min({ec,env,lifeQual})<currBiggest)
        {
        currBiggest=max({ec,env,lifeQual})-min({ec,env,lifeQual});
        max_Facility_index=counter;
        }
        counter++;
        
    }
    return facilitiesOptions[max_Facility_index];
    }

const string BalancedSelection::toString() const
{
    std::ostringstream oss;
    oss << "LifeQualityScore: " << LifeQualityScore << ", EconomyScore: " << EconomyScore<< ", EnvironmentScore: " << EnvironmentScore;
    return oss.str();    
}

 string BalancedSelection::toStringSimple() const
{
    return "bal";
}


BalancedSelection * BalancedSelection::clone() const
{
    return new BalancedSelection(*this); // Use copy constructor to create a deep copy
}

SustainabilitySelection::SustainabilitySelection():lastSelectedIndex(-1)
{}

const FacilityType &SustainabilitySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    for (size_t i = lastSelectedIndex; i < facilitiesOptions.size(); ++i) {
    
        lastSelectedIndex=(lastSelectedIndex+1)%(facilitiesOptions.size());
        if (facilitiesOptions[lastSelectedIndex].getCategory()==FacilityCategory::ENVIRONMENT)
        {
        return facilitiesOptions[lastSelectedIndex];
        }
}

}

const string SustainabilitySelection::toString() const
{
cout<<"sus";
}

SustainabilitySelection *SustainabilitySelection::clone() const
{
    return new SustainabilitySelection(*this); // Use copy constructor to create a deep copy
}
 string SustainabilitySelection::toStringSimple() const
{
    return "env";
}
NaiveSelection::NaiveSelection():lastSelectedIndex(-1)
{}

const FacilityType &NaiveSelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    lastSelectedIndex=(lastSelectedIndex+1)%(facilitiesOptions.size());
    return facilitiesOptions[lastSelectedIndex];
    }

const string NaiveSelection::toString() const
{
cout<<"nve";
}


NaiveSelection * NaiveSelection::clone() const
{
    return new NaiveSelection(*this); // Use copy constructor to create a deep copy
}
 string NaiveSelection::toStringSimple() const
{
    return "nve";
}

EconomySelection::EconomySelection():lastSelectedIndex(-1)
{}

const FacilityType &EconomySelection::selectFacility(const vector<FacilityType> &facilitiesOptions)
{
    for (size_t i = lastSelectedIndex; i < facilitiesOptions.size(); ++i) {
    
        lastSelectedIndex=(lastSelectedIndex+1)%(facilitiesOptions.size());
        if (facilitiesOptions[lastSelectedIndex].getCategory()==FacilityCategory::ECONOMY)
        {
        return facilitiesOptions[lastSelectedIndex];
        }
}
}
const string EconomySelection::toString() const
{
cout<<"eco";
}
EconomySelection *EconomySelection::clone() const
{
    return new EconomySelection(*this); // Use copy constructor to create a deep copy
}
 string EconomySelection::toStringSimple() const
{
    return "eco";
}