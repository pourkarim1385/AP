#pragma once

#include "../DAL/Repository.h"
#include "../Service/CommandParser.h"
#include "../Service/InviteService.h"
#include "../Service/LoginService.h"
#include "../Service/MatchMakingService.h"
#include "../Service/ReportService.h"
#include "../Service/RepositoryService.h"
#include "../Service/MatchServices/MatchPlayService.h"
#include "../models/Types.h"

class RunProgram {
    private:
    shared_ptr<User> loggedInUser;
    CommandParser commandParser;

    Repository& repo;
    LoginService& loginService;
    MatchMakingService& matchMakingService;
    InviteService& invitationService;
    MatchPlayService& matchPlayService;
    ReportService& reportService;

    void handleDeleteCommand(const Command& command);
    void handlePostCommand(const Command& command);
    void handleGetCommand(const Command& command);
    void handlePutCommand(const Command& command);

    void dispatchCommand(const Command& command);
    void validateCommand(const Command& command);
    void validatePremission(PrivilegeLevel neededAccess);
    bool hasValidArguments(const Command& command,const vector<string>& targetArguments) const;
    bool hasValidBaseCommand(const string& baseCommand);
    bool hasValidTargetCommand(const string& targetCommand);
    void print(const string& message);

    //handler functions
    void handleRegister(const Command& command);
    void handleLogin(const Command& command) ;
    void handleLogout (const Command& command);
    void handleCasualMatchReady (const Command& command);
    void handleInvitation (const Command& command);
    void handleStartMatch(const Command& command);
    void handleRejectInvitation(const Command& command);
    void handleAction(const Command& command);
    void handleReport(const Command& command);
    void handleMatchOpponents (const Command& command, const MatchType matchType);
    void handleMatchStatus (const Command& command);
    void handleProfile(const Command& command);
    void handleReceivedInvitations (const Command& command);
    void handleReports (const Command& command);
    void handleDismissReports (const Command& command);
    void handleBlock(const Command& command);
    void handlePenlatyAdd(const Command& command);

    public:
    explicit RunProgram(Repository& repo,
               LoginService& loginService,
               MatchMakingService& matchMakingService,
               InviteService& invitationService,
               MatchPlayService& matchPlayService,
               ReportService& reportService);
    void run();
};