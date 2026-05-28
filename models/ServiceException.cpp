#include "ServiceException.h"

const char *ServiceException::what() const noexcept {
    switch (type) {
        case ErrorType::BadRequest:       return "Bad Request";
        case ErrorType::PermissionDenied: return "Permission Denied";
        case ErrorType::NotFound:         return "Not Found";
        default:                          return "Unknown Error";
    }
}
