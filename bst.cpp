#include "bst.h"

template <typename T>
typename BST<T>::node *BST<T>::insert(BST<T>::node *n, const int &key, const T *content, bool &success)
{
    if (n == NULL)
    {
        n = new BST<T>::node;
        n->key = key;
        n->content = content;
        n->l = NULL;
        n->r = NULL;
        n->p = NULL;
    }
    else if (key == n->key)
    {
        success = false;
        return n;
    }
    else if (key > n->key)
    {
        n->r = insert(n->r, key, content);
        n->r->p = n;
    }
    else
    {
        n->l = insert(n->l, key, content);
        n->l->p = n;
    }

    return n;
}

template <typename T>
bool BST<T>::insert(const int &key, const T *content)
{
    bool success = true;
    root = insert(root, key, content, success);
    return success;
}

template <typename T>
typename BST<T>::node *BST<T>::search(const BST<T>::node *n, const int &key)
{
    if (n == NULL)
        return NULL;
    else if (n->key == key)
    {
        return n;
    }
    else if (key > n->key)
    {
        return search(n->r, key);
    }
    else
    {
        return search(n->l, key);
    }
}

template <typename T>
T *BST<T>::search(const int &key)
{
    node *n = search(root, key);

    if (n == NULL)
        return NULL;

    return n->content;
}

template <typename T>
typename BST<T>::node *BST<T>::min(BST<T>::node *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    else if (n->l == NULL)
    {
        return n;
    }
    else
        return min(n->l);
}

template <typename T>
typename BST<T>::node *BST<T>::max(BST<T>::node *n)
{
    if (n == NULL)
    {
        return NULL;
    }
    else if (n->r == NULL)
    {
        return n;
    }
    else
        return min(n->r);
}

template <typename T>
T *BST<T>::min()
{
    node *n = min(root);
    if (n == NULL)
    {
        return NULL;
    }
    return n->content;
}

template <typename T>
T *BST<T>::max()
{
    node *n = max(root);
    if (n == NULL)
    {
        return NULL;
    }
    return n->content;
}

template <typename T>
typename BST<T>::node *BST<T>::succ(BST<T>::node *n)
{
    if (n->r != NULL)
    {
        return min(n->r);
    }
    else
    {
        node *parent = n->p;
        node *current = n;

        while (parent != NULL and current == parent->r)
        {
            current = parent;
            parent = current->p;
        }

        return parent;
    }
}

template <typename T>
T *BST<T>::succ(const int &key)
{
    node *n = search(root, key);

    return n == NULL ? NULL : succ(n);
}

template <typename T>
typename BST<T>::node *BST<T>::pred(BST<T>::node *n)
{
    if (n->l != NULL)
    {
        return max(n->l);
    }
    else
    {
        node *parent = n->p;
        node *current = n;

        while (parent != NULL and current == parent->l)
        {
            current = parent;
            parent = current->p;
        }

        return parent;
    }
}

template <typename T>
T *BST<T>::pred(const int &key)
{
    node *n = search(root, key);

    return n == NULL ? NULL : pred(n);
}
