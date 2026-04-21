#ifndef STATE_H
#define STATE_H
#include <vector>
#include <set>
using namespace std;

class State
{
public:
    vector<vector<int>> grid;
    set<int> usedPieces;
    State(vector<vector<int>> g = {}, set<int> used = {});
    bool operator<(const State &other) const;
};
#endif
