#ifndef TRY1_USER_H
#define TRY1_USER_H
#include <string>
#include <vector>

#include "Match.h"

using namespace std;

enum PrivilegeLevel {
    adminAccess, normal,
};

enum playerStatus {
    unready, ready
};

class User {
protected:
    string name;
    string password;
    PrivilegeLevel privilegeLevel;
    public:
    explicit  User(const string& username, const string& password, PrivilegeLevel privilegeLevel) :
                name(username), password(password), privilegeLevel(privilegeLevel) {};
    bool matchesPassword(const string& pass) const{
        return password == pass;
    }

    PrivilegeLevel getPrivilegeLevel() const {return privilegeLevel;}
    string getName() const {return name;}
    PrivilegeLevel getPriverLevel() const {return privilegeLevel;}

    virtual string getProfile() const;
    virtual void addSentInvite(const int id){}
    virtual void addPendingInvite(const int id){}
    virtual void removeSentInvite(const int id){}
    virtual void removePendingInvite(const int id){}
    virtual void makeReady(matchType type){}
    virtual void makeUnready(){}
    virtual void leaveMatch(){}
    virtual void joinMatch(int id){}
    virtual int getOnGoingMatch() const{}
    virtual bool isInMatch() const{}
    virtual void addXP(int amount){}
    virtual void deductXP(int amount){}
    virtual void addWin(){}
    virtual void addLose(){}
};

struct playerStats {
    int totalWins;
    int totalLosses;
};

class Player : public User {
    private:
    int xp;
    playerStatus casualStatus;
    vector<int> sentInvitesId;
    vector<int> pendingInvitesId;
    playerStats stat;
    int onGoingMatchId;
    public:
    Player(const string& username, const string& password,const int xp = 500) : User(username, password, normal), xp(xp), stat(0,0) {
        onGoingMatchId = 0;
        casualStatus = unready;
    };
    void addSentInvite(const int id);
    void addPendingInvite(const int id);
    void removeSentInvite(const int id);
    void removePendingInvite(const int id);
    int getXP() const {return xp;};
    playerStatus getStatus(matchType type) const;
    virtual void makeReady(matchType type);
    virtual void makeUnready(matchType type);
    const vector<int>& getSentInvites() const;
    const vector<int>& getPendingInvites() const;
    virtual void leaveMatch();
    virtual void joinMatch(int id);
    virtual bool isInMatch() const;
    virtual int getOnGoingMatch() const;
    virtual void addXP(int amount);
    virtual void deductXP(int amount);
    virtual void addWin();
    virtual void addLose();
    virtual string getProfile() const;
};

class Admin : public User {
public:
    Admin(const string& username, const string& password) : User(username, password, adminAccess) {};
};

#endif //TRY1_USER_H