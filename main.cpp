#include "DAL/Repository.h"
#include "Logic/RunProgram.h"
#include "Service/RepositoryService.h"

int main(int argc, char* argv[]) {
    Repository repo;
    RepositoryService repository_service(repo);
    RunProgram programDirector(repo);
    repository_service.importData("D:\\Uni\\AP\\HWM\\5\\try1\\users.csv", "D:\\Uni\\AP\\HWM\\5\\try1\\admin.csv");
    programDirector.run();
}
