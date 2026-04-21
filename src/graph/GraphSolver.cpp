

#include "GraphSolver.h"
#include <iostream>
#include <queue>
#include <chrono>
#include <set>
using namespace std;

static bool dfsOne(Board &board, vector<Piece> &pieces, int idx, AVLTree &vis, int &states)
{
    State st(board.getGrid(), {});
    if (vis.contains(st))
        return false;
    vis.insert(st);
    states++;
    if (idx == (int)pieces.size())
        return true;
    Piece &p = pieces[idx];
    for (auto &var : p.variations)
    {
        for (int i = 0; i < board.getRows(); i++)
        {
            for (int j = 0; j < board.getCols(); j++)
            {
                if (board.placePiece(var, i, j, p.id))
                {
                    if (dfsOne(board, pieces, idx + 1, vis, states))
                        return true;
                    board.removePiece(var, i, j);
                }
            }
        }
    }
    return false;
}

static size_t dfsAll(Board &board, vector<Piece> &pieces, int idx, AVLTree &vis, int &states, bool printSolutions, size_t maxSolutions, size_t &found)
{
    if (maxSolutions && found >= maxSolutions)
        return found;
    State st(board.getGrid(), {});
    if (vis.contains(st))
        return found;
    vis.insert(st);
    states++;
    if (idx == (int)pieces.size())
    {
        found++;
        if (printSolutions)
            board.print();
        return found;
    }
    Piece &p = pieces[idx];
    for (auto &var : p.variations)
    {
        for (int i = 0; i < board.getRows(); i++)
        {
            for (int j = 0; j < board.getCols(); j++)
            {
                if (board.placePiece(var, i, j, p.id))
                {
                    dfsAll(board, pieces, idx + 1, vis, states, printSolutions, maxSolutions, found);
                    board.removePiece(var, i, j);
                    if (maxSolutions && found >= maxSolutions)
                        return found;
                }
            }
        }
    }
    return found;
}

GraphSolver::GraphSolver(Board b) : board(b)
{
    pieces = generatePentominoes();
}

bool GraphSolver::isComplete(const std::vector<std::vector<int>> &g) const
{
    for (auto &r : g)
        for (int v : r)
            if (v == 0)
                return false;
    return true;
}

bool GraphSolver::solveDFSOne(int &states, long long &ms, State &solution)
{
    states = 0;
    visited = AVLTree();
    auto start = chrono::high_resolution_clock::now();
    Board b = board;
    bool ok = dfsOne(b, pieces, 0, visited, states);
    auto end = chrono::high_resolution_clock::now();
    ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    if (ok)
        solution = State(b.getGrid(), {});
    return ok;
}

size_t GraphSolver::solveDFSAll(int &states, long long &ms, size_t maxSolutions, bool printSolutions)
{
    states = 0;
    visited = AVLTree();
    auto start = chrono::high_resolution_clock::now();
    Board b = board;
    size_t found = 0;
    dfsAll(b, pieces, 0, visited, states, printSolutions, maxSolutions, found);
    auto end = chrono::high_resolution_clock::now();
    ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return found;
}

bool GraphSolver::solveBFS(int &states, long long &ms, State &solution)
{
    states = 0;
    visited = AVLTree();
    auto start = chrono::high_resolution_clock::now();
    // derive usedPieces from starting grid
    set<int> used;
    for (auto &r : board.getGrid())
        for (int v : r)
            if (v > 0)
                used.insert(v);
    queue<State> q;
    q.push(State(board.getGrid(), used));

    while (!q.empty())
    {
        State cur = q.front();
        q.pop();
        if (visited.contains(cur))
            continue;
        visited.insert(cur);
        states++;
        if (isComplete(cur.grid))
        {
            solution = cur;
            auto end = chrono::high_resolution_clock::now();
            ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            return true;
        }
        // expand neighbors: place any remaining piece anywhere
        // reconstruct board from grid
        Board b(cur.grid);
        for (auto &p : pieces)
        {
            if (cur.usedPieces.count(p.id))
                continue;
            for (auto &var : p.variations)
            {
                for (int i = 0; i < b.getRows(); i++)
                {
                    for (int j = 0; j < b.getCols(); j++)
                    {
                        if (b.placePiece(var, i, j, p.id))
                        {
                            auto ng = b.getGrid();
                            State ns(ng, cur.usedPieces);
                            ns.usedPieces.insert(p.id);
                            if (!visited.contains(ns))
                                q.push(ns);
                            b.removePiece(var, i, j);
                        }
                    }
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    ms = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    return false;
}

void GraphSolver::listPieces()
{
    cout << "Pecas disponiveis:\n";
    for (auto &p : pieces)
        cout << "ID " << p.id << "\n";
}

void GraphSolver::showPieceVariations(const Piece &p)
{
    int k = 0;
    for (auto &var : p.variations)
    {
        cout << "Var " << k++ << ":\n";
        int mr = 0, mc = 0;
        for (auto [r, c] : var)
        {
            mr = max(mr, r);
            mc = max(mc, c);
        }
        vector<vector<int>> g(mr + 1, vector<int>(mc + 1, 0));
        for (auto [r, c] : var)
            g[r][c] = 1;
        for (auto &row : g)
        {
            for (int v : row)
                cout << (v ? '#' : '.');
            cout << "\n";
        }
        cout << "\n";
    }
}

void GraphSolver::playMode()
{
    cout << "Modo Jogar\n";
    vector<bool> used(pieces.size(), false);
    while (true)
    {
        board.print();
        listPieces();
        cout << "Escolha ID (-1 sai): ";
        int id;
        cin >> id;
        if (id == -1)
            break;
        int idx = -1;
        for (int i = 0; i < (int)pieces.size(); i++)
            if (pieces[i].id == id && !used[i])
            {
                idx = i;
                break;
            }
        if (idx == -1)
        {
            cout << "Invalido\n";
            continue;
        }
        auto &p = pieces[idx];
        showPieceVariations(p);
        cout << "Var: ";
        int v;
        cin >> v;
        cout << "Linha: ";
        int r;
        cin >> r;
        cout << "Coluna: ";
        int c;
        cin >> c;
        if (v >= 0 && v < (int)p.variations.size() && board.placePiece(p.variations[v], r, c, p.id))
        {
            used[idx] = true;
            cout << "OK\n";
        }
        else
            cout << "Inválido\n";
    }
}
