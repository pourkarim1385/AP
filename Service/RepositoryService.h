#pragma once

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