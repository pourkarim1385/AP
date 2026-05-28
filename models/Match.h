#ifndef TRY1_MATCH_H
#define TRY1_MATCH_H
#include <map>
#include <string>
#include <vector>


using namespace std;

enum matchSatus {
    finished, onGoing,
};

enum matchType {
    casual,
};

class Match {
    protected:
    int id;
    vector<string> playersUsername;
    matchSatus status;
    matchType type;
    string winner;
    int turn;
    public:
    explicit Match(int id, const vector<string>& players);
    virtual void updatePlayerMoves(const string& player, const string& move) = 0;
    virtual matchType getType() = 0;
    virtual bool isFinished() {return status == matchSatus::finished;}
    const vector<string>& getPlayersList() {return playersUsername;}
    virtual int getId() const {return id;}
    virtual string getStatus(const string& playerName){}
};

struct casualPlayer {
    string name;
    int bullets;
    vector<string> moves;
};

class CasualMath : public Match {
private:
    map<string, casualPlayer> casualPlayers;

    public:
    CasualMath(int id, const vector<string>& players);
    void addBullet(const string& player);
    void deductBullet(const string& player);
    string getHistory() const;
    virtual void updatePlayerMoves(const string& player, const string& move);
    bool playerHasMoved(const string& player);
    bool playerHasBullet(const string& player);
    bool turnIsFinished();
    string getRoundWinner();
    void goNextRound();
    void closeMatch();
    virtual matchType getType() override;
    virtual string getStatus(const string& playerName) override;
};


#endif //TRY1_MATCH_H