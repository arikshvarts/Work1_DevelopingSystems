// #include "Simulation.h"
// #include "Facility.h"
#include "../include/Facility.h"
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
    cout << "aviv eli goat";

    cout << "aviv eli goat";

    Facility fac=Facility(name,settlementName,category,10,0,0,0);
    cout << "aviv eli goat";

    cout << "aviv eli goat";
}