#pragma once

#include <iostream>
#include <vector>

class graph
{
private:
    struct verNode;
    struct edgeNode
    {
        int weight;
        verNode *end;
        edgeNode *next;
        explicit edgeNode(int w, verNode *v=nullptr, edgeNode *n=nullptr):
        end(v), weight(w), next(n) {}
    };
    struct verNode
    {
        int val;
        int in_d;
        edgeNode *head;
        verNode *next;
        explicit verNode(int v, int d=-1, verNode *n=nullptr):
        val(v), in_d(d), head(nullptr), next(n) {}
        ~verNode() = default;
    };
    verNode *verList;
    verNode *visit(int i);

public:
    graph(): verList(new verNode(0)) {};
    ~graph();

    void insert(int x, int y, int w);
    void topological();
};

graph::~graph()
{
    verNode *p = verList->next, *tmp;
    while (p) {
        tmp = p;
        p = p->next;
        delete tmp;
    }
    delete verList;
}

graph::verNode *graph::visit(int i) {
    verNode *p = verList;
    while (p->next && p->next->val < i) p = p->next;
    if (!p->next || p->next->val != i)
        p->next = new verNode(i, 0, p->next);
    return p->next;
}

void graph::insert(int x, int y, int w)
{
    verNode *p = visit(x), *n = visit(y);
    p->head = new edgeNode(w, n, p->head);
    n->in_d++;
}

void graph::topological() {
    verNode *p;
    edgeNode *e;
    //顺序输出0入度节点
    while (true) {
        p = verList->next;
        while (p && p->in_d != 0) p = p->next;
        if (p == nullptr) break;

        e = p->head;
        while (e) {
            e->end->in_d--;
            e = e->next;
        }

        std::cout << p->val << " ";
        p->in_d = -1;
    }

    //恢复节点入度
    p = verList->next;
    while (p) {
        e = p->head;
        while (e) {
            e->end->in_d++;
            e = e->next;
        }
        p = p->next;
    }
    p = verList->next;
    while (p) {
        p->in_d++;
        p = p->next;
    }
}
