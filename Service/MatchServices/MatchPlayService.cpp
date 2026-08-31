#include "MatchPlayService.h"
#include "../../models/Match.h"

void MatchPlayService::executePlayerAction(int matchId, const string& playerName, const string &action) {
    if (matchId == 0) {
        throw ServiceException(ErrorType::NotFound, "Match player does not exist");
    }

    matchService.handleAction(repo.getMatchByID(matchId), action, playerName);
}

string MatchPlayService::getMatchStatus(int matchId, const string& playerName, PrivilegeLevel privilegeLevel) {
    if(privilegeLevel == adminAccess)
        throw ServiceException(ErrorType::PermissionDenied, "Admin cannot get match status");

    if (matchId == 0) {
        throw ServiceException(ErrorType::NotFound, "Match player does not exist");
    }
    shared_ptr<Match> match = repo.getMatchByID(matchId);
    return match->getStatus(playerName);
}