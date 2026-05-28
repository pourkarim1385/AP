#ifndef TRY1_MATCHMAKINGSERVICE_H
#define TRY1_MATCHMAKINGSERVICE_H
#include "../DAL/Repository.h"


enum SortType {
    ascending ,descending
};

class MatchMakingService {
    private:
    Repository& repo;
    int lastId;
    int generateNextId();
    public:
    explicit MatchMakingService(Repository& repo) : repo(repo) {lastId = 0;};
    string getOpponentList(const string& player,const matchType type,const SortType sortType);
    void updatePlayerStatus(const string& player,const matchType type,const playerStatus& playerStatus);
    void startMatch(vector<string>& players, matchType type);
};


#endif //TRY1_MATCHMAKINGSERVICE_H