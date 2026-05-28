#ifndef TRY1_REPOSITORY_H
#define TRY1_REPOSITORY_H
#include <map>
#include <memory>
#include "../models/User.h"
#include "../models/Invitation.h"
#include "../models/Report.h"
#include "../models/Match.h"


using namespace std;

class Repository {
    private:
    map<string, shared_ptr<User>> users;
    map<int, shared_ptr<Invitation>> invitations;
    map<int, shared_ptr<Report>> reports;
    map<int, shared_ptr<Match>> matches;

    void addUser(const unique_ptr<User> user);

    bool hasValidAccess();
    public:
    Repository() = default;
    void importUser();
    shared_ptr<User> getUserByName(const string& name);
    shared_ptr<Invitation> getInvitationByID(int id);
    shared_ptr<Report> getReportByID(int id);
    shared_ptr<Match> getMatchByID(int id);
    void addPlayer(const string& name, const string& password);
    void addPlayer(const string& name, const string& password, int xp);

    void addInvitation(int id, const string& from, const string& to, matchType type);
    void removeInvitation(int id);

    void importPlayers(vector<unique_ptr<Player>> players);
    void importAdmins(vector<unique_ptr<Admin>> invitations);
    vector<shared_ptr<Player>> getPlayersList() const;
    const map<int, shared_ptr<Report>>& getReportsList() const;

    void addMatch(int id, vector<string>& players);
    void addReport(int id, const string& senderUsername, const string& reportedUsername, const string& reason);
};


#endif //TRY1_REPOSITORY_H