#include "ReportService.h"

#include "../models/ServiceException.h"
#include <sstream>

ReportService::ReportService(Repository &repo) : repo(repo), lastId(0) {}

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
        result << id << ": \"" << report->getSenderUsername() << "\" reported \"" <<
            report->getReportedUsername() << "\" for: " << '"' <<report->getReason() << '"' << endl;
    }
    return result.str();
}

void ReportService::submitReport(const string &senderUsername, const string &reportedUsername, const string &reason) {
    if (!repo.getUserByName(reportedUsername)) {
        throw ServiceException(ErrorType::NotFound, "User not found.");
    }
    if (repo.getUserByName(reportedUsername)->getPrivilegeLevel() == adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Cannot report admin");
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

void ReportService::dismissReport(int id, PrivilegeLevel privilegeLevel) {
    if (privilegeLevel != adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Access Denied");
    }
    repo.removeReport(id);
}

void ReportService::validatePenalty(PenaltyType type, int amount, int matchCount) {
    if (matchCount < 1) {
        throw ServiceException(ErrorType::BadRequest, "Penalty matchCount is out of range");
    }

    switch (type) {
        case PenaltyType::Bullet: {
            if (amount > 3 || amount < 1) {
                throw ServiceException(ErrorType::BadRequest, "Penalty amount is out of range");
            }
            break;
        }
        case PenaltyType::Health: {
            if (amount > 2 || amount < 1) {
                throw ServiceException(ErrorType::BadRequest, "Penalty amount is out of range");
            }
            break;
        }
        default: {
            throw ServiceException(ErrorType::BadRequest, "Penalty type not supported.");
        }
    }
}

void ReportService::addPenalty(const int reportId,const PenaltyType type,const int amount,const int matchCount,const PrivilegeLevel privilegeLevel) {
    const auto report = repo.getReportByID(reportId);
    if (!report) {
        throw ServiceException(ErrorType::NotFound, "Report not found.");
    }
    const auto targetUser = repo.getUserByName(report->getReportedUsername());
    if (!targetUser) {
        throw ServiceException(ErrorType::NotFound, "Report not found.");
    }

    if (privilegeLevel != adminAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Access Denied");
    }

    validatePenalty(type, amount, matchCount);
    targetUser->addPenalty(type, amount, matchCount);
    dismissReport(reportId, privilegeLevel);
}