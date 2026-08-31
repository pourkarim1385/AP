#pragma once

#include "../../DAL/Repository.h"

class MatchService {
private:
    void shoot(const string& playerName);
    void defend(const string& playerName);
    void reload(const string& playerName);
    void checkWin();

    void updateXp(const string& winnerName, const string& loserName);
    void updateRp(const string& winnerName, const string& loserName);

    const int BRONZE_DELTA_RP = 75;
    const int SILVER_DELTA_RP = 100;
    const int GOLD_DELTA_RP = 125;
    const int PLAT_DELTA_RP = 150;

    
    const float CASUAL_WIN_RATE = 0.1;
    const int CASUAL_WIN_INTERCEPT = 50;
    const int CASUAL_WIN_INF = 5;

protected:
    Repository& repo;
    std::shared_ptr<Match> match;

public:
    explicit MatchService(Repository& repo) : repo(repo) {};
    virtual ~MatchService() = default;

    void handleAction(shared_ptr<Match> match, const string& action, const string& playerName);
};