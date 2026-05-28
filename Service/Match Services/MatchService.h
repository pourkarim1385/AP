#ifndef TRY1_MATCHSERVICE_H
#define TRY1_MATCHSERVICE_H
#include "../../DAL/Repository.h"


class MatchService {
    protected:
    Repository& repo;
public:
    explicit MatchService(Repository& repo) : repo(repo) {};
    virtual ~MatchService() = default;
    virtual void handleAction(shared_ptr<CasualMath> match, const string& action, const string& playerName) = 0;
};

#endif //TRY1_MATCHSERVICE_H