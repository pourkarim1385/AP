#ifndef TRY1_INVITESERVICE_H
#define TRY1_INVITESERVICE_H
#include <memory>

#include "MatchMakingService.h"
#include "../DAL/Repository.h"
#include "../models/ServiceException.h"

using namespace std;

class InviteService {
    private:
    Repository& repo;
    MatchMakingService& matchMakingService;
    int lastId;

    int generateNextId();
    void addInvite(const string& from, const string& to, matchType type);
    string matchTypeToString(matchType type) const;
public:
    InviteService(Repository& repo, MatchMakingService& matchMakingService);
    void newInvite(const string& from, const string& to, const string& type);
    void accpeptInvite(int id);
    void rejectInvite(int id);
    string getPendingInvites(const string& user) const;
};


#endif //TRY1_INVITESERVICE_H