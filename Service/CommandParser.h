#pragma once

#include <map>
#include <string>

using namespace std;

struct Command;

class CommandParser {
private:
    map<string, string> processArguments(const string& inputArgs);
public:
    Command processCommand(const string& command);
};