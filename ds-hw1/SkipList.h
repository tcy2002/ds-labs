#pragma once

#include <vector>
#include <climits>
#include <time.h>

#include "Container.h"
#include <iostream>


enum SKNodeType
{
    HEAD = 1,
    NORMAL,
    NIL
};

struct SKNode
{
    int key;
    int val;
    SKNodeType type;
    std::vector<SKNode *> forwards;
    SKNode(int v,int _key, int _val, SKNodeType _type)
        : key(_key), val(_val), type(_type)
    {
        for (int i = 0; i < v; ++i)
        {
            forwards.push_back(nullptr);
        }
    }
};

class SkipList : public Container
{
public:
    static int MAX_LEVEL;//L(N) = log(1/p)(n)
    static double p;
private:
    int search_count;
    int move_pointer_count;
    SKNode *head;
    SKNode *NIL;
    std::vector<SKNode *> update;
    void update_create();
    void update_clear();
    //unsigned long long s = 1;
    double my_rand();
    int randomLevel();
    int level;
public:
    SkipList();
    void Insert(int key, int value);
    int Search(int key);//返回查找路径长度
    void Delete(int key);
    void TimeSearch(int key_start, int key_end);
    void Display();
    ~SkipList()
    {
        SKNode *n1 = head;
        SKNode *n2;
        while (n1)
        {
            n2 = n1->forwards[0];
            delete n1;
            n1 = n2;
        }
    }
    int getASL();
    int getInfo();
};
