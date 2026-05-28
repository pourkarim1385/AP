#include "CasualMatchService.h"
#include "../../models/ServiceException.h"

void CasualMatchService::shoot(const string& playerName) {
    if (!match->playerHasBullet(playerName))
        throw ServiceException(ErrorType::BadRequest, "No bullet to shoot");

    match->updatePlayerMoves(playerName, "shoot");
    match->deductBullet(playerName);
}

void CasualMatchService::defend(const string& playerName) {
    match->updatePlayerMoves(playerName, "defend");
}

void CasualMatchService::reload(const string& playerName) {
    match->updatePlayerMoves(playerName, "reload");
    match->addBullet(playerName);
}

void CasualMatchService::updateXp(const string& winnerName, const string& loserName) {
    shared_ptr<Player> winner = dynamic_pointer_cast<Player>(repo.getUserByName(winnerName));
    shared_ptr<Player> loser = dynamic_pointer_cast<Player>(repo.getUserByName(loserName));

    int deltaXp = max(5, (int)(50 - (0.1 * (winner->getXP() - loser->getXP()))));
    winner->addXP(deltaXp);
    winner->addWin();
    loser->deductXP(deltaXp);
    loser->addLose();
}

void CasualMatchService::checkWin() {
    if (match->turnIsFinished()) {
        string roundWinner = match->getRoundWinner();
        if (roundWinner == "draw") {
            match->goNextRound();
        }
        else {
            string loserName;
            match->closeMatch();
            for (auto& playersInMatch : match->getPlayersList()) {
                if (playersInMatch != roundWinner) loserName = playersInMatch;
                repo.getUserByName(playersInMatch)->leaveMatch();
            }
            updateXp(roundWinner, loserName);
        }
    }
}

void CasualMatchService::handleAction(shared_ptr<CasualMath> match, const string& action, const string& playerName) {
    this->match = match;//check again

    if (action == "shoot") {
        shoot(playerName);
    }
    else if (action == "reload") {
        reload(playerName);
    }
    else if (action == "defend") {
        defend(playerName);
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "Action not recognized");
    }

    checkWin();
}