#ifndef TRY1_INVITATION_H
#define TRY1_INVITATION_H
#include <string>

#include "Match.h"

using namespace std;

class Invitation {
    private:
    int id;
    string sender;
    string receiver;
    matchType type;
    public:
    Invitation(int id, const string& sender, const string& receiver, matchType type)
                : id(id), sender(sender), receiver(receiver), type(type) {};
    string getSender() const{
        return sender;
    }
    string getReceiver() const{
        return receiver;
    }
    matchType getType() const {
        return type;
    }
};


#endif //TRY1_INVITATION_H