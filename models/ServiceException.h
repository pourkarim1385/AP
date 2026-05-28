#ifndef TRY1_SERVICEEXCEPTION_H
#define TRY1_SERVICEEXCEPTION_H
#include <exception>
#include <string>

using namespace std;

enum class ErrorType {
    BadRequest,
    PermissionDenied,
    NotFound,
};


class ServiceException : public exception {
private:
    ErrorType type;
    string detail;
public:
    ServiceException(ErrorType type, std::string detail)
        : type(type), detail(std::move(detail)) {
    }
    ErrorType getType() const{return type;}
    const char* what() const noexcept override;
    string getDetail() const {return detail;}
};


#endif //TRY1_SERVICEEXCEPTION_H