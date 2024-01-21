//
// Created by ILHAN on 17.11.2023.
//
#include <iostream>
#include <string>
#include "AVL.h"
using namespace std;

// ------------------------------- PUBLIC --------------------------------------
template <class Key, class Value>
AVLSearchTree<Key, Value>::AVLSearchTree() {
    root = nullptr;
}
// Taken from the lecture slides
template <class Key, class Value>
AVLSearchTree<Key, Value>::~AVLSearchTree()
{
    makeEmpty();
}
// Taken from the lecture slides
template <class Key, class Value>
const AvlNode<Key, Value> * AVLSearchTree<Key, Value>::find(const Key & x) const
{
    return elementAt(find(x, root));
}
// Taken from the lecture slides
template <class Key, class Value>
const Key & AVLSearchTree<Key, Value>::findMin() const
{
    return elementAt(findMin(root));
}
// Taken from the lecture slides
template <class Key, class Value>
const Key & AVLSearchTree<Key, Value>::findMax() const
{
    return elementAt(findMax(root));
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty()
{
    makeEmpty(root);
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key & x, const Value & v)
{
    insert(x, v, root);
}

template <typename Key, typename Value>
void AVLSearchTree<Key, Value>::remove(const Key & x, const Value & v) {
    remove(x, v, root);
}
// Taken from the lecture slides
template <class Key, class Value>
bool AVLSearchTree<Key, Value>::isEmpty() const {
    return (root == nullptr);
}

// ------------------------------- PRIVATE --------------------------------------

template <class Key,class Value>
int AVLSearchTree<Key, Value>::height(AvlNode<Key, Value> *t) const {
    if (t == nullptr) {
        return -1; // Height of an empty tree is -1
    } else {
        // Recursively calculate the height of the left and right subtrees
        int leftHeight = height (t->left);
        int rightHeight = height(t->right);

        // Return the maximum height of the left and right subtrees, plus 1 for the current node
        return 1 + max(leftHeight, rightHeight);
    }
}

template <class Key,class Value>
int AVLSearchTree<Key, Value>::max(int lhs, int rhs) const{
    if (lhs > rhs) {
        return lhs;
    } else {
        return rhs;
    }
}
// Taken from the lecture slides
template <class Key, class Value>
const AvlNode<Key, Value> * AVLSearchTree<Key, Value>::elementAt(AvlNode<Key, Value> * t) const
{
    if (t == nullptr)
        return nullptr;
    else
        return t;
}
// Taken from the lecture slides
template <class Key, class Value>
AvlNode<Key, Value> * AVLSearchTree<Key, Value>::find(const Key & x, AvlNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    else if(x < t->key)
        return find(x, t->left);
    else if(t->key < x)
        return find(x, t->right);
    else
        return t; // Match
}
// Taken from the lecture slides
template <class Key, class Value>
AvlNode<Key, Value> * AVLSearchTree<Key, Value>::findMin(AvlNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    if(t->left == nullptr)
        return t;
    return findMin(t->left);
}
// Taken from the lecture slides
template <class Key, class Value>
AvlNode<Key, Value> * AVLSearchTree<Key, Value>::findMax(AvlNode<Key, Value> * t) const
{
    if(t == nullptr)
        return nullptr;
    if(t->right == nullptr)
        return t;
    return findMin(t->right);
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::insert(const Key & x, const Value & v, AvlNode<Key, Value> *& t) const
{
    if(t == nullptr) //  create a new node at the right place
        t = new AvlNode<Key, Value>(x, v);
    else if( x < t->key ) {
        insert(x, v, t->left);
        if (height( t->left ) - height( t->right ) == 2) {
            if (x < t->left->key)  // X was inserted to the left-left subtree!
                rotateWithLeftChild(t);
            else                 // X was inserted to the left-right subtree!
                doubleWithLeftChild(t);
        }
    }
    else if(t->key < x) {
        insert(x, v, t->right);  // right subtree
        if (height( t->right) - height(t->left) == 2) {
            // height of the right subtree increased
            if (t->right->key < x)
                // X was inserted to right-right subtree
                rotateWithRightChild(t);
            else // X was inserted to right-left subtree
                doubleWithRightChild(t);
        }
    }
    else
        ;  // Duplicate; do nothing
    t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template <class Key, class Value>
void AVLSearchTree<Key, Value>::remove(const Key & x, const Value & v, AvlNode<Key, Value> *& t) {
    if (t != nullptr) {
        if (x < t->key) {
            // Key might be in the left subtree
            remove(x, v, t->left);
        } else if (x > t->key) {
            // Key might be in the right subtree
            remove(x, v, t->right);
        } else {
            // Key found, perform deletion

            // Node with only one child or no child
            if (t->left == nullptr || t->right == nullptr) {
                AvlNode<Key, Value> *temp;
                if (t->left) {
                    temp = t->left;
                } else {
                    temp = t->right;
                }
                // No child case
                if (temp == nullptr) {
                    temp = t;
                    t = nullptr;
                } else {
                    // One child case
                    *t = *temp; // Copy the contents of the non-empty child
                }

                delete temp;
            } else {
                // Node with two children, get the inorder successor (smallest in the right subtree)
                AvlNode<Key, Value> *temp = findMin(t->right);

                // Copy the inorder successor's data to this node
                t->key = temp->key;
                t->value = temp->value;

                // Delete the inorder successor
                remove(temp->key, temp->value, t->right);
            }
        }

        // Update height and balance factor of the current node after the recursive calls
        height(t);

        // Perform AVL tree balancing
        t = balance(t);
    }
}


template <class Key, class Value>
AvlNode<Key, Value>* AVLSearchTree<Key, Value>::balance(AvlNode<Key, Value> *t) const {
    if (t == nullptr) {
        return nullptr;
    }

    // Update height of the current node
    height(t);

    // Check the balance factor
    int balanceFactor = height(t->right) - height(t->left);

    // Left Heavy
    if (balanceFactor > 1) {
        // Determine whether it's a left-left or left-right case
        if (height(t->right->left) <= height(t->right->right)) {
            rotateWithLeftChild(t);
        } else {
            doubleWithLeftChild(t);
        }
    }
        // Right Heavy
    else if (balanceFactor < -1) {
        // Determine whether it's a right-right or right-left case
        if (height(t->left->right) <= height(t->left->left)) {
            rotateWithRightChild(t);
        } else {
            doubleWithRightChild(t);
        }
    }

    return t;
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithLeftChild(AvlNode<Key, Value> *& k2) const
{
    if (k2 == nullptr || k2->left == nullptr) {
        // Add appropriate error handling or return, depending on your design
        return;
    }
    AvlNode<Key, Value> * k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::rotateWithRightChild (AvlNode<Key, Value> *& k1) const
{
    if (k1 == nullptr || k1->right == nullptr) {
        // Add appropriate error handling or return, depending on your design
        return;
    }
    AvlNode<Key, Value> * k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithLeftChild(AvlNode<Key, Value> *& k3) const
{
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::doubleWithRightChild(AvlNode<Key, Value> *& k1) const
{
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}
// Taken from the lecture slides
template <class Key, class Value>
void AVLSearchTree<Key, Value>::makeEmpty(AvlNode<Key, Value> *& t) const
{
    if(t != nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr;
}



