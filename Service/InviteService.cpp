#include "InviteService.h"
#include <sstream>
#include "../models/ServiceException.h"

int InviteService::generateNextId() {
    return ++lastId;
}

void InviteService::accpeptInvite(int id) {
    shared_ptr<Invitation> invitation = repo.getInvitationByID(id);
    if (!invitation)
        throw ServiceException(ErrorType::NotFound, "Invitation not found.");

    shared_ptr<Player> sender= dynamic_pointer_cast<Player>(repo.getUserByName(invitation->getSender()));
    shared_ptr<Player> reciever = dynamic_pointer_cast<Player>(repo.getUserByName(invitation->getReceiver()));
    if (!sender || !reciever || sender->isInMatch() || reciever->isInMatch()) {
        throw ServiceException(ErrorType::PermissionDenied, "Player not found or Match cannot be started");
    }
    vector<string> players = {invitation->getSender(), invitation->getReceiver()};
    matchMakingService.startMatch(players, invitation->getType());
    repo.removeInvitation(id);
}

void InviteService::rejectInvite(int id) {
    repo.removeInvitation(id);
}

void InviteService::addInvite(const string& from, const string& to, matchType type) {
    int id = generateNextId();
    repo.addInvitation(id, from, to, type);
}

void InviteService::newInvite(const string &from, const string &to, const string& type) {
    matchType targetType;
    if (type == "casual") {
        targetType = matchType::casual;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "MatchType not found");
    }

    if (!repo.getUserByName(to)) {
        throw ServiceException(ErrorType::NotFound, "User not found");
    }
    if (repo.getUserByName(to)->getPriverLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Cannot invite admin");
    }

    addInvite(from, to, targetType);
    //matchmaking service
}

string InviteService::matchTypeToString(const matchType type) const{
    switch (type) {
        case matchType::casual:   return "casual";
        default:           return "Unknown";
    }
}

string InviteService::getPendingInvites(const string& username) const {
    shared_ptr<Player> player = dynamic_pointer_cast<Player>(repo.getUserByName(username));
    if (!player) {
        throw ServiceException(ErrorType::PermissionDenied, "User not found");
    }


    const vector<int>& pendingInvitesList = player->getPendingInvites();
    if (pendingInvitesList.empty()) {
        return "Empty";
    }
    ostringstream result;
    for (auto& invitationId : pendingInvitesList) {
        shared_ptr<Invitation> invitation = repo.getInvitationByID(invitationId);
        result << invitationId << ": Invitation from " << '"' << invitation->getSender() << '"' << " for a "
        << '"' << matchTypeToString(invitation->getType()) << '"' << " match" << endl;
    }
    return result.str();
}

InviteService::InviteService(Repository &repo, MatchMakingService &matchMakingService) : repo(repo), matchMakingService(matchMakingService) {
    lastId = 0;
}