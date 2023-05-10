#include "AvlTree.h"

void AvlTree::insert(int x, int v, node *&p, node *par) {
    if (p == nullptr) {
        p = new node(x, v, par);
        adjust(p);
        return;
    }
    if (x == p->key) {p->value = v; return;}
    if (x < p->key) insert(x, v, p->left, p);
    else insert(x, v, p->right, p);
}

void AvlTree::adjust(node *t) {
    int diff{};
    while (t->parent != nullptr) {
        t = t->parent;
        int lh = height(t->left), rh = height(t->right);
        t->height = max(lh, rh) + 1;
        diff = lh - rh;
        if (diff < -1 || diff > 1) break;
    }
    if (diff >= -1 && diff <= 1) return;

    node *p1, *p2, *p3, *c1, *c2, *c3, *c4;

    if (diff > 1) {
        p3 = t;
        if (height(t->left->left) > height(t->left->right)) {
            p2 = t->left;
            p1 = p2->left;
            c2 = p1->right;
            c3 = p2->right;
        } else {
            p1 = t->left;
            p2 = p1->right;
            c2 = p2->left;
            c3 = p2->right;
        }
    } else {
        p1 = t;
        if (height(t->right->right) > height(t->right->left)) {
            p2 = t->right;
            p3 = p2->right;
            c2 = p2->left;
            c3 = p3->left;
        } else {
            p3 = t->right;
            p2 = p3->left;
            c2 = p2->left;
            c3 = p2->right;
        }
    }
    c1 = p1->left;
    c4 = p3->right;
    p2->parent = t->parent;

    connect34(p1, p2, p3, c1, c2, c3, c4);

    if (isRoot(p2)) { root = p2; return; }
    t = p2->parent;
    t->key > p2->key ? t->left = p2 : t->right = p2;
}

void AvlTree::connect34(node *p1, node *p2, node *p3, node *c1, node *c2, node *c3, node *c4) {
    p1->left = c1;
    p1->right = c2;
    p3->left = c3;
    p3->right = c4;
    p2->left = p1;
    p2->right = p3;

    p1->parent = p2;
    p3->parent = p2;
    if (c1 != nullptr) c1->parent = p1;
    if (c2 != nullptr) c2->parent = p1;
    if (c3 != nullptr) c3->parent = p3;
    if (c4 != nullptr) c4->parent = p3;

    p1->height = max(height(c1), height(c2)) + 1;
    p3->height = max(height(c3), height(c4)) + 1;
    p2->height = max(p1->height, p2->height) + 1;

    timeOfRotate++;
}

int AvlTree::search(int x, node *p) const {
    if (p == nullptr) return -1;
    if (p->key == x) return p->value;
    if (p->key > x) return search(x, p->left);
    else return search(x, p->right);
}

void AvlTree::Reset(node *&p) {
    if (p == nullptr) return;
    Reset(p->left);
    Reset(p->right);
    delete p;
}

int AvlTree::height(node *p){
    if (p == nullptr) return 0;
    return p->height;
}
