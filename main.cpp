#include <complex>
#include <iostream>

#include "DAL/Repository.h"
#include "Logic/RunProgram.h"
#include "Service/RepositoryService.h"

using namespace std;

int main(int argc, char* argv[]) {
    Repository repo;

    RepositoryService repository_service(repo);
    LoginService loginService(repo);
    MatchMakingService matchMakingService(repo);
    InviteService invitationService(repo, matchMakingService);
    MatchPlayService matchPlayService(repo);
    ReportService reportService(repo);

    RunProgram programDirector(repo, loginService, matchMakingService, invitationService, matchPlayService, reportService);

    try {
        if (argc < 3)
            throw invalid_argument("Not enough arguments");

        repository_service.importData(argv[1], argv[2]);
    }
    catch (...) {
        cerr << "Error importing data" << endl;
        return 1;
    }

    programDirector.run();
    return 0;
}
