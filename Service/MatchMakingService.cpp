#include "MatchMakingService.h"
#include <sstream>
#include <algorithm>

#include "../models/ServiceException.h"
#include "../models/Match.h"

string MatchMakingService::getOpponentList(const string& player,const MatchType type, const SortType sortType) {
    vector<pair<string, int>> opponents;
    const auto& thisUser = repo.getUserByName(player);
    for (auto& opponent : repo.getPlayersList()) {
        if (opponent->getStatus(type) == ready && opponent->getName() != player) {
            if(type == ranked && opponent->getRanked() != thisUser->getRanked()) continue;
            const int score = (type == casual) ? opponent->getXP() : opponent->getRP();
            pair<string, int> tempOpponent = {opponent->getName(), score};
            opponents.push_back(tempOpponent);
        }
    }

    if(opponents.empty()){
        return "Empty\n";
    }

    sort(opponents.begin(), opponents.end(), [sortType](const pair<string, int>& a, const pair<string, int>& b) {
        if (a.second != b.second) {
            return (sortType == ascending) ? a.second < b.second : a.second > b.second;
        }

        int minLen = min(a.first.length(), b.first.length());
        for (int i = 0; i < minLen; ++i) {
            unsigned char charA = tolower(a.first[i]);
            unsigned char charB = tolower(b.first[i]);
            if (charA != charB) {
                return charA < charB;
            }
        }
        return a.first.length() < b.first.length();
    });


    int index = 1;
    
    ostringstream result;
    for (auto& [username, xp] : opponents) {
        if (type == casual) {
            result << index << ". " << username << " with " << xp << " XP" << endl;
        }else {
            result << index << ". " << username << " with " << xp << " RP" << endl;
        }
        index++;
    }
    return result.str();
}

void MatchMakingService::updatePlayerStatus(const string &player, const MatchType type, const playerStatus &playerStatus) {
    const shared_ptr<User> user = repo.getUserByName(player);
    if (!user) {
        throw ServiceException(ErrorType::NotFound, "Player does not exist");
    }
    if (user->getPrivilegeLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Cannot invite admin");
    }
    
    user->updateReadyStatus(type, playerStatus);
}

void MatchMakingService::startMatch(vector<string>& players,const MatchType type) {
    const int id = generateNextId();
    for (auto& player : players) {
        repo.getUserByName(player)->joinMatch(id);
    }

    switch (type) {
        case MatchType::casual: {
            repo.addMatch(make_shared<CasualMatch>(id, players));
            break;
        }
        case MatchType::ranked: {
            repo.addMatch(make_shared<RankedMatch>(id, players, repo));
            break;
        }
        default: {
            throw ServiceException(ErrorType::BadRequest, "Invalid match type");
        }
    }
}

int MatchMakingService::generateNextId() {
    return ++lastId;
}