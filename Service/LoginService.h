#ifndef TRY1_LOGICSERVICE_H
#define TRY1_LOGICSERVICE_H
#include "../DAL/Repository.h"


class LoginService {
private:
    Repository& repo;
    bool isValidUsername(const string &username) const;

    public:
    LoginService(Repository& repo);
    bool registerUser(const string &name, const string &password);
    bool loginUser(const string &name, const string &password);
};


#endif //TRY1_LOGICSERVICE_H