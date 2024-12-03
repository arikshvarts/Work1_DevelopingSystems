// #include "Simulation.h"
// #include "Facility.h"
#include "../include/Facility.h"
#include "../include/Settlement.h"

#include <iostream>

using namespace std;

// Simulation* backup = nullptr;

int main(int argc, char** argv){
    // if(argc!=2){
    //     cout << "usage: simulation <config_path>" << endl;
    //     return 0;
    // }
    // string configurationFile = argv[1];
    // Simulation simulation(configurationFile);
    // simulation.start();
    // if(backup!=nullptr){
    // 	delete backup;
    // 	backup = nullptr;
    // }
<<<<<<< Updated upstream
    string name = "avivEli";
    string settlementName = "BeitarJerusalem";
    FacilityCategory category = FacilityCategory::ECONOMY;
    cout << "aviv eli goat";

    cout << "aviv eli goat";
    int i=0;
    Facility fac=Facility(name,settlementName,category,55,55,5,55);
    cout << fac.getCost();

    cout << fac.getLifeQualityScore();

      cout << "aaab" << endl;
    Settlement s1("Beer Sheva", SettlementType(1));
    cout << s1.toString() << endl;
=======
    // string name = "avivEli";
    // string settlementName = "BeitarJerusalem";
    // FacilityCategory category = FacilityCategory::ECONOMY;
    // SettlementType type = SettlementType::CITY;

    // cout << "aviv eli goat";

    // cout << "aviv eli goat";
    // int i=0;
    // Settlement set(name,type);
    // Facility fac(name,settlementName,category,55,55,5,55);
    // cout << fac.getCost();

    // cout << fac.getLifeQualityScore();
    // Simulation simulation("config_file.txt");
    // simulation.printInitialState();
    // simulation.start();

   const string s = "Set_Name";
    Settlement settlement = Settlement(s, SettlementType::METROPOLIS);
    cout << settlement.getName() << endl;

    // Create FacilityTypes
    FacilityType facilityT = FacilityType("setty", FacilityCategory::ECONOMY, 20, 3, 7, 5);  // 4,8,7=3
    FacilityType facilityT2 = FacilityType("setty2", FacilityCategory::ENVIRONMENT, 3, 6, 4, 6);  // 7,5,8=3
    FacilityType facilityT3 = FacilityType("setty3", FacilityCategory::ENVIRONMENT, 8, 2, 3, 0);  // 3,4,2=2
    FacilityType facilityT4 = FacilityType("setty4", FacilityCategory::ECONOMY, 20, 5, 5, 4);  // 3,4,6=3
    FacilityType facilityT5 = FacilityType("setty5", FacilityCategory::ENVIRONMENT, 20, 4, 7, 3);  // 5,8,5=3
    FacilityType facilityT6 = FacilityType("setty6", FacilityCategory::ECONOMY, 20, 4, 4, 4);  // 5,5,6=1

    // // Create Facilities
    Facility facility = Facility("Fac_Name", settlement.getName(),
        FacilityCategory::LIFE_QUALITY, 10, 3, 5, 2);
    Facility facility2 = Facility("def", settlement.getName(),
        FacilityCategory::LIFE_QUALITY, 10, 3, 5, 2);
    cout << facility.toString() << endl;

    // // Create vector of FacilityTypes
    vector<FacilityType> facilitiesOptions = { facilityT, facilityT2, facilityT3,
        facilityT4, facilityT5, facilityT6 };

    // Create BalancedSelection policy
    BalancedSelection bs = BalancedSelection(1, 1, 2);
    cout << bs.selectFacility(facilitiesOptions).getName() << endl;


    // // Create Plan and execute steps
    Plan p1 = Plan(100, settlement, new NaiveSelection(), facilitiesOptions);
    cout << p1.toString() << endl;
    cout << p1.getEconomyScore() << endl;
    // cout << "step 1:" << endl;
    // p1.step();
    // cout << "step 2:" << endl;
    // p1.step();
    // p1.setSelectionPolicy(new SustainabilitySelection());
    // cout << "step 3:" << endl;
    // p1.step();
    // cout << "step 4:" << endl;
    // p1.step();
    // // cout << "step 5"<< endl;
    // p1.step();
    // cout << "step 6:" << endl;
    // p1.step();
    // Settlement set1 = Settlement("metro", SettlementType::METROPOLIS);
    // cout << int(set1.getType());
    // // cout << set1.toString();

    return 0;
>>>>>>> Stashed changes
}