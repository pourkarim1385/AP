#include "RunProgram.h"

#include <iostream>
#include <sstream>

#include "../models/ServiceException.h"

const string OK_MESSAGE = "OK\n";
namespace BASE_CMD {
    const string DELETE_CMD = "DELETE";
    const string PUT_CMD = "PUT";
    const string POST_CMD = "POST";
    const string GET_CMD = "GET";
    const string commandsList[] = {DELETE_CMD, PUT_CMD, POST_CMD, GET_CMD};
}

namespace TARGET_CMD {
    const string REGISTER = "register";
    const string LOGIN = "login";
    const string LOGOUT = "logout";
    const string CASUAL_MATCH_READY = "casual_match_ready";
    const string INVITATION = "invitation";
    const string START_MATCH = "start_match";
    const string REJECT_INVITATION = "reject_invitation";
    const string ACTION = "action";
    const string REPORT = "report";
    const string CASUAL_MATCH_OPPONENTS = "casual_match_opponents";
    const string RANKED_MATCH_OPPONENTS = "ranked_match_opponents";
    const string MATCH_STATUS = "match_status";
    const string PROFILE = "profile";
    const string RECEIVED_INVITATIONS = "received_invitations";
    const string REPORTS = "reports";
    const string BLOCK = "block";
    const string DISMISS_REPORT = "dismiss_report";
    const string PENALTY = "penalty";
    const string targetCommandsList[] = {REGISTER, LOGIN, LOGOUT, CASUAL_MATCH_READY, INVITATION,
         START_MATCH, REJECT_INVITATION, ACTION, REPORT, CASUAL_MATCH_OPPONENTS, MATCH_STATUS,
         PROFILE, RECEIVED_INVITATIONS, REPORTS, BLOCK, DISMISS_REPORT, PENALTY, RANKED_MATCH_OPPONENTS};
}

namespace ARGS {
    const string USERNAME = "username";
    const string PASSWORD = "password";
    const string STATUS = "status";
    const string MATCH_TYPE = "match_type";
    const string INVITATION_ID = "invitation_id";
    const string ACTION = "action";
    const string REASON = "reason";
    const string SORT_ORDER = "sort_order";
    const string BLOCK_STATUS = "status";
    const string REPORT_ID = "report_id";
    const string REPORT_TYPE = "type";
    const string AMOUNT = "amount";
    const string MATCH_COUNT = "number_of_matches";
    const SortType defaultSortType = descending;
}

namespace ARG_VAL {
    const string TRUE_VAL = "true";
    const string FALSE_VAL = "false";
    const string ASC = "asc";
    const string DESC = "desc";
    const string CASUAL_MATCH = "casual";
    const string RANKED_MATCH = "ranked";
    const string BLOCKED = "blocked";
    const string UNBLOCKED = "unblocked";
    const string BULLET_PENALTY = "bullet_penalty";
    const string HEALTH_PENALTY = "health_penalty";
}

RunProgram::RunProgram(Repository& repo, LoginService& loginService, MatchMakingService& matchMakingService,
    InviteService& invitationService, MatchPlayService& matchPlayService, ReportService& reportService):
            repo(repo), loginService(loginService), matchMakingService(matchMakingService),
            invitationService(invitationService), matchPlayService(matchPlayService), reportService(reportService) {
}

void RunProgram::validatePremission(PrivilegeLevel neededAccess){
    if (!loggedInUser || loggedInUser->getPrivilegeLevel() != neededAccess) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }
}

bool RunProgram::hasValidBaseCommand(const string& baseCommand) {
    for (auto& preferredCommand : BASE_CMD::commandsList) {
        if (baseCommand == preferredCommand) {
            return true;
        }
    }
    return false;
}

bool RunProgram::hasValidTargetCommand(const string& targetCommand){
    for (auto& preferredCommand : TARGET_CMD::targetCommandsList) {
        if (targetCommand == preferredCommand) {
            return true;
        }
    }
    return false;
}

void RunProgram::validateCommand(const Command& command) {
    if (!hasValidBaseCommand(command.baseCommand)) {
        throw ServiceException(ErrorType::BadRequest, "Invalid Base command.");
    }
    if(!hasValidTargetCommand(command.targetCommand)){
        throw ServiceException(ErrorType::NotFound, "Invalid command.");
    }
}

