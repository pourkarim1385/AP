#ifndef TRY1_RUNPROGRAM_H
#define TRY1_RUNPROGRAM_H
#include "../DAL/Repository.h"
#include "../Service/InviteService.h"
#include "../Service/LoginService.h"
#include "../Service/MatchMakingService.h"
#include "../Service/ReportService.h"
#include "../Service/RepositoryService.h"
#include "../Service/Match Services/MatchPlayService.h"

struct Command {
    string baseCommand;
    string targetCommand;
    map<string, string> args;
};

class RunProgram {
    private:
    Repository& repo;
    shared_ptr<User> loggedInUser;
    //services:
    LoginService loginService;
    MatchMakingService matchMakingService;
    MatchPlayService matchPlayService;
    InviteService invitationService;
    ReportService reportService;
    //
    //
    void dispatchCommand(const Command& command);
    void validateCommand(const Command& command);
    bool hasValidArguments(const Command& command,const vector<string>& targetArguments) const;
    map<string, string> processArguments(const string& inputArgs);
    Command processCommand(const string& command);
    bool hasValidBaseCommand(const string& baseCommand);
    void print(const string& message);
    public:
    explicit RunProgram(Repository& repo);
    void run();
};

#endif //TRY1_RUNPROGRAM_H