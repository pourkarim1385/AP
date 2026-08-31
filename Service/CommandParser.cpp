#include "CommandParser.h"
#include <sstream>
#include "../models/Types.h"

map<string, string> CommandParser::processArguments(const string& inputArgs) {
    map<string, string> arguments;
    int len = inputArgs.length();
    int i = 0;

    for (; i < len; ) {
        while (i < len && inputArgs[i] == ' ') {
            i++;
        }
        
        if (i == len) {
            break;
        }

        string title = "";
        while (i < len && inputArgs[i] != ' ' && inputArgs[i] != '"') {
            title += inputArgs[i];
            i++;
        }

        while (i < len && inputArgs[i] == ' ') {
            i++;
        }

        if (i == len || inputArgs[i] != '"') {
            arguments[title] = INVALID_STR;
            while (i < len && inputArgs[i] != ' ') {
                i++;
            }
            continue;
        }
        
        i++; 
        
        string content = "";
        bool isClosed = false;
        
        for (; i < len; i++) {
            if (inputArgs[i] == '"') {
                isClosed = true;
                i++; 
                break;
            }
            content += inputArgs[i];
        }

        if (!isClosed) {
            arguments[title] = "INVALID";
        } else {
            arguments[title] = content;
        }
    }

    return arguments;
}

Command CommandParser::processCommand(const string& command) {
    Command result;
    istringstream stream(command);
    string cmd, arguments;
    getline(stream, cmd, '?');
    getline(stream, arguments);

    istringstream cmdStream(cmd);
    cmdStream >> result.baseCommand;
    cmdStream >> result.targetCommand;

    result.args = processArguments(arguments);
    return result;
}