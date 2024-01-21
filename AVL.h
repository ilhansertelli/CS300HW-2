//
// Created by ILHAN on 17.11.2023.
//
#include <string>
#include <vector>
using namespace std;

#ifndef CS300_THE2_AVL_H
#define CS300_THE2_AVL_H

template <class Key, class Value>
struct AvlNode {
    Key key;
    Value value;
    AvlNode * left;
    AvlNode * right;
    int height;

    AvlNode(const Key & k, const Value & v,  AvlNode * l = nullptr, AvlNode * r = nullptr, const int h = 0):
    key(k), value(v), left(l), right(r), height(h) {}
};

template <class Key, class Value>
class AVLSearchTree
{

public:
    AVLSearchTree();
    ~AVLSearchTree();

    const Key & findMin() const;
    const Key & findMax() const;
    const AvlNode<Key, Value> * find(const Key & x) const;
    bool isEmpty() const;

    void makeEmpty();
    void insert(const Key & x, const Value & v);
    void remove(const Key & x, const Value & v);

private:
    AvlNode<Key, Value> * root;
    Key ITEM_NOT_FOUND;

    const AvlNode<Key, Value> * elementAt(AvlNode<Key, Value> * t) const;
    void insert(const Key & x, const Value & v, AvlNode<Key, Value> *& t) const;
    void remove(const Key & x, const Value & v, AvlNode<Key, Value> *& t);

    AvlNode<Key, Value> * findMin(AvlNode<Key, Value> * t) const;
    AvlNode<Key, Value> * findMax(AvlNode<Key, Value> * t) const;
    AvlNode<Key, Value> * find(const Key & x, AvlNode<Key, Value> * t) const;

    void makeEmpty(AvlNode<Key, Value> *& t) const;

    // Avl manipulations
    int height(AvlNode<Key, Value> * t) const;
    int max(int lhs, int rhs) const;
    AvlNode<Key, Value> * balance(AvlNode<Key, Value> *t) const;
    void rotateWithLeftChild(AvlNode<Key, Value> *& k2) const;
    void rotateWithRightChild(AvlNode<Key, Value> *& k1) const;
    void doubleWithLeftChild(AvlNode<Key, Value> *& k3) const;
    void doubleWithRightChild(AvlNode<Key, Value> *& k1) const;

};

#include "AVL.cpp"
#endif //CS300_THE2_AVL_H

