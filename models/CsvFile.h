#pragma once

#include <memory>
#include <string>
#include <vector>

#include "User.h"

using namespace std;

class CsvFile {
protected:
    string fileAddress;
    vector<string> header;
    vector<vector<string>> content;
    int usernameIndex = 0; int passwordIndex = 1;
    void readFile();
    CsvFile(const string &fileAddress) : fileAddress(fileAddress) {readFile();};
private:
    void processLine(const string &queryLine, vector<string> &setLine);
    void trimHeader(string &str);
};

class UserCsvFile : public CsvFile {
private:
    const int xpIndex = 2;
    const int rpIndex = 3;

public:
    vector<unique_ptr<Player>> exportFile();
    UserCsvFile(const string &fileAddress) : CsvFile(fileAddress) {};
};

class AdminCsvFile : public CsvFile {
    private:
    public:
    vector<unique_ptr<Admin>> exportFile();
    AdminCsvFile(const string &fileAddress) : CsvFile(fileAddress) {};
};
