#include "MatchPlayService.h"
#include "../../models/Match.h"

void MatchPlayService::executePlayerAction(int matchId, const string& playerName, const string &action) {
    if (matchId == 0) {
        throw ServiceException(ErrorType::NotFound, "Match player does not exist");
    }
    shared_ptr<Match> match = repo.getMatchByID(matchId);
    switch (match->getType()) {
        case matchType::casual: {
            shared_ptr<CasualMath> casualMatch = dynamic_pointer_cast<CasualMath>(match);
            casualMatchService.handleAction(casualMatch, action, playerName);
            break;
        }
        default:
            throw ServiceException(ErrorType::NotFound, "Unsupported match type");
    }
}

string MatchPlayService::getMatchStatus(int matchId, const string& playerName) {
    if (matchId == 0) {
        throw ServiceException(ErrorType::NotFound, "Match player does not exist");
    }
    shared_ptr<Match> match = repo.getMatchByID(matchId);
    return match->getStatus(playerName);
}