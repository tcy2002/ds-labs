#pragma once

#include <vector>
#include <iostream>
#define isRoot(p) ((p)->parent == nullptr)

class AvlTree {
private:
    struct node
    {
        int key;
        int value;
        node *left;
        node *right;
        node *parent;
        int height;
        explicit node(int k, int v, node *p=nullptr):
                key(k), value(v), left(nullptr), right(nullptr), parent(p), height(1) {}
    };
    node *root{};
    int timeOfRotate{};

    void adjust(node *t);
    void connect34(node *, node *, node *, node *, node *, node *, node *);
    static int max(int a, int b){return a>b ? a:b;}
    void Reset(node *p);
    int height(node *p);
    void updateHeight(node *p);
    node *removeMin(node *p);

public:
    AvlTree() {root = nullptr;}
    ~AvlTree() {Reset(root);}

    void insert(int key, int v);
    int search(int key) const;
    void Delete(int key);
    void Reset() {Reset(root); root = nullptr; timeOfRotate = 0;}
    int getRotate() const {return timeOfRotate;}
};

