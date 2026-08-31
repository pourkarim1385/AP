#pragma once

#include <string>

#include "Match.h"
#include "Types.h"

using namespace std;

class Invitation {
    private:
    int id;
    string sender;
    string receiver;
    MatchType type;
    public:
    Invitation(int id, const string& sender, const string& receiver, MatchType type)
                : id(id), sender(sender), receiver(receiver), type(type) {};
    string getSender() const{
        return sender;
    }
    string getReceiver() const{
        return receiver;
    }
    MatchType getType() const {
        return type;
    }
};