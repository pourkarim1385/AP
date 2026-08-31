#pragma once

#include <exception>
#include <string>

using namespace std;

enum class ErrorType {
    BadRequest,
    PermissionDenied,
    NotFound,
    MisMatch,
};

class ServiceException : public exception {
private:
    ErrorType type;
    string detail;
public:
    ServiceException(ErrorType type, string detail = "No detail added")
        : type(type), detail(detail) {
    }
    ErrorType getType() const{return type;}
    const char* what() const noexcept override;
    string getDetail() const {return detail;}
};