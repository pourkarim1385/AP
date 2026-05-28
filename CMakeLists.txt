cmake_minimum_required(VERSION 4.0)
project(try1)

set(CMAKE_CXX_STANDARD 20)

add_executable(try1 main.cpp
        models/User.cpp
        models/User.h
        models/Match.cpp
        models/Match.h
        models/Invitation.cpp
        models/Invitation.h
        models/Report.cpp
        models/Report.h
        models/CsvFile.cpp
        models/CsvFile.h
        DAL/Repository.cpp
        DAL/Repository.h
        Logic/RunProgram.cpp
        Logic/RunProgram.h
        Service/LoginService.cpp
        Service/LoginService.h
        Service/InviteService.cpp
        Service/InviteService.h
        Service/MatchMakingService.cpp
        Service/MatchMakingService.h
        Service/RepositoryService.cpp
        Service/RepositoryService.h
        models/ServiceException.cpp
        models/ServiceException.h
        "Service/Match Services/MatchService.cpp"
        "Service/Match Services/MatchService.h"
        "Service/Match Services/CasualMatchService.cpp"
        "Service/Match Services/CasualMatchService.h"
        "Service/Match Services/MatchPlayService.cpp"
        "Service/Match Services/MatchPlayService.h"
        Service/ReportService.cpp
        Service/ReportService.h)
