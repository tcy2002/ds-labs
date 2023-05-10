#include "avlTree.h"

void avlTree::insert(int x, int v, node *&p){
    if (p == nullptr){
        p = new node(x, v);
    }
    if (x < p->key){
        insert(x, v, p->left);
        if (height(p->left) - height(p->right) > 1) {
            if (x > p->left->key) LR(p);
            else LL(p);
        }
        p->height = max(height(p->left), height(p->right)) + 1;
    } else if (x > p->key) {
        insert(x, v, p->right);
        if (height(p->right) - height(p->left) > 1) {
            if (x < p->right->key) RL(p);
            else RR(p);
        }
        p->height = max(height(p->right), height(p->left)) + 1;
    }
}


bool avlTree::remove(int x, node *&p){
    if (p == nullptr)
        return true;
    if (x == p->key){
        if (p->left == nullptr || p->right == nullptr){
            node *old = p;
            p = (p->left!=nullptr) ? p->left:p->right;
            delete old;
            return false;
        }
        else{
            node *tmp = p->right;
            while(tmp->left != nullptr)
                tmp = tmp->left;
            p->key = tmp->key;
            if (remove(tmp->key, p->right))
                return true;
            return adjust(p, 1);
        }
    }
    if (x < p->key){
        if (remove(x, p->left))
            return true;
        return adjust(p, 0);
    }
    else{
        if (remove(x, p->right))
            return true;
        return adjust(p, 1);
    }
}

bool avlTree::adjust(node *&p, int sub){
    if (sub){
        if (height(p->left) - height(p->right) == 1)
            return true;
        if (height(p->left) == height(p->left)){
            p->height--;
            return false;
        }
        if (height(p->left->right) > height(p->left->left)){
            LR(p);
            return false;
        }
        LL(p);
        if (height(p->right) == height(p->left))
            return false;
        else
            return true;
    }
    else{
        if (height(p->right) - height(p->left) == 1)
            return true;
        if (height(p->right) == height(p->right)){
            p->height--;
            return false;
        }
        if (height(p->right->left) > height(p->right->right)){
            RL(p);
            return false;
        }
        RR(p);
        if (height(p->left) == height(p->right))
            return false;
        else
            return true;
    }
}

void avlTree::LL(node *&p){
    node *p1 = p->left;
    p->left = p1->right;
    p1->right = p;
    p->height = max(height(p->left), height(p->right)) + 1;
    p1->height = max(height(p1->left), height(p)) + 1;
    p = p1;
}

void avlTree::RR(node *&p){
    node *p1 = p->right;
    p->right = p1->left;
    p1->left = p;
    p->height = max(height(p->right), height(p->left)) + 1;
    p1->height = max(height(p1->right), height(p)) + 1;
    p = p1;
}

int avlTree::height(node *p){
    if (p == nullptr)
        return 0;
    else return p->height;
}

void avlTree::Display(){
    std::vector<stNode> st;
    std::cout << "mid traversal: ";

    if (root == nullptr)
        return;
    stNode r(root);

    st.push_back(r);
    while (!st.empty()){
        r = st.back();
        st.pop_back();
        if (++r.t == 2){
            std::cout << "(" << r.p->key << "," << r.p->val << ") ";
            if (r.p->right != nullptr){
                stNode cr(r.p->right);
                st.push_back(cr);
            }
        }
        else{
            st.push_back(r);
            if (r.p->left != nullptr){
                stNode cr(r.p->left);
                st.push_back(cr);
            }
        }
    }
    std::cout << std::endl;
}

void avlTree::Reset(node *&p) {
    if (p == nullptr) return;
    Reset(p->left);
    Reset(p->right);
    delete p;
}

void avlTree::Reset() {
    if (root == nullptr) return;
    Reset(root->left);
    Reset(root->right);
    root = nullptr;
}