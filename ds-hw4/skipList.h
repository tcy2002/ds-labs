#pragma once

#include <iostream>
#include <valarray>
#include <ctime>
#include <malloc.h>

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
    SKNode **forwards;
    SKNode(int _key, int _val, SKNodeType _type, int max)
            : key(_key), val(_val), type(_type)
    {
        forwards = new SKNode*[max];
        for (int i = 0; i < max; ++i)
            forwards[i] = nullptr;
    }
};

class SkipList
{
private:
    SKNode *head;
    SKNode *NIL;
    unsigned long long s = 1;
    double my_rand();
    int randomLevel();
    int max_level;
    double rate;

public:
    SkipList(int size, double rate): rate(rate)
    {
        max_level = log(size) / log(1.0 / rate) + 1;
        head = new SKNode(0, 0, SKNodeType::HEAD, max_level);
        NIL = new SKNode(INT_MAX, 0, SKNodeType::NIL, max_level);
        for (int i = 0; i < max_level; ++i)
        {
            head->forwards[i] = NIL;
        }
    }
    void insert(int key, int value);
    void Search(int key, int &length);
    void Delete(int key);
    void Reset();
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
};

