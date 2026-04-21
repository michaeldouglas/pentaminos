#ifndef AVL_TREE_H
#define AVL_TREE_H
#include "State.h"

struct Node
{
    State key;
    Node *left;
    Node *right;
    int height;
    Node(State s) : key(s), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree
{
private:
    Node *root;
    int height(Node *n);
    int balanceFactor(Node *n);
    Node *rotateRight(Node *y);
    Node *rotateLeft(Node *x);
    Node *insert(Node *node, State key);
    bool search(Node *node, State key);

public:
    AVLTree();
    void insert(State key);
    bool contains(State key);
};
#endif
