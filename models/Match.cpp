#include "Match.h"

#include <sstream>
#include "ServiceException.h"

Match::Match(int id, const vector<string>& players) : playersUsername(players), id(id){
    status = onGoing;
    turn = 1;
}

CasualMath::CasualMath(int id, const vector<string> &players) : Match(id, players) {
    type = casual;
    status = onGoing;
    turn = 1;
    for (auto& player : players) {
        casualPlayer tempPlayer(player);
        tempPlayer.bullets = 1;
        tempPlayer.moves.push_back("");
        tempPlayer.moves.push_back("");
        casualPlayers[player] = tempPlayer;
    }
}

bool CasualMath::playerHasMoved(const string& player) {
    if (casualPlayers[player].moves[turn].empty())
        return false;
    return true;
}

void CasualMath::updatePlayerMoves(const string &player, const string &move) {
    if (playerHasMoved(player)) {
        throw ServiceException(ErrorType::PermissionDenied, "Player has already moved");
    }
    casualPlayers[player].moves[turn] = move;
}

void CasualMath::addBullet(const string& player) {
    casualPlayers[player].bullets++;
}

bool CasualMath::playerHasBullet(const string& player) {
    return casualPlayers[player].bullets > 0;
}

bool CasualMath::turnIsFinished() {
    for (auto& player : casualPlayers) {
        if (!playerHasMoved(player.first)) {
            return false;
        }
    }
    return true;
}

string CasualMath::getRoundWinner() {
    const string& p1 = playersUsername[0];
    const string& p2 = playersUsername[1];

    if (casualPlayers.find(p1) == casualPlayers.end() || casualPlayers.find(p2) == casualPlayers.end()) {
        throw ServiceException(ErrorType::PermissionDenied, "Players not found");
    }

    auto& cp1 = casualPlayers[p1];
    auto& cp2 = casualPlayers[p2];

    const string& m1 = cp1.moves.back();
    const string& m2 = cp2.moves.back();

    //if (m1 == "shoot" && cp1.bullets <= 0) {
    //    throw runtime_error(p1 + " tried to shoot without bullets.");
    //}
    //if (m2 == "shoot" && cp2.bullets <= 0) {
    //    throw runtime_error(p2 + " tried to shoot without bullets.");
    //}

    if (m1 == m2) {
        return "draw";
    }

    if (m1 == "shoot" && m2 == "reload") return p1;
    if (m2 == "shoot" && m1 == "reload") return p2;

    if ((m1 == "shoot" && m2 == "defend") || (m2 == "shoot" && m1 == "defend")) {
        return "draw";
    }

    if ((m1 == "reload" && m2 == "defend") || (m2 == "reload" && m1 == "defend")) {
        return "draw";
    }

    return "draw";
}

void CasualMath::deductBullet(const string& player) {
    casualPlayers[player].bullets--;
}

matchType CasualMath::getType() {
    return type;
}

void CasualMath::goNextRound() {
    if (!turnIsFinished())
        throw ServiceException(ErrorType::BadRequest, "Turn is not finished");
    turn++;
    for (auto& player : casualPlayers) {
        player.second.moves.push_back("");
    }
}

void CasualMath::closeMatch() {
    winner = getRoundWinner();
    status = matchSatus::finished;
}

string CasualMath::getStatus(const string& playerName) {
    ostringstream result;
    result << "Turn " << turn << endl;

    string opponentName = "";
    for (const auto& name : playersUsername) {
        if (name != playerName) {
            opponentName = name;
            break;
        }
    }

    auto& me = casualPlayers[playerName];
    auto& opponent = casualPlayers[opponentName];

    string myCurrentStatus = playerHasMoved(playerName) ? me.moves.back() : "pending";
    string oppCurrentStatus = playerHasMoved(opponentName) ? "played" : "pending";
    result << "You: " << myCurrentStatus << endl << "Your opponent: " << oppCurrentStatus << endl << "History: " << endl;
    result << "Opponent’s moves:   Your moves:" << endl;

    if (turn < 2) {
        result << "Empty" << endl;
    }

    for (int turnIndex = 1; turnIndex < turn; turnIndex++) {
        string oppMove = casualPlayers[opponentName].moves[turnIndex];
        string myMove = casualPlayers[playerName].moves[turnIndex];
        result << oppMove;
        for (int i = 0; i < 20 - myMove.length(); i++)
            result << " ";
        result << myMove << endl;
    }
    result << "Your remaining bullets: " << me.bullets;
    return result.str();
}