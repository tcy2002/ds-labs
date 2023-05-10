#include "ScapegoatTree.h"
#include <iostream>
#include <cmath>
#include <stack>

int ScapegoatTree::GetRebalanceTimes() const
{
	// TODO
	return rebalanceTimes;
}

int ScapegoatTree::Search(int key)
{
    // TODO
    int depth = 0;
    node *p = root;

    while (p != nullptr) {
        if (p->key == key) {
            break;
        }
        if (key < p->key) p = p->lc;
        else p = p->rc;
        depth++;
    }

    return depth;
}

ScapegoatTree::node *ScapegoatTree::findScapeGoat(node *p) {
    if (p == nullptr) return nullptr;
    int limit = floor(calSize(p) * alpha);
    int sizeOfLc = calSize(p->lc), sizeOfRc = calSize(p->rc);
    if (sizeOfLc > limit || sizeOfRc > limit) return p;
    return findScapeGoat(p->parent);
}

int ScapegoatTree::calSize(node *p) {
    if (p == nullptr) return 0;
    else return calSize(p->lc) + calSize(p->rc) + 1;
}

void ScapegoatTree::inOrder(node *p, node *arr[], int &pos) {
    if (p == nullptr) return;
    inOrder(p->lc, arr, pos);
    arr[pos++] = p;
    inOrder(p->rc, arr, pos);
}

ScapegoatTree::node *ScapegoatTree::rebuild(node *arr[], int start, int end, node *p) {
    if (start > end) return nullptr;
    int mid = (start + end) / 2;
    arr[mid]->lc = rebuild(arr, start, mid - 1, arr[mid]);
    arr[mid]->rc = rebuild(arr, mid + 1, end, arr[mid]);
    arr[mid]->parent = p;
    return arr[mid];
}

void ScapegoatTree::Rebuild(node *p) {
    rebalanceTimes++;
    if (p == nullptr) return;

    int stSize = calSize(p), pos = 0;
    node **arr = new node*[stSize];
    inOrder(p, arr, pos);
    node *res = rebuild(arr, 0, stSize - 1, p->parent);
    delete []arr;

    if (p == root) root = res;
    else {
        if (res->parent->lc == p)
            res->parent->lc = res;
        else res->parent->rc = res;
    }
}

void ScapegoatTree::Insert(int key)
{
    // TODO
    if (root == nullptr) {
        root = new node(key);
        size++;
        setMaxSize();
        return;
    }

    int depth = 0;
    node *p = root;

    while (p) {
        if (key < p->key) {
            if (p->lc == nullptr) {
                p->lc = new node(key, p);
                size++;
                depth++;
                setMaxSize();
                break;
            }
            else p = p->lc;
        } else if (key > p->key) {
            if (p->rc == nullptr) {
                p->rc = new node(key, p);
                size++;
                depth++;
                setMaxSize();
                break;
            }
            else p = p->rc;
        } else break;
        depth++;
    }

    if (depth > floor(log(size) / (-log(alpha))))
        Rebuild(findScapeGoat(p));

//    traverse();
}

int ScapegoatTree::removeMin(node *p) {
    int res;
    if (p->lc == nullptr) {
        p->parent->rc = p->rc;
    } else {
        while (p->lc) p = p->lc;
        p->parent->lc = p->rc;
    }
    if (p->rc) p->rc->parent = p->parent;
    res = p->key;
    delete p;
    return res;
}

void ScapegoatTree::Delete(int key)
{
    // TODO
    node *p = root;

    while (p != nullptr) {
        if (key == p->key) {
            if (p->rc && p->lc) p->key = removeMin(p->rc);
            else {
                if (p->rc) {
                    if (p == root) root = p->rc;
                    else if (p == p->parent->lc) p->parent->lc = p->rc;
                    else p->parent->rc = p->rc;
                    p->rc->parent = p->parent;
                } else {
                    if (p == root) root = p->rc;
                    else if (p == p->parent->lc) p->parent->lc = p->lc;
                    else p->parent->rc = p->lc;
                    if (p->lc) p->lc->parent = p->parent;
                }
                delete p;
            }
            size--;
            break;
        }
        if (key < p->key) p = p->lc;
        else p = p->rc;
    }

    if (size <= alpha * max_size) {
        Rebuild(root);
        max_size = size;
    }

//    traverse();
}

void ScapegoatTree::traverse() const {
    vector<node *> que;
    node *tmp;

    cout << "current tree: " << endl;
    que.insert(que.begin(), root);
    while (!que.empty()) {
        tmp = que.back();
        que.pop_back();
        cout << tmp->key << " ";
        if (tmp->lc) que.insert(que.begin(), tmp->lc);
        if (tmp->rc) que.insert(que.begin(), tmp->rc);
    }
    cout << endl;
}

void ScapegoatTree::Reset(node *p) {
    if (p == nullptr) return;
    Reset(p->lc);
    Reset(p->rc);
    delete p;
}
