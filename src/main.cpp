#include "../include/Facility.h"
#include "../include/Settlement.h"
#include "../include/Simulation.h"

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
    string name = "avivEli";
    string settlementName = "BeitarJerusalem";
    FacilityCategory category = FacilityCategory::ECONOMY;
    SettlementType type = SettlementType::CITY;

    cout << "aviv eli goat";

    cout << "aviv eli goat";
    int i=0;
    Settlement set(name,type);
    Facility fac(name,settlementName,category,55,55,5,55);
    cout << fac.getCost();

    cout << fac.getLifeQualityScore();
    Simulation simulation("config_file.txt");
    simulation.printInitialState();
    simulation.start();
    return 0;
}