#include "MatchService.h"
#include "../../models/Match.h"

void MatchService::shoot(const string& playerName) {
    if (!match->playerHasBullet(playerName))
        throw ServiceException(ErrorType::BadRequest, "No bullet to shoot");

    match->updatePlayerMoves(playerName, "shoot");
}

void MatchService::defend(const string& playerName) {
    match->updatePlayerMoves(playerName, "defend");
}

void MatchService::reload(const string& playerName) {
    match->updatePlayerMoves(playerName, "reload");
}

void MatchService::handleAction(shared_ptr<Match> match, const string& action, const string& playerName) {
    this->match = match;

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

void MatchService::checkWin() {
    if (match->turnIsFinished()) {
        match->applyRoundChange();

        const string roundWinner = match->getRoundWinner();
        if (roundWinner == "draw") {
            match->goNextRound();
        }
        else {
            string loserName;
            for (auto& playersInMatch : match->getPlayersList()) {
                if (playersInMatch != roundWinner) loserName = playersInMatch;
                repo.getUserByName(playersInMatch)->leaveMatch();
            }

            if (match->getType() == casual) {
                updateXp(roundWinner, loserName);
            }
            else if (match->getType() == ranked) {
                updateRp(roundWinner, loserName);
            }

            match->closeMatch();
        }
    }
}

void MatchService::updateXp(const string& winnerName, const string& loserName) {
    auto winner = repo.getUserByName(winnerName);
    auto loser = repo.getUserByName(loserName);

    const int deltaXp = max(CASUAL_WIN_INF, (int)(CASUAL_WIN_INTERCEPT - (CASUAL_WIN_RATE * (winner->getXP() - loser->getXP()))));
    winner->applyMatchResult(casual, win, deltaXp);
    loser->applyMatchResult(casual, lose, deltaXp);
}

void MatchService::updateRp(const string &winnerName, const string &loserName) {
    auto winner = repo.getUserByName(winnerName);
    auto loser = repo.getUserByName(loserName);

    const int healthBonus = match->getWinnerHealthBonus();
    int deltaRp = 0;

    switch (winner->getRanked()) {
        case Bronze: {
            deltaRp = BRONZE_DELTA_RP;
            break;
        }
        case Silver: {
            deltaRp = SILVER_DELTA_RP;
            break;
        }
        case Gold: {
            deltaRp = GOLD_DELTA_RP;
            break;
        }
        case Plat: {
            deltaRp = PLAT_DELTA_RP;
            break;
        }
    }

    winner->applyMatchResult(ranked, win, deltaRp + healthBonus);
    loser->applyMatchResult(ranked, lose, deltaRp);
}
