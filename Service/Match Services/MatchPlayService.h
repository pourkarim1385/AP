#ifndef TRY1_MATCHPLAYSERVICE_H
#define TRY1_MATCHPLAYSERVICE_H
#include "CasualMatchService.h"
#include "../../DAL/Repository.h"
#include "../../models/Match.h"
#include "../../models/ServiceException.h"


class MatchPlayService {
    private:
    Repository& repo;
    CasualMatchService casualMatchService;
    public:
    explicit MatchPlayService(Repository& repo) : repo(repo), casualMatchService(repo) {};
    void executePlayerAction(int matchId, const string& playerName, const string &action);
    string getMatchStatus(int matchId, const string& playerName);
};

#endif //TRY1_MATCHPLAYSERVICE_H