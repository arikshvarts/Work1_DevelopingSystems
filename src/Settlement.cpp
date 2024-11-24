#include <iostream>
#include "../include/Settlement.h"

    Settlement :: Settlement(const string &name, SettlementType type) : name(name), type(type){}
    const string &Settlement :: getName() const{
        return name;
    }
    SettlementType Settlement ::  getType() const{
        return type;
    }
    const string Settlement ::  toString() const  {
        string types[3] = {"Village","City", "Metropolin"};
        return "Name: " + getName() + "     Type: " + types[int(type)];
    }   