bool RunProgram::hasValidArguments(const Command& command, const vector<string>& targetArguments) const {
    for (auto& arg : targetArguments) {
        if (command.args.count(arg) == 0) {
            return false;
        }
    }
    for(auto& [argTitle, argContent] : command.args){
        if(argContent == INVALID_STR){
            return false;
        }
    }
    return true;
}

void RunProgram::handleRegister(const Command& command) {
    if (!hasValidArguments(command, {ARGS::USERNAME, ARGS::PASSWORD})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");
    }
    const string username = command.args.at(ARGS::USERNAME);
    const string password = command.args.at(ARGS::PASSWORD);
    if(username.empty() || password.empty())
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");

    if (loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }

    loginService.registerUser(username, password);
    loggedInUser = repo.getUserByName(username);
    print(OK_MESSAGE);
}

void RunProgram::handleLogin(const Command& command) {
    if (!hasValidArguments(command, {ARGS::USERNAME, ARGS::PASSWORD})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");
    }
    const string username = command.args.at(ARGS::USERNAME);
    const string password = command.args.at(ARGS::PASSWORD);
    if(username.empty() || password.empty())
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");

    if (loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }

    if (loginService.loginUser(username, password)) {
        loggedInUser = repo.getUserByName(username);
    }
    print(OK_MESSAGE);
}

void RunProgram::handleLogout(const Command& command) {
    if (!loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "No user is loggedIn");
    }
    loggedInUser = nullptr;
    print(OK_MESSAGE);
}

