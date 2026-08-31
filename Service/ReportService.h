#pragma once

#include "../DAL/Repository.h"

class ReportService {
private:
    Repository& repo;
    int lastId;
    int generateNextId();
    void validatePenalty(PenaltyType type, int amount, int matchCount);

public:
    ReportService(Repository& repo);
    void submitReport(const string& senderUsername, const string& reportedUsername, const string& reason);
    void dismissReport(int id, PrivilegeLevel privilegeLevel);
    void addPenalty(int reportId, PenaltyType type, int amount, int matchCount, PrivilegeLevel privilegeLevel);
    string getReports(PrivilegeLevel access);
    string getUserProfile(const string& username);
};