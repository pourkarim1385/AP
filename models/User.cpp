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

playerStatus Player::getStatus(const MatchType type) const {
    if (type == casual) {
        return casualStatus;
    }
    if (type == ranked) {
        return rankedStatus;
    }
    return unready;
}

void Player::updateReadyStatus(const MatchType type, const playerStatus status) {
    switch (type) {
        case casual: {
            casualStatus = status;
            break;
        }
        case ranked: {
            rankedStatus = status;
            break;
        }
        default:
            throw ServiceException(ErrorType::BadRequest, "match type didn't found");
    }
}

void Player::applyMatchResult(const MatchType type, const MatchResult result, const int changeAmount) {
    switch (type) {
        case casual: {
            if (result == win) {
                addWin();
                addXP(changeAmount);
            }
            else {
                addLose();
                deductXP(changeAmount);
            }
            break;
        }
        case ranked: {
            if (result == win) {
                addWin();
                addRP(changeAmount);
                updateRankLevel();
            }
            else {
                addLose();
                deductRP(changeAmount);
                updateRankLevel();
            }
            break;
        }
        default:
            throw ServiceException(ErrorType::BadRequest, "match type didn't found");
    }
}

const vector<int>& Player::getPendingInvites() const {
    return pendingInvitesId;
}

const vector<int>& Player::getSentInvites() const {
    return sentInvitesId;
}

void Player::leaveMatch() {
    if (isInMatch()) {
        onGoingMatchId = 0;
    } else {
        throw ServiceException(ErrorType::BadRequest, "leaving match");
    }
}

void Player::joinMatch(int id) {
    if (isInMatch()) {
        throw ServiceException(ErrorType::BadRequest, "error joining match");
    }
    onGoingMatchId = id;
}

bool Player::isInMatch() const {
    return onGoingMatchId != 0;
}

int Player::getOnGoingMatch() const {
    return onGoingMatchId;
}

void Player::addXP(const int amount) {
    stat.xp += amount;
}

void Player::deductXP(const int amount) {
    stat.xp -= amount;
}

void Player::addRP(const int amount) {
    stat.rp += amount;
}

void Player::deductRP(const int amount) {
    stat.rp -= amount;
}

void Player::addLose() {
    stat.totalLosses++;
}

void Player::addWin() {
    stat.totalWins++;
}

string User::getProfile() const {
    ostringstream result;
    result << "username: \"" << name << "\"\n";
    return result.str();
}

void Player::updateRankLevel() {
    if (stat.rp < 1400) {
        stat.rank = RankLevel::Bronze;
    }
    else if (stat.rp < 1750) {
        stat.rank = RankLevel::Silver;
    }
    else if (stat.rp < 2250) {
        stat.rank = RankLevel::Gold;
    }
    else {
        stat.rank = RankLevel::Plat;
    }
}

string Player::getProfile() const {
    ostringstream result;
    result << "username: \"" << name << "\"\n";
    result << "Level: " << stat.rank << "\n";
    result << "RP: " << stat.rp << "\n";
    result << "XP: " << stat.xp << "\n";
    result << "Total wins: " << stat.totalWins << "\n";
    result << "Total losses: " << stat.totalLosses << "\n";
    return result.str();
}

bool Player::hasBlocked(const string& username) {
    if (const auto& it = find(blockedUsers.begin(), blockedUsers.end(), username); it != blockedUsers.end()) {
        return true;
    }
    return false;
}

void Player::addBlocked(const string& username) {
    blockedUsers.push_back(username);
}

void Player::unblock(const string& username) {
    if (!hasBlocked(username)) {
        throw ServiceException(ErrorType::NotFound, "user not blocked");
    }
    blockedUsers.erase(find(blockedUsers.begin(), blockedUsers.end(), username));
}

void Player::addPenalty(const PenaltyType type, const int amount, const int matches) {
    penalties.insert_or_assign(type, Penalty(type, amount, matches));
}

int Player::getPenalty(PenaltyType type) {
    auto it = penalties.find(type);
    if (it != penalties.end()) {
        it->second.decrementMatch();

        int amount = it->second.getAmount();

        if (it->second.isExpired()) {
            penalties.erase(it);
        }

        return amount;
    }
    return 0;
}

map<PenaltyType, Penalty> Player::getPenalties() const {
    return penalties;
}