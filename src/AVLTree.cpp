
#include "AVLTree.h"
#include <algorithm>
using namespace std;

int AVLTree::height(Node* n){ return n? n->height:0; }
int AVLTree::balanceFactor(Node* n){ return n? height(n->left)-height(n->right):0; }

Node* AVLTree::rotateRight(Node* y){
    Node* x=y->left; Node* T2=x->right;
    x->right=y; y->left=T2;
    y->height=max(height(y->left),height(y->right))+1;
    x->height=max(height(x->left),height(x->right))+1;
    return x;
}
Node* AVLTree::rotateLeft(Node* x){
    Node* y=x->right; Node* T2=y->left;
    y->left=x; x->right=T2;
    x->height=max(height(x->left),height(x->right))+1;
    y->height=max(height(y->left),height(y->right))+1;
    return y;
}
Node* AVLTree::insert(Node* node, State key){
    if(!node) return new Node(key);
    if(key < node->key) node->left=insert(node->left,key);
    else if(node->key < key) node->right=insert(node->right,key);
    else return node;
    node->height=1+max(height(node->left),height(node->right));
    int b=balanceFactor(node);
    if(b>1 && key < node->left->key) return rotateRight(node);
    if(b<-1 && node->right->key < key) return rotateLeft(node);
    if(b>1 && node->left->key < key){ node->left=rotateLeft(node->left); return rotateRight(node); }
    if(b<-1 && key < node->right->key){ node->right=rotateRight(node->right); return rotateLeft(node); }
    return node;
}
bool AVLTree::search(Node* node, State key){
    if(!node) return false;
    if(key < node->key) return search(node->left,key);
    else if(node->key < key) return search(node->right,key);
    return true;
}
AVLTree::AVLTree(): root(nullptr) {}
void AVLTree::insert(State key){ root=insert(root,key); }
bool AVLTree::contains(State key){ return search(root,key); }
