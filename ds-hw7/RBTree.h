#pragma once
#include <iostream>

typedef enum {BLACK, RED} node_color;
#define isRoot(p) ((p)->parent == nullptr)
#define isRed(p) ((p) && (p)->color == RED)

class RBTree {
private:
    struct node
    {
        int key;
        int value;
        node *left;
        node *right;
        node *parent;
        int height;
        node_color color;
        explicit node(int k, int v, node *p=nullptr, node_color c=RED):
                key(k), value(v), color(c), left(nullptr), right(nullptr), parent(p), height(1) {}
    };
    node *root{};
    int timeOfRotate{};

    void insert(int x, int v, node *&p, node *par);
    void adjust(node *t);
    void connect34(node *, node *, node *, node *, node *, node *, node *);
    static int max(int a, int b){return a>b ? a:b;}
    int search(int x, node *p) const;
    void Reset(node *&p);
    int height(node *p);

public:
    RBTree() {root = nullptr;}
    ~RBTree() {Reset(root);}

    void insert(int x, int v) {insert(x, v, root, nullptr);}
    int search(int key) const {return search(key, root);}
    void Reset() {Reset(root); root = nullptr; timeOfRotate = 0;}
    int getRotate() const {return timeOfRotate;}
};