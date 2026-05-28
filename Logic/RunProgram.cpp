#include "RunProgram.h"

#include <iostream>
#include <sstream>

#include "../models/ServiceException.h"

namespace COMMANDS {
    const string commandsList[] = {"DELETE", "PUT", "POST", "GET"};
}

RunProgram::RunProgram(Repository& repo) :
        repo(repo), loginService(repo), matchMakingService(repo), invitationService(repo,
            matchMakingService), matchPlayService(repo), reportService(repo) {

}

map<string, string> RunProgram::processArguments(const string& inputArgs) {
    map<string, string> arguments;
    if (inputArgs.empty()) {
        return arguments;
    }

    istringstream stream(inputArgs);
    string argTitle, argContent;
    while (stream >> argTitle >> argContent) {
        const string trimmedArgContent = argContent.substr(1, argContent.size() - 2);
        arguments[argTitle] = trimmedArgContent;
    }
    return arguments;
}

Command RunProgram::processCommand(const string& command) {
    Command result;
    istringstream stream(command);
    string cmd, arguments;
    getline(stream, cmd, '?');
    getline(stream, arguments);

    istringstream cmdStream(cmd);
    cmdStream >> result.baseCommand;
    cmdStream >> result.targetCommand;

    result.args = processArguments(arguments);

    return result;
}

bool RunProgram::hasValidBaseCommand(const string& baseCommand) {
    for (auto& preferedCommand : COMMANDS::commandsList) {
        if (baseCommand == preferedCommand) {
            return true;
        }
    }
    return false;
}

void RunProgram::validateCommand(const Command& command) {
    if (!hasValidBaseCommand(command.baseCommand)) {
        throw ServiceException(ErrorType::BadRequest, "Invalid command.");
    }
    if ((command.targetCommand == "register" || command.targetCommand == "login") && loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }
    if ((command.targetCommand != "register" && command.targetCommand != "login") && !loggedInUser) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid command.");
    }
}

bool RunProgram::hasValidArguments(const Command& command,const vector<string>& targetArguments) const{
    for (auto& arg : targetArguments) {
        if (command.args.count(arg) == 0) {
            return false;
        }
    }
    return true;
}

void RunProgram::dispatchCommand(const Command& command) {
    string output = "OK";
    if (command.baseCommand == "DELETE") {

    }
    else if (command.baseCommand == "PUT") {

    }
    else if (command.baseCommand == "POST") {
        if (command.targetCommand == "register") {
            if (!hasValidArguments(command, {"username", "password"})) {
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            }
            const string username = command.args.at("username"),
            password = command.args.at("password");
            loginService.registerUser(username, password);
            print(output);
            return;
        }
        if (command.targetCommand == "login") {
            if (!hasValidArguments(command, {"username", "password"})) {
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            }
            const string username = command.args.at("username"),
            password = command.args.at("password");
            if (loginService.loginUser(username, password)) {
                loggedInUser = repo.getUserByName(username);
            }
            print(output);
            return;
        }
        if (command.targetCommand == "logout") {
            if (loggedInUser) {
                loggedInUser = nullptr;
            }
            else {
                throw ServiceException(ErrorType::PermissionDenied, "No user is loggedIn");
            }
            print(output);
            return;
        }
        if (command.targetCommand == "casual_match_ready") {
            playerStatus readyStatus; matchType type = casual;
            if (!hasValidArguments(command, {"status"})) {
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            }
            if (command.args.at("status") == "true") {
                readyStatus = ready;
            }
            else {
                readyStatus = unready;
            }

            //matchmaking system
            matchMakingService.updatePlayerStatus(loggedInUser->getName(), type, readyStatus);
            print(output);
            return;
        }
        if (command.targetCommand == "invitation") {
            if (!hasValidArguments(command, {"username", "match_type"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");

            string targetUsername = command.args.at("username");
            string targetMatchType = command.args.at("match_type");
            invitationService.newInvite(loggedInUser->getName(), targetUsername, targetMatchType);
            print(output);
            return;
        }
        if (command.targetCommand == "start_match") {
            if (!hasValidArguments(command, {"invitation_id"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            int invitationId = stoi(command.args.at("invitation_id"));
            invitationService.accpeptInvite(invitationId);
            print(output);
            return;
        }
        if (command.targetCommand == "reject_invitation") {
            if (!hasValidArguments(command, {"invitation_id"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            int invitationId = stoi(command.args.at("invitation_id"));
            invitationService.rejectInvite(invitationId);
            print(output);
            return;
        }
        if (command.targetCommand == "action") {
            if (!hasValidArguments(command, {"action"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            string action = command.args.at("action");
            int matchId = loggedInUser->getOnGoingMatch();
            matchPlayService.executePlayerAction(matchId, loggedInUser->getName(), action);
            print(output);
            return;
        }
        if (command.targetCommand == "report") {
            if (!hasValidArguments(command, {"username", "reason"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid command.");
            string reportedUsername = command.args.at("username");
            string reportedReason = command.args.at("reason");
            string senderUsername = loggedInUser->getName();
            reportService.submitReport(senderUsername, reportedUsername, reportedReason);
            print(output);
            return;
        }
        throw ServiceException(ErrorType::NotFound, "Invalid target command");
    }
    else if (command.baseCommand == "GET") {
        if (command.targetCommand == "casual_match_opponents") {
            SortType sType;
            if (!hasValidArguments(command, {"sort_order"}))
                throw ServiceException(ErrorType::BadRequest, "Invalid argument (sort type is not given) command");
            if (command.args.at("sort_order") == "asc") {
                sType = ascending;
            }
            else if (command.args.at("sort_order") == "desc"){
                sType = descending;
            }
            else {
                throw ServiceException(ErrorType::BadRequest, "Invalid sort order command");
            }
            output = matchMakingService.getOpponentList(loggedInUser->getName(), casual, sType);
            print(output);
            return;
        }
        if (command.targetCommand == "match_status") {
            output = matchPlayService.getMatchStatus(loggedInUser->getOnGoingMatch(), loggedInUser->getName());
            print(output);
            return;
        }
        if (command.targetCommand == "profile") {
            string username;
            if (hasValidArguments(command, {"username"}))
                username = command.args.at("username");
            else
                username = loggedInUser->getName();

            output = reportService.getUserProfile(username);
            print(output);
            return;
        }
        if (command.targetCommand == "received_invitations") {
            output = invitationService.getPendingInvites(loggedInUser->getName());
            print(output);
            return;
        }
        if (command.targetCommand == "reports") {
            output = reportService.getReports(loggedInUser->getPrivilegeLevel());
            print(output);
            return;
        }
        throw ServiceException(ErrorType::NotFound, "Invalid target command");
    }
    throw ServiceException(ErrorType::BadRequest, "Invalid Base command");
}

void RunProgram::run() {
    string inputCommand;
    while (getline(cin, inputCommand)) {
        try {
            Command command = processCommand(inputCommand);
            validateCommand(command);
            dispatchCommand(command);
        }
        catch (ServiceException& error){
            print(error.what());
            cout << "///////debug: " << error.getDetail() << endl;
        }
    }
}

void RunProgram::print(const string& message = "OK") {
    cout << message << endl;
}