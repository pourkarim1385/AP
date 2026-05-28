#include "MatchMakingService.h"
#include <sstream>
#include <algorithm>

#include "../models/ServiceException.h"

string MatchMakingService::getOpponentList(const string& player,const matchType type, const SortType sortType) {
    vector<pair<string, int>> opponents;
    for (auto& opponent : repo.getPlayersList()) {
        if (opponent->getStatus(type) == ready) {
            pair<string, int> tempOpponent = {opponent->getName(), opponent->getXP()};
            opponents.push_back(tempOpponent);
        }
    }

    sort(opponents.begin(), opponents.end(), [sortType](const pair<string, int>& opponentA, const pair<string, int>& opponentB) {
        if (sortType == ascending) {
            return opponentA.second < opponentB.second;
        }
        else {
            return opponentA.second > opponentB.second;
        }
    });

    int index = 1;
    ostringstream result;

    for (auto& [username, xp] : opponents) {
        result << index << ". " << username << " with " << xp << "XP" << endl;
        index++;
    }
    return result.str();
}

void MatchMakingService::updatePlayerStatus(const string &player, const matchType type, const playerStatus &playerStatus) {
    shared_ptr<User> user = repo.getUserByName(player);
    if (!user) {
        throw ServiceException(ErrorType::NotFound, "Player does not exist");
    }
    if (user->getPriverLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Cannot invite admin");
    }
    shared_ptr<Player> targetPlayer = dynamic_pointer_cast<Player>(user);
    if (playerStatus == ready) {
        targetPlayer->makeReady(type);
        return;
    }
    if (playerStatus == unready) {
        targetPlayer->makeUnready(type);
        return;
    }

    throw ServiceException(ErrorType::BadRequest, "status type didn`t found");
}

void MatchMakingService::startMatch(vector<string>& players, matchType type) {
    switch (type) {
        case matchType::casual: {
            int id = generateNextId();
            for (auto& player : players) {
                repo.getUserByName(player)->joinMatch(id);
            }
            repo.addMatch(id, players);
            break;
        }
        default: {
            throw ServiceException(ErrorType::BadRequest, "Invalid match type");
            break;
        }
    }
}

int MatchMakingService::generateNextId() {
    return ++lastId;
}