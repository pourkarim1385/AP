#include "CsvFile.h"
#include <fstream>
#include <sstream>


void CsvFile::readFile() {
    fstream file;
    file.open(fileAddress);
    if (!file.is_open()) {
        throw invalid_argument("Error opening file");
    }

    string line;

    getline(file, line);
    processLine(line, header);
    while (getline(file, line)) {
        vector<string> setLine;
        processLine(line, setLine);
        content.push_back(setLine);
    }

    file.close();
}

void CsvFile::trimHeader(string &str) {
    if (!str.empty() && str.back() == '\r') {
        str.pop_back();
    }
}

void CsvFile::processLine(const string &queryLine, vector<string> &setLine) {
    istringstream lineStream(queryLine);
    string item;
    while (getline(lineStream, item, ',')) {
        setLine.push_back(item);
    }
}

vector<unique_ptr<Player>> UserCsvFile::exportFile() {
    vector<unique_ptr<Player>> result;
    for (auto& data : content) {
        try {
            const string username = data[usernameIndex], password = data[passwordIndex];
            const int xp = stoi(data[xpIndex]);
            result.push_back(make_unique<Player>(username, password, xp));
        }
        catch (const invalid_argument &e) {
            const string username = data[usernameIndex], password = data[passwordIndex];
            result.push_back(make_unique<Player>(username, password));
        }
    }
    return result;
}

vector<unique_ptr<Admin>> AdminCsvFile::exportFile() {
    vector<unique_ptr<Admin>> result;
    for (auto& data : content) {
        const string username = data[usernameIndex], password = data[passwordIndex];
        result.push_back(make_unique<Admin>(username, password));
    }
    return result;
}
