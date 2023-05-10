#pragma once

#include <vector>
#include <iostream>

class avlTree{
private:
    struct node{
        int key;
        int val;
        node *right;
        node *left;
        int height;
        explicit node(int x, int v, node *l=nullptr, node *r=nullptr, int h=1):
            key(x),
            val(v),
            height(h),
            left(l),
            right(r)
        {}
    };
    struct stNode{
        node *p;
        int t;
        explicit stNode(node *n = nullptr):p(n), t(0){}
    };
    node *root;

    void insert(int x, int v, node *&p);
    bool remove(int x, node *&p);
    bool adjust(node *&p, int sub);
    void LL(node *&p);
    void RR(node *&p);
    void LR(node *&p){RR(p->left); LL(p);}
    void RL(node *&p){LL(p->right); RR(p);}
    int height(node *p);
    int max(int a, int b){return a>b ? a:b;}
    void Reset(node *&p);

public:
    avlTree(){root = nullptr;}
    void insert(int x, int v){insert(x, v, root);};
    void remove(int x){remove(x, root);};
    void Reset();
    void Display();
};

