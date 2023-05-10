//
// Created by Shen on 2022/3/17.
//

#include <iostream>
#include <queue>
#include "AVLTree.h"


AVLTree::AVLTree() : root(nullptr), parent(nullptr) ,search_count(0),rotate_count(0){}

AVLTree::~AVLTree() {
    del(root);
}

void AVLTree::del(TreeNode *p) {
    if (!p) {
        return;
    }
    del(p->lchild);
    del(p->rchild);
    delete p;
}

bool AVLTree::isRoot(TreeNode *p) {
    return !p->parent;
}

bool AVLTree::isLchild(TreeNode *p) {
    return !isRoot(p) && p == p->parent->lchild;
}

bool AVLTree::isRchild(TreeNode *p) {
    return !isRoot(p) && p == p->parent->rchild;
}

/*
 * 在函数中进行一些操作的时候  都会使用一些临时变量（指针）
 * 这些指针虽然指向的内容是一样的  但是修改这些指针并不能修改树
 * 指针的父节点的子节点才是真正能够修改树的指针
 * 如果使用宏定义 可以直接用
 * 但是如果是函数  返回的还是一个rvalue的临时变量
 * 可以用指针的指针或指针的引用
 */
AVLTree::TreeNode *&AVLTree::fromParentTo(TreeNode *p) {
    if (isRoot(p)) {
        return root;
    }
    return isLchild(p) ? p->parent->lchild : p->parent->rchild;
}

AVLTree::TreeNode *AVLTree::tallerChild(TreeNode *p) {
    int lh = stature(p->lchild);
    int rh = stature(p->rchild);
    if (lh == rh) {
        return isLchild(p) ? p->lchild : p->rchild;//等高返回与父亲同侧
    }
    return lh > rh ? p->lchild : p->rchild;
}

int AVLTree::stature(TreeNode *p) {
    return p ? p->height : -1;
}

bool AVLTree::balanced(TreeNode *p) {
//    return stature(p->lchild) == stature(p->rchild);
    int f = balanceFactor(p);
    return f > -2 && f < 2;
}

int AVLTree::balanceFactor(TreeNode *p) {
    return stature(p->lchild) - stature(p->rchild);
}

AVLTree::TreeNode *&AVLTree::searchIn(TreeNode *&p, int key,TreeNode *&pare) {
    search_count++;
    if (!p || p->data == key) {
        return p;
    }

//    std::cout<<"search:"<<p->data<<std::endl;
    parent = p;
    TreeNode *&next = key < p->data ? p->lchild : p->rchild;
    return searchIn(next, key,pare);
}

void AVLTree::updateHeight(TreeNode *p) {
    int lh = stature(p->lchild);
    int rh = stature(p->rchild);
    p->height = 1 + (lh > rh ? lh : rh);
}

AVLTree::TreeNode *&AVLTree::search(int key) {
    parent = nullptr;
    return searchIn(root, key,parent);
}

void AVLTree::connect3(TreeNode *a, TreeNode *b, TreeNode *c) {
    a->lchild = b;
    if (b) {
        b->parent = a;
    }
    a->rchild = c;
    if (c) {
        c->parent = a;
    }
    updateHeight(a);
}

AVLTree::TreeNode *
AVLTree::connect34(TreeNode *a, TreeNode *b, TreeNode *c, TreeNode *t0, TreeNode *t1, TreeNode *t2, TreeNode *t3) {
    connect3(a, t0, t1);
    connect3(c, t2, t3);
    connect3(b, a, c);
    return b;
}

AVLTree::TreeNode *AVLTree::rotateAt(TreeNode *p) {
    rotate_count++;
    TreeNode *v = p->parent;
    TreeNode *g = v->parent;
    if (isLchild(v)) {
        if (isLchild(p)) {
            v->parent = g->parent;
            return connect34(p, v, g, p->lchild, p->rchild, v->rchild, g->rchild);
        }
        p->parent = g->parent;
        return connect34(v, p, g, v->lchild, p->lchild, p->rchild, g->rchild);
    }
    if (isRchild(p)) {
        v->parent = g->parent;
        return connect34(g, v, p, g->lchild, v->lchild, p->lchild, p->rchild);
    }
    p->parent = g->parent;
    return connect34(g, p, v, g->lchild, p->lchild, p->rchild, v->rchild);

}

void AVLTree::Insert(int key, int value) {
//    std::cout<<"insert"<<key<<std::endl;
    TreeNode *&x = search(key);
    if (x) {
        return; //不可重复
    }
    if(parent){

//    std::cout<<"parent:"<<parent->data<<std::endl;
//    std::cout<<"rootp:"<<root->parent<<std::endl;
    }
    x = new TreeNode(key, parent);

    for (TreeNode *g = parent; g; g = g->parent) {
        if (!balanced(g)) {
            fromParentTo(g) = rotateAt(tallerChild(tallerChild(g)));
            break;
        }
        updateHeight(g);
    }
}

void AVLTree::Display() {
    std::queue<TreeNode *> q;
    TreeNode *x = root;
    if (!x) {
        return;
    }
    q.push(x);
    while (!q.empty()) {
        x = q.front();
        q.pop();
        std::cout<< balanceFactor(x)<<" "<<x->data<<" ";
        if (x->lchild) {
            std::cout << x->lchild->data << " ";
            q.push(x->lchild);
        }else{
            std::cout<<" null ";
        }
        if (x->rchild) {
            std::cout << x->rchild->data << std::endl;
            q.push(x->rchild);
        }else{
            std::cout<<" null "<<std::endl;
        }
    }
    std::cout<<stature(root);
}

int AVLTree::getASL()
{
    return search_count;
}

int AVLTree::getInfo()
{
    return rotate_count;
}