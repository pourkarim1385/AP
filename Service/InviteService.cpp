#include "InviteService.h"
#include <sstream>
#include "../models/ServiceException.h"
#include "../models/Invitation.h"

int InviteService::generateNextId() {
    return ++lastId;
}

void InviteService::accpeptInvite(const string& username, int id, PrivilegeLevel privilegeLevel) {
    auto invitation = repo.getInvitationByID(id);
    if (!invitation) {
        throw ServiceException(ErrorType::NotFound, "Invitation not found.");
    }
    if(privilegeLevel == adminAccess)
        throw ServiceException(ErrorType::PermissionDenied);

    auto sender = repo.getUserByName(invitation->getSender());
    auto receiver = repo.getUserByName(invitation->getReceiver());

    if (!sender || !receiver || sender->isInMatch() || receiver->isInMatch()) {
        throw ServiceException(ErrorType::PermissionDenied, "Player not found or Match cannot be started");
    }
    if(invitation->getReceiver() != username){
        throw ServiceException(ErrorType::PermissionDenied, "You cannot accpet this invite!");
    }

    if (invitation->getType() == ranked) {
        if (sender->getRanked() != receiver->getRanked()) {
            repo.removeInvitation(id);
            throw ServiceException(ErrorType::MisMatch,"Level Mismatch");
        }
    }

    vector<string> players = {invitation->getSender(), invitation->getReceiver()};
    matchMakingService.startMatch(players, invitation->getType());
    repo.removeInvitation(id);
}

void InviteService::rejectInvite(const string& username, const int id, PrivilegeLevel privilegeLevel) {
    auto invitation = repo.getInvitationByID(id);
    if (!invitation) {
        throw ServiceException(ErrorType::NotFound, "Invitation not found.");
    }
    
    if(invitation->getReceiver() != username || privilegeLevel == adminAccess){
        throw ServiceException(ErrorType::PermissionDenied, "You cannot accpet this invite!");
    }
    repo.removeInvitation(id);
}

void InviteService::addInvite(const string& from, const string& to, MatchType type) {
    const int id = generateNextId();
    repo.addInvitation(id, from, to, type);

    auto sender = repo.getUserByName(from);
    auto receiver = repo.getUserByName(to);

    if (sender) sender->addSentInvite(id);
    if (receiver) receiver->addPendingInvite(id);
}

void InviteService::newInvite(const string &from, const string &to, MatchType type) {
    MatchType targetType;

    auto targetUser = repo.getUserByName(to);
    if (!targetUser) {
        throw ServiceException(ErrorType::NotFound, "User not found");
    }
    if (targetUser->getPrivilegeLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Cannot invite admin");
    }
    if (targetUser->hasBlocked(from)) {
        throw ServiceException(ErrorType::NotFound, "Cannot invite blocked");
    }

    addInvite(from, to, type);
}

string InviteService::getPendingInvites(const string& username) const {
    auto player = repo.getUserByName(username);
    if (!player) {
        throw ServiceException(ErrorType::PermissionDenied, "User not found");
    }

    const auto& pendingInvitesList = player->getPendingInvites();
    if (pendingInvitesList.empty()) {
        return "Empty\n";
    }

    ostringstream result;
    for (size_t i = 0; i < pendingInvitesList.size(); ++i) {
        auto invitation = repo.getInvitationByID(pendingInvitesList[i]);
        if (invitation) {
            result << pendingInvitesList[i] << ": Invitation from \"" << invitation->getSender()
                   << "\" for a \"" << invitation->getType() << "\" match" << endl;
        }
    }
    return result.str();
}

InviteService::InviteService(Repository &repo, MatchMakingService &matchMakingService)
    : repo(repo), matchMakingService(matchMakingService), lastId(0) {}