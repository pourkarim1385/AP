#pragma once

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