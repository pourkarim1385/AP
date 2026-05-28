#ifndef TRY1_REPORTSERVICE_H
#define TRY1_REPORTSERVICE_H
#include "../DAL/Repository.h"


class ReportService {
private:
    Repository& repo;
    int lastId;
    int generateNextId();
    public:
    ReportService(Repository& repo);
    void submitReport(const string& senderUsername, const string& reportedUsername, const string& reason);
    string getReports(PrivilegeLevel access);
    string getUserProfile(const string& username);
};


#endif //TRY1_REPORTSERVICE_H