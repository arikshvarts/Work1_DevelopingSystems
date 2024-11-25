#include "./include/SelectionPolicy.h";
#include <limits>
#include <algorithm> // For std::max_element or min_element
using namespace std;


NaiveSelection ::NaiveSelection() : lastSelectedIndex(-1){} //initalize last selected index to -1 because after the first selection, +1 to lastselected brings to 0 
const FacilityType& NaiveSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions) {
    lastSelectedIndex = (lastSelectedIndex + 1) % facilitiesOptions.size();
    return facilitiesOptions[lastSelectedIndex];
}
const string NaiveSelection :: toString() const{
        return "Naive Selection, we are selecting the facilities by order. The last selected index is - " + std:: to_string(lastSelectedIndex);
}

NaiveSelection* NaiveSelection :: clone() const {
    //this method return a pointer to a new NaiveSelection object with the same details
    return new NaiveSelection(*this);
}

BalancedSelection :: BalancedSelection(int LifeQualityScore, int EconomyScore, int EnvironmentScore) : LifeQualityScore(LifeQualityScore), EconomyScore(EconomyScore), EnvironmentScore(EnvironmentScore){}
const FacilityType& BalancedSelection :: selectFacility(const vector<FacilityType>& facilitiesOptions){
    const FacilityType *Most_Balanced = nullptr;
    int Min_Difference =  std::numeric_limits<int>::max();  //initalize Min_Difference to be max value for start (ensure the diff in the first facility will be smaller/equal)
    int future_scores[3];
    int future_Difference;
    for (const FacilityType& fac : facilitiesOptions){ //for each FacilityType in options. we choose refernce to prevent a lot of uneffiecient copies
        future_scores[0] = LifeQualityScore + fac.getLifeQualityScore();
        future_scores[1] = EconomyScore + fac.getEconomyScore();
        future_scores[2] = EnvironmentScore + fac.getEnvironmentScore();
        future_Difference = *std::max_element(future_scores, future_scores + 3) - *std::min_element(future_scores, future_scores + 3);
        // max/min return a pointer.. 
        if(future_Difference < Min_Difference){
            Most_Balanced = &fac;
        }
    }
    return *Most_Balanced;
    
}
const string BalancedSelection :: toString() const {
    return "Balanced Selection, we are selecting the facility that minimizes the difference between the maximum and minimum scores";
}
BalancedSelection* BalancedSelection ::  clone() const {

}

