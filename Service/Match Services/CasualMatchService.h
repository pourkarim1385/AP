#ifndef TRY1_CASUALMATCHSERVICE_H
#define TRY1_CASUALMATCHSERVICE_H
#include <memory>

#include "MatchService.h"
#include "../../models/Match.h"


class CasualMatchService : public MatchService{
    private:
    std::shared_ptr<CasualMath> match;
    void validateAction();
    void shoot(const string& playerName);
    void defend(const string& playerName);
    void reload(const string& playerName);
    void updateXp(const string& winnerName, const string& loserName);
    void updateHistory();
    void checkWin();
    int turn;
    public:
    string getStatus();
    CasualMatchService(Repository& repo) : MatchService(repo){};
    void handleAction(shared_ptr<CasualMath> match, const string& action, const string& playerName);
};


#endif //TRY1_CASUALMATCHSERVICE_H