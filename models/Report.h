#pragma once

#include <string>

using namespace std;

class Report {
    private:
    int id;
    string reportedUsername;
    string senderUsername;
    string reason;
    public:
    Report(int id, const string& reportedUsername, const string& senderUsername, const string& reason)
        : id(id), reportedUsername(reportedUsername), senderUsername(senderUsername), reason(reason) {};
    string getReportedUsername() const {return reportedUsername;}
    string getSenderUsername() const {return senderUsername;}
    string getReason() const {return reason;}
};