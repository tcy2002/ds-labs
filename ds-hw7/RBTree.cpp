#include "RBTree.h"

void RBTree::insert(int key, int value, node *&t, node *p) {
    if (t == nullptr) {
        t = new node(key, value, p, p == nullptr ? BLACK : RED);
        adjust(t);
        return;
    }
    if (key == t->key) { t->value = value; return; }
    if (key < t->key) insert(key, value, t->left, t);
    else insert(key, value, t->right, t);
}

void RBTree::adjust(node *t) {
    if (!isRed(t->parent)) return;
    t = t->parent->parent;

    if (isRed(t->left) && isRed(t->right)) {
        t->left->color = t->right->color = BLACK;
        t->left->height++;
        t->right->height++;
        if (isRoot(t)) { t->height++; return; }
        t->color = RED;
        adjust(t);
        return;
    }

    node *p1, *p2, *p3, *c1, *c2, *c3, *c4;

    if (isRed(t->left)) {
        p3 = t;
        if (isRed(t->left->left)) {
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
        if (isRed(t->right->right)) {
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

void RBTree::connect34(node *p1, node *p2, node *p3, node *c1, node *c2, node *c3, node *c4) {
    p2->color = BLACK;
    p1->color = p3->color = RED;

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

    p1->height = max(height(c1), height(c2));
    p3->height = max(height(c3), height(c4));
    p2->height = max(p1->height, p3->height) + 1;

    timeOfRotate++;
}

int RBTree::search(int x, node *p) const {
    if (p == nullptr) return -1;
    if (p->key == x) return p->value;
    if (p->key > x) return search(x, p->left);
    else return search(x, p->right);
}

void RBTree::Reset(node *&p) {
    if (p == nullptr) return;
    Reset(p->left);
    Reset(p->right);
    delete p;
}

int RBTree::height(node *p){
    if (p == nullptr) return 0;
    return p->height;
}
