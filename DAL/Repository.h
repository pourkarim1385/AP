#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>
#include "../models/User.h"
#include "../models/Report.h"

class Invitation;
class Match;

using namespace std;

class Repository {
private:
    map<string, shared_ptr<User>> users;
    map<int, shared_ptr<Invitation>> invitations;
    map<int, shared_ptr<Report>> reports;
    map<int, shared_ptr<Match>> matches;

    void addUser(unique_ptr<User> user);

public:
    Repository() = default;

    shared_ptr<User> getUserByName(const string& name);
    shared_ptr<const User> getUserByName(const string& name) const;

    shared_ptr<Invitation> getInvitationByID(int id);
    shared_ptr<const Invitation> getInvitationByID(int id) const;

    shared_ptr<Report> getReportByID(int id);
    shared_ptr<const Report> getReportByID(int id) const;

    shared_ptr<Match> getMatchByID(int id);
    shared_ptr<const Match> getMatchByID(int id) const;

    void addPlayer(const string& name, const string& password);
    void addPlayer(const string& name, const string& password, int xp);

    void addInvitation(int id, const string& from, const string& to, MatchType type);
    void removeInvitation(int id);

    void importPlayers(vector<unique_ptr<Player>> players);
    void importAdmins(vector<unique_ptr<Admin>> admins);

    vector<shared_ptr<const Player>> getPlayersList() const;
    const map<int, shared_ptr<Report>>& getReportsList() const;

    void addMatch(shared_ptr<Match> match);

    void addReport(int id, const string& senderUsername, const string& reportedUsername, const string& reason);
    void removeReport(int id);
};