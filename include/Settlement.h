#pragma once
#include <string>
#include <vector>
using std::string;
using std::vector;

enum class SettlementType {
    VILLAGE,
    CITY,
    METROPOLIS,
};

class Settlement {
    public:
        Settlement(const string &name, SettlementType type);
        const string &getName() const;
        SettlementType getType() const;
        const string toString() const;
        Settlement* clone() const;

        private:
            const string name;
            SettlementType type;
};