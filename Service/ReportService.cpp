#include "ReportService.h"

#include "../models/ServiceException.h"
#include <sstream>


ReportService::ReportService(Repository &repo) : repo(repo) {
    lastId = 1;
}

int ReportService::generateNextId() {
    return ++lastId;
}

string ReportService::getReports(PrivilegeLevel access) {
    if (access != adminAccess)
        throw ServiceException(ErrorType::PermissionDenied, "Access Denied");
    ostringstream result;
    const map<int, shared_ptr<Report>>& reportsList = repo.getReportsList();
    if (reportsList.empty()) {
        result << "Empty" << endl;
        return result.str();
    }

    for (auto& [id, report] : reportsList) {
        result << id << ": " << '"' << report->getReportedUsername() << '"' <<
            " reported " << '"' << report->getSenderUsername() << '"' << " for: " << '"' <<
                report->getReason() << '"' << endl;
    }
    return result.str();
}

void ReportService::submitReport(const string &senderUsername, const string &reportedUsername, const string &reason) {
    if (!repo.getUserByName(reportedUsername)) {
        throw ServiceException(ErrorType::NotFound, "User not found.");
    }
    if (reason.empty()) {
        throw ServiceException(ErrorType::BadRequest, "No reason provided.");
    }
    repo.addReport(generateNextId(), senderUsername, reportedUsername, reason);
}

string ReportService::getUserProfile(const string &username) {
    const shared_ptr<User> targetUser = repo.getUserByName(username);
    if (!targetUser) {
        throw ServiceException(ErrorType::NotFound, "User not found.");
    }
    if (targetUser->getPrivilegeLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Access Denied");
    }
    return targetUser->getProfile();
}