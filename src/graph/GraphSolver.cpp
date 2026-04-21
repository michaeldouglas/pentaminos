
#include "GraphSolver.h"
#include <iostream>
#include <queue>
#include <chrono>
using namespace std;

static bool dfsUtil(Board &board, vector<Piece> &pieces, int idx, AVLTree &vis, int &states)
{
    State st(board.getGrid(), {});
    if (vis.contains(st))
        return false;
    vis.insert(st);
    states++;
    if (idx == (int)pieces.size())
    {
        board.print();
        return true;
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
                    if (dfsUtil(board, pieces, idx + 1, vis, states))
                        return true;
                    board.removePiece(var, i, j);
                }
            }
        }
    }
    return false;
}

GraphSolver::GraphSolver(Board b) : board(b)
{
    pieces = generatePentominoes();
}

void GraphSolver::solveDFS()
{
    cout << "DFS com backtracking + AVL\n";
    int states = 0;
    if (!dfsUtil(board, pieces, 0, visited, states))
        cout << "Nenhuma solucao\n";
    cout << "Estados: " << states << "\n";
}

void GraphSolver::solveBFS()
{
    cout << "BFS (estrutura base)\n";
    queue<State> q;
    q.push(State(board.getGrid(), {}));
    int states = 0;
    auto start = chrono::high_resolution_clock::now();
    while (!q.empty())
    {
        State cur = q.front();
        q.pop();
        if (visited.contains(cur))
            continue;
        visited.insert(cur);
        states++;
        bool complete = true;
        for (auto &r : cur.grid)
            for (int c : r)
                if (c == 0)
                    complete = false;
        if (complete)
        {
            cout << "Solução BFS\n";
            break;
        }
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Estados: " << states << "\n";
    cout << "Tempo(ms): " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "\n";
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
