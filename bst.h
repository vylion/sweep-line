#ifndef BST_H
#define BST_H

#include <iostream>
#include "helpers.h"

template <typename T>
class BST
{
private:
    struct node
    {
        int key;
        T *content;
        node *l;
        node *r;
        node *p;
    };
    node *root = NULL;
    node *insert(node *n, const int &key, const T *content, bool &success);
    node *search(const node *n, const int &key);
    node *min(node *n);
    node *max(node *n);
    node *succ(node *n);
    node *pred(node *n);

public:
    bool insert(const int &key, const T *content);
    T *search(const int &key);
    T *min();
    T *max();
    T *succ(const int &key);
    T *pred(const int &key);
    bool remove(const int &key);
};

#endif // !BST_H