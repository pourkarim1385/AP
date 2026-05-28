#include "Repository.h"

#include "../models/ServiceException.h"


shared_ptr<User> Repository::getUserByName(const string &name) {
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

void Repository::addPlayer(const string &name, const string &password) {
    shared_ptr<User> user = make_shared<Player>(name, password);
    users[name] = user;
}
void Repository::addPlayer(const string &name, const string &password, int xp) {
    shared_ptr<User> user = make_shared<Player>(name, password, xp);
    users[name] = user;
}

void Repository::addUser(unique_ptr<User> user) {
    users[user->getName()] = move(user);
}

void Repository::addInvitation(const int id, const string& from, const string& to, matchType type) {
    invitations[id] = make_shared<Invitation>(id, from, to, type);
    users[from]->addSentInvite(id);
    users[to]->addPendingInvite(id);
}

void Repository::removeInvitation(const int id) {
    shared_ptr<Invitation> targetInvite = getInvitationByID(id);
    if (!targetInvite) {
        throw ServiceException(ErrorType::NotFound, "Invitation not found");
    }
    users[targetInvite->getSender()]->removeSentInvite(id);
    users[targetInvite->getReceiver()]->removePendingInvite(id);
    invitations.erase(id);
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

vector<shared_ptr<Player>> Repository::getPlayersList() const {
    vector<shared_ptr<Player>> result;
    for (auto& [username, player] : users) {
        shared_ptr<Player> tempPlayer = dynamic_pointer_cast<Player>(player);
        if (tempPlayer) {
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

void Repository::addMatch(int id, vector<string>& players) {
    matches[id] = make_shared<CasualMath>(id, players);
}

void Repository::addReport(int id, const string &senderUsername, const string &reportedUsername, const string &reason) {
    reports[id] = make_shared<Report>(id, senderUsername, reportedUsername, reason);
}

shared_ptr<Report> Repository::getReportByID(int id) {
    auto it = reports.find(id);
    if (it == reports.end()) {
        return nullptr;
    }
    return it->second;
}

const map<int, shared_ptr<Report> > &Repository::getReportsList() const {
    return reports;
}



