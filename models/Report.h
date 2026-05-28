#ifndef TRY1_REPORT_H
#define TRY1_REPORT_H
#include <string>

using namespace std;

class Report {
    private:
    int id;
    string reportedUsername;
    string senderUsername;
    string reason;
    public:
    Report(int id, string reportedUsername, string senderUsername, string reason)
            : id(id), reportedUsername(reportedUsername){}
    string getReportedUsername() const {return reportedUsername;}
    string getSenderUsername() const {return senderUsername;}
    string getReason() const {return reason;}
};


#endif //TRY1_REPORT_H