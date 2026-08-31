#pragma once

#include "../DAL/Repository.h"

class MatchMakingService {
    private:
    Repository& repo;
    int lastId;
    int generateNextId();
    public:
    explicit MatchMakingService(Repository& repo) : repo(repo), lastId(0) {};
    string getOpponentList(const string& player,const MatchType type,const SortType sortType);
    void updatePlayerStatus(const string& player,const MatchType type,const playerStatus& playerStatus);
    void startMatch(vector<string>& players, MatchType type);
};