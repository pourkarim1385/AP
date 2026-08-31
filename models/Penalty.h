#pragma once
#include "Types.h"

class Penalty {
private:
    PenaltyType type;
    int amount;
    int remainingMatches;
public:
    Penalty(PenaltyType type, int amount, int remainingMatches) : type(type), amount(amount), remainingMatches(remainingMatches) {}

    bool isExpired() const { return remainingMatches <= 0; }
    int getAmount() const { return amount; }
    PenaltyType getType() const { return type; }
    void decrementMatch() { remainingMatches--; }
};
