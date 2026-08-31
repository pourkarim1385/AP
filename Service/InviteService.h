#pragma once

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
    void addInvite(const string& from, const string& to, MatchType type);
    string matchTypeToString(MatchType type) const;
public:
    InviteService(Repository& repo, MatchMakingService& matchMakingService);
    void newInvite(const string& from, const string& to, MatchType type);
    void accpeptInvite(const string& username, int id, PrivilegeLevel privilegeLevel);
    void rejectInvite(const string& username, int id, PrivilegeLevel privilegeLevel);
    string getPendingInvites(const string& user) const;
};