#include "Settlement.h"

Settlement::Settlement(const string &name, SettlementType type):name(name)
{
    this->type=type;
}
const string &Settlement::getName() const
{
    return name;
}

SettlementType Settlement::getType() const
{
    return SettlementType();
}


