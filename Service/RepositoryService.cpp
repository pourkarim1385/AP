#include "RepositoryService.h"

#include "../DAL/Repository.h"
#include "../models/CsvFile.h"

void RepositoryService::importData(const string& usersCsvPath, const string& adminsCsvPath) {
    UserCsvFile userCsvFile(usersCsvPath);
    AdminCsvFile adminCsvFile(adminsCsvPath);
    repo.importPlayers(userCsvFile.exportFile());
    repo.importAdmins(adminCsvFile.exportFile());
}
