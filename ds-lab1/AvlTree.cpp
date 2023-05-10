#include "AvlTree.h"

void AvlTree::insert(int key, int v) {
    if (root == nullptr) {
        root = new node(key, v);
        return;
    }

    node *p = root;

    while (p) {
        if (key < p->key) {
            if (p->left == nullptr) {
                p->left = new node(key, v, p);
                updateHeight(p);
                adjust(p->parent);
                break;
            }
            else p = p->left;
        } else if (key > p->key) {
            if (p->right == nullptr) {
                p->right = new node(key, v, p);
                updateHeight(p);
                adjust(p->parent);
                break;
            }
            else p = p->right;
        } else break;
    }
}

int AvlTree::search(int key) const {
    int depth = 0;
    node *p = root;

    while (p != nullptr) {
        if (p->key == key) {
            break;
        }
        if (key < p->key) p = p->left;
        else p = p->right;
        depth++;
    }

    return depth;
}

AvlTree::node *AvlTree::removeMin(node *p) {
    if (p->left == nullptr) {
        p->parent->right = p->right;
    } else {
        while (p->left) p = p->left;
        p->parent->left = p->right;
    }
    if (p->right) p->right->parent = p->parent;
    updateHeight(p->parent);
    return p;
}

void AvlTree::Delete(int key) {
    node *p = root, *tmp;

    while (p != nullptr) {
        if (key == p->key) {
            if (p->right && p->left) {
                tmp = removeMin(p->right);
                p->key = tmp->key;
                adjust(tmp->parent);
                delete tmp;
            }
            else {
                if (p->right) {
                    if (p == root) root = p->right;
                    else if (p == p->parent->left) p->parent->left = p->right;
                    else p->parent->right = p->right;
                    p->right->parent = p->parent;
                } else {
                    if (p == root) root = p->right;
                    else if (p == p->parent->left) p->parent->left = p->left;
                    else p->parent->right = p->left;
                    if (p->left) p->left->parent = p->parent;
                }
                updateHeight(p->parent);
                adjust(p->parent);
                delete p;
            }
            return;
        }
        if (key < p->key) p = p->left;
        else p = p->right;
    }
}

void AvlTree::updateHeight(node *p) {
    if (p == nullptr) return;
    int h = max(height(p->left), height(p->right)) + 1;
    if (h != p->height) {
        p->height = h;
        if (p->parent) updateHeight(p->parent);
    }
}

void AvlTree::adjust(node *t) {
    int diff{};
    while (t != nullptr) {
        diff = height(t->left) - height(t->right);
        if (diff < -1 || diff > 1) break;
        t = t->parent;
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
    p2->height = max(p1->height, p3->height) + 1;

    timeOfRotate++;
}

void AvlTree::Reset(node *p) {
    if (p == nullptr) return;
    Reset(p->left);
    Reset(p->right);
    delete p;
}

int AvlTree::height(node *p){
    if (p == nullptr) return 0;
    return p->height;
}
