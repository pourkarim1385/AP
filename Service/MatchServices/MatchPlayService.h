#pragma once

#include "MatchService.h"
#include "../../DAL/Repository.h"
#include "../../models/Match.h"
#include "../../models/ServiceException.h"


class MatchPlayService {
    private:
    Repository& repo;
    MatchService matchService;
    public:
    explicit MatchPlayService(Repository& repo) : repo(repo), matchService(repo) {};
    void executePlayerAction(int matchId, const string& playerName, const string &action);
    string getMatchStatus(int matchId, const string& playerName, PrivilegeLevel privilegeLevel);
};