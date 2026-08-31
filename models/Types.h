#pragma once

#include <map>
#include <string>
#include <ostream>

const std::string INVALID_STR = "INVALID";

enum PrivilegeLevel {
    adminAccess, normal,
};

enum playerStatus {
    unready, ready
};

enum SortType {
    ascending ,descending
};

enum MatchStatus {
    finished, onGoing,
};

enum MatchType {
    casual, ranked,
};

enum RankLevel {
    Bronze, Silver, Gold, Plat,
};

enum MatchResult {
    win, lose,
};

enum class PenaltyType {
    Health, Bullet
};

struct Command {
    std::string baseCommand;
    std::string targetCommand;
    std::map<std::string, std::string> args;
};

using namespace std;

inline ostream& operator<<(ostream& os, RankLevel level) {
    switch (level) {
        case RankLevel::Bronze: os << "Bronze"; break;
        case RankLevel::Silver: os << "Silver"; break;
        case RankLevel::Gold:   os << "Golden"; break;
        case RankLevel::Plat:   os << "Platinum"; break;
        default:
            os << "Unknown";
    }
    return os;
}

inline ostream& operator<<(ostream& os, MatchType type) {
    switch (type) {
        case MatchType::casual: os << "casual"; break;
        case MatchType::ranked: os << "ranked"; break;

        default:
            os << "Unknown";
    }
    return os;
}