void RunProgram::handleCasualMatchReady(const Command& command) {
    if (!hasValidArguments(command, {ARGS::STATUS})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }
    validatePremission(normal);
    
    playerStatus readyStatus;
    if(command.args.at(ARGS::STATUS) == ARG_VAL::TRUE_VAL){
        readyStatus = ready;
    }
    else if (command.args.at(ARGS::STATUS) == ARG_VAL::FALSE_VAL){
        readyStatus = unready;
    }
    else{
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    MatchType type = casual;
    matchMakingService.updatePlayerStatus(loggedInUser->getName(), type, readyStatus);
    print(OK_MESSAGE);
}

void RunProgram::handleInvitation(const Command& command) {
    if (!hasValidArguments(command, {ARGS::USERNAME, ARGS::MATCH_TYPE})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }
    validatePremission(normal);

    string targetUsername = command.args.at(ARGS::USERNAME);
    MatchType targetMatchType;
    if (command.args.at(ARGS::MATCH_TYPE) == ARG_VAL::CASUAL_MATCH) {
        targetMatchType = casual;
    }
    else if (command.args.at(ARGS::MATCH_TYPE) == ARG_VAL::RANKED_MATCH) {
        targetMatchType = ranked;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    invitationService.newInvite(loggedInUser->getName(), targetUsername, targetMatchType);
    print(OK_MESSAGE);
}

void RunProgram::handleStartMatch(const Command& command) {
    if (!hasValidArguments(command, {ARGS::INVITATION_ID})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }
    int invitationId = 0;
    try{
        invitationId = stoi(command.args.at(ARGS::INVITATION_ID));
    }
    catch(...){
        throw ServiceException(ErrorType::BadRequest, "Id must be number");
    }
    
    validatePremission(normal);

    try {
        invitationService.accpeptInvite(loggedInUser->getName(),invitationId, loggedInUser->getPrivilegeLevel());
        print(OK_MESSAGE);
    }
    catch(const ServiceException& e) {
        if (e.getType() == ErrorType::MisMatch) {
            print(e.getDetail() + "\n");
            return;
        }
        else {
            throw e;
        }
    }
}

void RunProgram::handleRejectInvitation(const Command& command) {
    if (!hasValidArguments(command, {ARGS::INVITATION_ID})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }
    int invitationId = 0;
    try{
        invitationId = stoi(command.args.at(ARGS::INVITATION_ID));
    }
    catch(...){
        throw ServiceException(ErrorType::BadRequest, "Id must be number");
    }
    
    validatePremission(normal);

    invitationService.rejectInvite(loggedInUser->getName(),invitationId, loggedInUser->getPrivilegeLevel());
    print(OK_MESSAGE);
}

void RunProgram::handleAction(const Command& command) {
    if (!hasValidArguments(command, {ARGS::ACTION})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");
    }
    
    validatePremission(normal);

    string action = command.args.at(ARGS::ACTION);
    int matchId = loggedInUser->getOnGoingMatch();
    matchPlayService.executePlayerAction(matchId, loggedInUser->getName(), action);
    print(OK_MESSAGE);
}

void RunProgram::handleReport(const Command& command) {
    if (!hasValidArguments(command, {ARGS::USERNAME, ARGS::REASON})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");
    }
    
    validatePremission(normal);

    string reportedUsername = command.args.at(ARGS::USERNAME);
    string reportedReason = command.args.at(ARGS::REASON);
    string senderUsername = loggedInUser->getName();
    reportService.submitReport(senderUsername, reportedUsername, reportedReason);
    print(OK_MESSAGE);
}

void RunProgram::handleBlock(const Command& command) {
    if (!hasValidArguments(command, {ARGS::USERNAME, ARGS::BLOCK_STATUS})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arg.");
    }

    validatePremission(normal);
    string targetUsername = command.args.at(ARGS::USERNAME);
    bool blockStatus;

    if (command.args.at(ARGS::BLOCK_STATUS) == ARG_VAL::BLOCKED) {
        blockStatus = true;
    }
    else if (command.args.at(ARGS::BLOCK_STATUS) == ARG_VAL::UNBLOCKED) {
        blockStatus = false;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    if (!repo.getUserByName(targetUsername)) {
        throw ServiceException(ErrorType::BadRequest, "User not found.");
    }

    if (blockStatus == true) {
        loggedInUser->addBlocked(targetUsername);
    }
    else {
        loggedInUser->unblock(targetUsername);
    }

    print(OK_MESSAGE);
}

void RunProgram::handlePostCommand(const Command& command) {
    if (command.targetCommand == TARGET_CMD::REGISTER) return handleRegister(command);
    if (command.targetCommand == TARGET_CMD::LOGIN) return handleLogin(command);
    if (command.targetCommand == TARGET_CMD::LOGOUT) return handleLogout(command);
    if (command.targetCommand == TARGET_CMD::CASUAL_MATCH_READY) return handleCasualMatchReady(command);
    if (command.targetCommand == TARGET_CMD::INVITATION) return handleInvitation(command);
    if (command.targetCommand == TARGET_CMD::START_MATCH) return handleStartMatch(command);
    if (command.targetCommand == TARGET_CMD::REJECT_INVITATION) return handleRejectInvitation(command);
    if (command.targetCommand == TARGET_CMD::ACTION) return handleAction(command);
    if (command.targetCommand == TARGET_CMD::REPORT) return handleReport(command);
    if (command.targetCommand == TARGET_CMD::BLOCK) return handleBlock(command);
    if (command.targetCommand == TARGET_CMD::DISMISS_REPORT) return handleDismissReports(command);
    if (command.targetCommand == TARGET_CMD::PENALTY) return handlePenlatyAdd(command);

    throw ServiceException(ErrorType::NotFound, "Invalid target command");
}

void RunProgram::handleMatchOpponents(const Command& command, const MatchType matchType) {
    validatePremission(normal);


    SortType sType = ARGS::defaultSortType;

    if (hasValidArguments(command, {ARGS::SORT_ORDER})) {
        if (command.args.at(ARGS::SORT_ORDER) == ARG_VAL::ASC) {
            sType = ascending;
        } else if (command.args.at(ARGS::SORT_ORDER) == ARG_VAL::DESC) {
            sType = descending;
        }
        else{
            throw ServiceException(ErrorType::BadRequest, "Sort type is invalid!");
        } 
    }

    const string output = matchMakingService.getOpponentList(loggedInUser->getName(), matchType, sType);
    print(output);
}

void RunProgram::handleMatchStatus(const Command& command) {
    validatePremission(normal);

    const string output = matchPlayService.getMatchStatus(loggedInUser->getOnGoingMatch(), loggedInUser->getName(), loggedInUser->getPrivilegeLevel());
    print(output);
}

void RunProgram::handleProfile(const Command& command) {
    string username;
    if (hasValidArguments(command, {ARGS::USERNAME})) {
        username = command.args.at(ARGS::USERNAME);
    }
    
    if (!loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }
    
    if (username.empty()) {
        username = loggedInUser->getName();
    }
    
    const string output = reportService.getUserProfile(username);
    print(output);
}

void RunProgram::handleReceivedInvitations(const Command& command) {
    validatePremission(normal);

    const string output = invitationService.getPendingInvites(loggedInUser->getName());
    print(output);
}

void RunProgram::handleReports(const Command& command) {
    validatePremission(adminAccess);

    const string output = reportService.getReports(loggedInUser->getPrivilegeLevel());
    print(output);
}

void RunProgram::handleDismissReports(const Command& command) {
    int reportId = 0;
    if (!hasValidArguments(command, {ARGS::REPORT_ID})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    try {
        reportId = stoi(command.args.at(ARGS::REPORT_ID));
    }
    catch (...) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    validatePremission(adminAccess);

    reportService.dismissReport(reportId, loggedInUser->getPrivilegeLevel());
    print(OK_MESSAGE);
}

void RunProgram::handlePenlatyAdd(const Command &command) {
    int reportId = 0, amount = 0, matchCount = 0;
    PenaltyType type;
    if (!hasValidArguments(command, {ARGS::REPORT_ID, ARGS::REPORT_TYPE, ARGS::AMOUNT, ARGS::MATCH_COUNT})) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command arguments.");
    }

    try {
        reportId = stoi(command.args.at(ARGS::REPORT_ID));
        amount = stoi(command.args.at(ARGS::AMOUNT));
        matchCount = stoi(command.args.at(ARGS::MATCH_COUNT));
    }
    catch (...) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command. not a number");
    }

    if (command.args.at(ARGS::REPORT_TYPE) == ARG_VAL::BULLET_PENALTY) {
        type = PenaltyType::Bullet;
    }
    else if (command.args.at(ARGS::REPORT_TYPE) == ARG_VAL::HEALTH_PENALTY) {
       type = PenaltyType::Health;
    }
    else {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }

    validatePremission(adminAccess);

    reportService.addPenalty(reportId, type, amount, matchCount, loggedInUser->getPrivilegeLevel());
    print(OK_MESSAGE);
}


void RunProgram::handleGetCommand(const Command& command) {
    if (command.targetCommand == TARGET_CMD::CASUAL_MATCH_OPPONENTS) return handleMatchOpponents(command, casual);
    if (command.targetCommand == TARGET_CMD::RANKED_MATCH_OPPONENTS) return handleMatchOpponents(command, ranked);
    if (command.targetCommand == TARGET_CMD::MATCH_STATUS) return handleMatchStatus(command);
    if (command.targetCommand == TARGET_CMD::PROFILE) return handleProfile(command);
    if (command.targetCommand == TARGET_CMD::RECEIVED_INVITATIONS) return handleReceivedInvitations(command);
    if (command.targetCommand == TARGET_CMD::REPORTS) return handleReports(command);

    throw ServiceException(ErrorType::NotFound, "Invalid target command");
}

void RunProgram::handleDeleteCommand(const Command& command) {
    //
    throw ServiceException(ErrorType::NotFound, "Invalid target command");
}

void RunProgram::handlePutCommand(const Command& command) {
    //
    throw ServiceException(ErrorType::NotFound, "Invalid target command");
}

void RunProgram::dispatchCommand(const Command& command) {
    if (command.baseCommand == BASE_CMD::DELETE_CMD) {
        handleDeleteCommand(command);
        return;
    }
    if (command.baseCommand == BASE_CMD::PUT_CMD) {
        handlePutCommand(command);
        return;
    }
    if (command.baseCommand == BASE_CMD::POST_CMD) {
        handlePostCommand(command);
        return;
    }
    if (command.baseCommand == BASE_CMD::GET_CMD) {
        handleGetCommand(command);
        return;
    }
    throw ServiceException(ErrorType::BadRequest, "Invalid Base command");
}

void RunProgram::run() {
    string inputCommand;
    while (getline(cin, inputCommand)) {
        try {
            Command command = commandParser.processCommand(inputCommand);
            validateCommand(command);
            dispatchCommand(command);
        }
        catch (const ServiceException& error) {
            print(error.what());
            //debug:
            print(error.getDetail());
            print("\n");
        }
    }
}

void RunProgram::print(const string& message) {
    cout << message;
}