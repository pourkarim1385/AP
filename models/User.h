#pragma once

#include <string>
#include <vector>
#include <ostream>

#include "Penalty.h"
#include "Types.h"
#include "ServiceException.h"

using namespace std;

namespace defaultValues {
    const int xp = 500;
    const int rp = 1200;
}

struct playerStats {
    int totalWins;
    int totalLosses;
    int xp;
    int rp;
    RankLevel rank;
};

class User {
protected:
    string name;
    string password;
    PrivilegeLevel privilegeLevel;
public:
    explicit User(const string& username, const string& password, PrivilegeLevel privilegeLevel) :
                name(username), password(password), privilegeLevel(privilegeLevel) {}

    virtual ~User() = default;

    bool matchesPassword(const string& pass) const {
        return password == pass;
    }

    PrivilegeLevel getPrivilegeLevel() const { return privilegeLevel; }
    string getName() const { return name; }

    virtual string getProfile() const;
    
    virtual void addSentInvite(const int id) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void addPendingInvite(const int id) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void removeSentInvite(const int id) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void removePendingInvite(const int id) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void updateReadyStatus(MatchType type, playerStatus status) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void leaveMatch() { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual void joinMatch(int id) { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual int getOnGoingMatch() const { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual bool isInMatch() const { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual const vector<int>& getPendingInvites() const { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual bool hasBlocked(const string& username) {throw ServiceException(ErrorType::BadRequest, "Not supported");}
    virtual void addBlocked(const string& username){throw ServiceException(ErrorType::BadRequest, "Not supported");}
    virtual void unblock(const string& username){throw ServiceException(ErrorType::BadRequest, "Not supported");}
    virtual void applyMatchResult(MatchType type, MatchResult result, int changeAmount) {throw ServiceException(ErrorType::BadRequest, "Not supported");}
    virtual void addPenalty(PenaltyType type, int amount, int matches) {throw ServiceException(ErrorType::BadRequest, "Not supported");}

    virtual RankLevel getRanked() const {throw ServiceException(ErrorType::BadRequest, "Not supported");}
    virtual int getXP() const { throw ServiceException(ErrorType::BadRequest, "Not supported"); }
    virtual int getRP() const { throw ServiceException(ErrorType::BadRequest, "Not supported"); }

};

class Player final : public User {
private:
    playerStatus casualStatus;
    playerStatus rankedStatus;
    vector<int> sentInvitesId;
    vector<int> pendingInvitesId;
    vector<string> blockedUsers;
    playerStats stat;
    int onGoingMatchId;
    map<PenaltyType, Penalty> penalties;

    void updateRankLevel();
    void addXP(int amount);
    void deductXP(int amount);
    void addRP(int amount);
    void deductRP(int amount);
    void addWin();
    void addLose();

public:
    Player(const string& username, const string& password, const int xp = defaultValues::xp, const int rp = defaultValues::rp)
        : User(username, password, normal), casualStatus(unready), rankedStatus(ready), stat{0, 0, xp, rp, RankLevel::Bronze}, onGoingMatchId(0) {
        updateRankLevel();
    }

    void addSentInvite(const int id) override;
    void addPendingInvite(const int id) override;
    void removeSentInvite(const int id) override;
    void removePendingInvite(const int id) override;

    int getXP() const override { return stat.xp; }
    int getRP() const override { return stat.rp; }
    RankLevel getRanked()const override {return stat.rank;}
    playerStatus getStatus(MatchType type) const;

    void updateReadyStatus(MatchType type, playerStatus status) override;
    void applyMatchResult(MatchType type, MatchResult result, int changeAmount) override;

    const vector<int>& getSentInvites() const;
    const vector<int>& getPendingInvites() const;

    void leaveMatch() override;
    void joinMatch(int id) override;
    bool isInMatch() const override;
    int getOnGoingMatch() const override;
    string getProfile() const override;
    bool hasBlocked(const string& username) override;
    void addBlocked(const string& username) override;
    void unblock(const string& username) override;

    void addPenalty(PenaltyType type, int amount, int matches);
    map<PenaltyType, Penalty> getPenalties() const;

    int getPenalty(PenaltyType type);
};

class Admin final : public User {
public:
    Admin(const string& username, const string& password) : User(username, password, adminAccess) {}
};