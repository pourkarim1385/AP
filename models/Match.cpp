#include "Match.h"

#include <sstream>
#include "ServiceException.h"
#include "../DAL/Repository.h"

Match::Match(int id, const vector<string>& players) : playersUsername(players), id(id){
    status = onGoing;
    turn = 1;
}

CasualMatch::CasualMatch(int id, const vector<string> &players) : Match(id, players) {
    type = casual;
    status = onGoing;
    turn = 1;
    for (auto& player : players) {
        MatchPlayer tempPlayer;
        tempPlayer.name = player;
        tempPlayer.bullets = 1;
        tempPlayer.lives = 1;
        tempPlayer.moves.push_back("");
        tempPlayer.moves.push_back("");
        matchPlayers[player] = tempPlayer;
    }
}

RankedMatch::RankedMatch(const int id, const vector<string> &players, Repository& repo) : Match(id, players) {
    type = ranked;
    status = onGoing;
    turn = 1;
    for (auto& playerUsername : players) {
        const auto player = static_pointer_cast<Player>(repo.getUserByName(playerUsername));
        const int bulletPenalty = player->getPenalty(PenaltyType::Bullet);
        const int headPenalty = player->getPenalty(PenaltyType::Health);

        MatchPlayer tempPlayer;
        tempPlayer.name = playerUsername;
        tempPlayer.bullets = 3 - bulletPenalty;
        tempPlayer.lives = 3 - headPenalty;
        tempPlayer.moves.push_back("");
        tempPlayer.moves.push_back("");
        matchPlayers[playerUsername] = tempPlayer;
    }
}

bool Match::playerHasMoved(const string& player) {
    if (matchPlayers[player].moves[turn].empty())
        return false;
    return true;
}

void Match::updatePlayerMoves(const string &player, const string &move) {
    if (playerHasMoved(player)) {
        throw ServiceException(ErrorType::PermissionDenied, "Player has already moved");
    }
    matchPlayers[player].moves[turn] = move;
}

void Match::addBullet(const string& player) {
    matchPlayers[player].bullets++;
}

bool Match::playerHasBullet(const string& player) {
    return matchPlayers[player].bullets > 0;
}

bool Match::turnIsFinished() {
    for (auto& player : matchPlayers) {
        if (!playerHasMoved(player.first)) {
            return false;
        }
    }
    return true;
}

int RankedMatch::getWinnerHealthBonus() {
    const string winner = getRoundWinner();
    return matchPlayers[winner].lives * 25;
}

void Match::deductBullet(const string& player) {
    matchPlayers[player].bullets--;
}

MatchType Match::getType() {
    return type;
}

void Match::goNextRound() {
    if (!turnIsFinished())
        throw ServiceException(ErrorType::BadRequest, "Turn is not finished");
    turn++;
    for (auto& player : matchPlayers) {
        player.second.moves.push_back("");
    }
}

void Match::closeMatch() {
    winner = getRoundWinner();
    status = MatchStatus::finished;
}

string CasualMatch::getStatus(const string& playerName) {
    ostringstream result;
    result << "Turn " << turn << endl;

    string opponentName = "";
    for (const auto& name : playersUsername) {
        if (name != playerName) {
            opponentName = name;
            break;
        }
    }

    auto& me = matchPlayers[playerName];
    auto& opponent = matchPlayers[opponentName];

    string myCurrentStatus = playerHasMoved(playerName) ? me.moves.back() : "pending";
    string oppCurrentStatus = playerHasMoved(opponentName) ? "played" : "pending";
    result << "You: " << myCurrentStatus << endl << "Your opponent: " << oppCurrentStatus << endl << "History:" << endl;
    result << "Opponent's moves:   Your moves:" << endl;

    for (int turnIndex = 1; turnIndex < turn; turnIndex++) {
        string oppMove = matchPlayers[opponentName].moves[turnIndex];
        string myMove = matchPlayers[playerName].moves[turnIndex];
        result << oppMove;
        for (int i = 0; i < 20 - oppMove.length(); i++)
            result << " ";
        result << myMove << endl;
    }
    result << "Your remaining bullets: " << me.bullets << endl;
    return result.str();
}

string Match::getRoundWinner() {
    if (playersUsername.size() < 2) return "";

    const string p1 = playersUsername[0];
    const string p2 = playersUsername[1];

    const int p1Lives = matchPlayers[p1].lives;
    const int p2Lives = matchPlayers[p2].lives;

    if (p1Lives <= 0 && p2Lives <= 0) {
        return "draw";
    } else if (p1Lives <= 0) {
        return p2;
    } else if (p2Lives <= 0) {
        return p1;
    }

    return "draw";
}

void Match::applyRoundChange() {
    string p1 = playersUsername[0];
    string p2 = playersUsername[1];

    auto& mp1 = matchPlayers[p1];
    auto& mp2 = matchPlayers[p2];

    string m1 = mp1.moves[turn];
    string m2 = mp2.moves[turn];

    if (m1 == "shoot") mp1.bullets--;
    else if (m1 == "reload") mp1.bullets++;

    if (m2 == "shoot") mp2.bullets--;
    else if (m2 == "reload") mp2.bullets++;

    if (m1 == "shoot" && m2 != "defend" && m2 != "shoot") {
        mp2.lives--;
    }
    else if (m2 == "shoot" && m1 != "defend" && m1 != "shoot") {
        mp1.lives--;
    }
}

string RankedMatch::getStatus(const string& playerName) {
    ostringstream result;
    result << "Turn " << turn << endl;

    string opponentName = "";
    for (const auto& name : playersUsername) {
        if (name != playerName) {
            opponentName = name;
            break;
        }
    }

    auto& me = matchPlayers[playerName];
    auto& opponent = matchPlayers[opponentName];

    string myCurrentStatus = playerHasMoved(playerName) ? me.moves.back() : "pending";
    string oppCurrentStatus = playerHasMoved(opponentName) ? "played" : "pending";
    result << "You: " << myCurrentStatus << endl << "Your opponent: " << oppCurrentStatus << endl << "History:" << endl;
    result << "Opponent's moves:   Your moves:" << endl;

    for (int turnIndex = 1; turnIndex < turn; turnIndex++) {
        string oppMove = matchPlayers[opponentName].moves[turnIndex];
        string myMove = matchPlayers[playerName].moves[turnIndex];
        result << oppMove;
        for (int i = 0; i < 20 - oppMove.length(); i++)
            result << " ";
        result << myMove << endl;
    }
    result << "Your remaining bullets: " << me.bullets << endl;
    result << "Your remaining health: " << me.lives << endl;

    return result.str();
}