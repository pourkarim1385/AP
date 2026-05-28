#include "LoginService.h"

#include "../models/ServiceException.h"

LoginService::LoginService(Repository &repo) : repo(repo) {

}

bool LoginService::loginUser(const string &name, const string &password) {
    shared_ptr<User> targetUser = repo.getUserByName(name);
    if (!targetUser) {
        throw ServiceException(ErrorType::NotFound, "User not found");
    }
    if (!targetUser->matchesPassword(password)) {
        throw ServiceException(ErrorType::PermissionDenied, "Invalid password");
    }
    return true;
}

bool LoginService::isValidUsername(const string &username) const{
    bool isAlreadyRegistered = repo.getUserByName(username) ? true : false;
    return !isAlreadyRegistered;
}

bool LoginService::registerUser(const string &name, const string &password) {
    if (!isValidUsername(name)) {
        throw ServiceException(ErrorType::BadRequest, "Username already exists");
    }
    repo.addPlayer(name, password);
    return true;
}