#include "Repository.h"
#include "../models/ServiceException.h"
#include "../models/Invitation.h"
#include "../models/Match.h"

shared_ptr<User> Repository::getUserByName(const string &name) {
    auto it = users.find(name);
    if (it == users.end()) {
        return nullptr;
    }
    return it->second;
}

shared_ptr<const User> Repository::getUserByName(const string &name) const {
    auto it = users.find(name);
    if (it == users.end()) {
        return nullptr;
    }
    return it->second;
}

shared_ptr<Invitation> Repository::getInvitationByID(int id) {
    auto it = invitations.find(id);
    if (it == invitations.end()) {
        return nullptr;
    }
    return it->second;
}

shared_ptr<const Invitation> Repository::getInvitationByID(int id) const {
    auto it = invitations.find(id);
    if (it == invitations.end()) {
        return nullptr;
    }
    return it->second;
}

void Repository::addPlayer(const string &name, const string &password) {
    users[name] = make_shared<Player>(name, password);
}

void Repository::addPlayer(const string &name, const string &password, int xp) {
    users[name] = make_shared<Player>(name, password, xp);
}

void Repository::addUser(unique_ptr<User> user) {
    if (user) {
        string name = user->getName();
        users[name] = move(user);
    }
}

void Repository::addInvitation(const int id, const string& from, const string& to, MatchType type) {
    invitations[id] = make_shared<Invitation>(id, from, to, type);
}

void Repository::removeInvitation(const int id) {
    auto targetInvite = getInvitationByID(id);
    if (!targetInvite) {
        throw ServiceException(ErrorType::NotFound, "Invitation not found");
    }

    auto sender = getUserByName(targetInvite->getSender());
    if (sender) sender->removeSentInvite(id);

    auto receiver = getUserByName(targetInvite->getReceiver());
    if (receiver) receiver->removePendingInvite(id);

    invitations.erase(id);
}

void Repository::removeReport(const int id) {
    auto targetReport = getReportByID(id);
    if (!targetReport) {
        throw ServiceException(ErrorType::NotFound, "Invitation not found");
    }

    reports.erase(id);
}

void Repository::importPlayers(vector<unique_ptr<Player>> players) {
    for (auto& player : players) {
        addUser(move(player));
    }
}

void Repository::importAdmins(vector<unique_ptr<Admin>> admins) {
    for (auto& admin : admins) {
        addUser(move(admin));
    }
}

vector<shared_ptr<const Player>> Repository::getPlayersList() const {
    vector<shared_ptr<const Player>> result;

    for (const auto& [username, user] : users) {
        if (user->getPrivilegeLevel() == normal) {
            auto tempPlayer = static_pointer_cast<const Player>(user);
            result.push_back(tempPlayer);
        }
    }

    return result;
}

shared_ptr<Match> Repository::getMatchByID(int id) {
    auto it = matches.find(id);
    if (it == matches.end()) {
        return nullptr;
    }
    return it->second;
}

shared_ptr<const Match> Repository::getMatchByID(int id) const {
    auto it = matches.find(id);
    if (it == matches.end()) {
        return nullptr;
    }
    return it->second;
}

void Repository::addMatch(shared_ptr<Match> match) {
    matches[match->getId()] = move(match);
}

void Repository::addReport(int id, const string &senderUsername, const string &reportedUsername, const string &reason) {
    reports[id] = make_shared<Report>(id, reportedUsername, senderUsername, reason);
}

shared_ptr<Report> Repository::getReportByID(int id) {
    auto it = reports.find(id);
    if (it == reports.end()) {
        return nullptr;
    }
    return it->second;
}

shared_ptr<const Report> Repository::getReportByID(int id) const {
    auto it = reports.find(id);
    if (it == reports.end()) {
        return nullptr;
    }
    return it->second;
}

const map<int, shared_ptr<Report>>& Repository::getReportsList() const {
    return reports;
}