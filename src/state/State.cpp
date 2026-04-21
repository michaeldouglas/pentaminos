
#include "State.h"
State::State(vector<vector<int>> g, set<int> used): grid(g), usedPieces(used) {}
bool State::operator<(const State& other) const {
    return grid < other.grid;
}
