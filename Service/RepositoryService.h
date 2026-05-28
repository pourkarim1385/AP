#ifndef TRY1_REPOSITORYSERVICE_H
#define TRY1_REPOSITORYSERVICE_H
#include <string>


class Repository;
using namespace std;
class RepositoryService {
    private:
    Repository& repo;

    public:
    explicit RepositoryService(Repository& repo) : repo(repo) {};
    void importData(const string& usersCsvPath, const string& adminsCsvPath);
};


#endif //TRY1_REPOSITORYSERVICE_H