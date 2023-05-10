//
// Created by Shen on 2022/3/17.
//

#ifndef LAB0_AVLTREE_H
#define LAB0_AVLTREE_H
#include "Container.h"


class AVLTree: public Container {
private:
    int search_count;
    int rotate_count;
    struct TreeNode{
        int data;
        int height;
        TreeNode *parent;
        TreeNode *lchild;
        TreeNode *rchild;
        TreeNode():parent(nullptr),lchild(nullptr),rchild(nullptr),height(0),data(-1){}
        TreeNode(int key ,TreeNode* p):parent(p),lchild(nullptr),rchild(nullptr),height(0),data(key){}
    };
    TreeNode *root;
    TreeNode *parent; //查找操作时 修改parent为找到的节点的父节点 方便插入


    int stature(TreeNode *p);//高度 空树-1 叶节点0
    bool balanced(TreeNode *p);
    int balanceFactor(TreeNode *p);//平衡因子

    bool isRoot(TreeNode *p);
    bool isLchild(TreeNode *p);//是否是父结点的左儿子
    bool isRchild(TreeNode *p);
    TreeNode*& fromParentTo(TreeNode *p);
    TreeNode* tallerChild(TreeNode *p);

    void connect3(TreeNode *a,TreeNode *b,TreeNode *c);
    TreeNode* connect34(TreeNode *a,TreeNode *b,TreeNode *c,TreeNode *t0,TreeNode *t1,TreeNode *t2,TreeNode *t3);
    TreeNode* rotateAt(TreeNode *p);
    TreeNode*& searchIn(TreeNode *&p ,int key,TreeNode *&pare);
    void updateHeight(TreeNode *p);

    void del(TreeNode *p);
public:
    AVLTree();
    ~AVLTree();
    TreeNode*& search(int key);
    void Insert(int key,int value);
    void Display();
    int getASL();
    int getInfo();
};


#endif //LAB0_AVLTREE_H
