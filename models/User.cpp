#include "User.h"
#include <algorithm>
#include <sstream>

#include "ServiceException.h"


void Player::addSentInvite(const int id) {
    sentInvitesId.push_back(id);
}

void Player::addPendingInvite(const int id) {
    pendingInvitesId.push_back(id);
}

void Player::removeSentInvite(const int id) {
    auto it = find(sentInvitesId.begin(), sentInvitesId.end(), id);
    if (it != sentInvitesId.end()) {
        sentInvitesId.erase(it);
    }
}

void Player::removePendingInvite(const int id) {
    auto it = find(pendingInvitesId.begin(), pendingInvitesId.end(), id);
    if (it != pendingInvitesId.end()) {
        pendingInvitesId.erase(it);
    }
}

playerStatus Player::getStatus(matchType type) const {
    if (type == casual) {
        return casualStatus;
    }
    return unready;
}

void Player::makeReady(matchType type) {
    if (type == casual) {
        casualStatus = ready;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "match type didn`t found");
    }
}

void Player::makeUnready(matchType type) {
    if (type == casual) {
        casualStatus = unready;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "match type didn`t found");
    }
}

const vector<int>& Player::getPendingInvites() const {
    return pendingInvitesId;
}
const vector<int>& Player::getSentInvites() const{
    return sentInvitesId;
}

void Player::leaveMatch() {
    if (isInMatch())
        onGoingMatchId = 0;
    throw ServiceException(ErrorType::BadRequest, "leaving match");
}
void Player::joinMatch(int id) {
    if (isInMatch())
        throw ServiceException(ErrorType::BadRequest, "joining match");

    onGoingMatchId = id;
}

bool Player::isInMatch() const {
    return onGoingMatchId != 0;
}

int Player::getOnGoingMatch() const {
    return onGoingMatchId;
}

void Player::addXP(int amount) {
    xp += amount;
}
void Player::deductXP(int amount) {
    xp -= amount;
}

void Player::addLose() {
    stat.totalLosses++;
}
void Player::addWin() {
    stat.totalWins++;
}

string User::getProfile() const {
    ostringstream result;
    result << "username: " << '"' << name << '"' << endl;
    return result.str();
}

string Player::getProfile() const {
    ostringstream result;
    result << "username: " << '"' << name << '"' << endl;
    result << "XP: " << xp << endl;
    result << "Total wins: " << stat.totalWins << endl;
    result << "Total losses: " << stat.totalLosses << endl;
    return result.str();
}
