#include "ServiceException.h"

const char *ServiceException::what() const noexcept {
    switch (type) {
        case ErrorType::BadRequest:       return "Bad Request\n";
        case ErrorType::PermissionDenied: return "Permission Denied\n";
        case ErrorType::NotFound:         return "Not Found\n";
        default:                          return "Unknown Error\n";
    }
}
