#pragma once

#include <map>
#include <string>
#include <vector>
#include "Types.h"
#include "ServiceException.h"
#include "../DAL/Repository.h"

class Repository;
using namespace std;

struct MatchPlayer {
    string name;
    int lives;
    int bullets;
    vector<string> moves;
};

class Match {
protected:
    int id;
    vector<string> playersUsername;
    map<string, MatchPlayer> matchPlayers;
    void addBullet(const string& player);
    void deductBullet(const string& player);
    MatchStatus status;
    MatchType type;
    string winner;
    int turn;

public:
    explicit Match(int id, const vector<string>& players);
    virtual ~Match() = default;

    virtual void updatePlayerMoves(const string& player, const string& move);
    virtual MatchType getType();
    virtual bool isFinished() {return status == MatchStatus::finished;}
    const vector<string>& getPlayersList() {return playersUsername;}
    virtual int getId() const {return id;}

    virtual string getStatus(const string& playerName) = 0;

    virtual bool playerHasMoved(const string& player);
    virtual bool playerHasBullet(const string& player);
    virtual bool turnIsFinished();
    virtual string getRoundWinner();
    virtual void goNextRound();
    virtual void closeMatch();
    virtual void applyRoundChange();
    virtual int getWinnerHealthBonus() {throw ServiceException(ErrorType::BadRequest, "Not supported");}
};

class CasualMatch : public Match {
    public:
    CasualMatch(int id, const vector<string>& players);
    ~CasualMatch() = default;
    string getStatus(const string& playerName) override;
};

class RankedMatch : public Match {
public:
    RankedMatch(int id, const vector<string>& players, Repository& repo);
    ~RankedMatch() = default;

    int getWinnerHealthBonus() override;
    string getStatus(const string& playerName) override;
};