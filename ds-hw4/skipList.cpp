#include "skipList.h"

double SkipList::my_rand()
{
    s = (16807 * s) % 2147483647ULL;
    return (s + 0.0) / 2147483647ULL;
}

int SkipList::randomLevel()
{
    int result = 1;
    while (result < max_level && my_rand() < rate)
    {
        ++result;
    }
    return result;
}

void SkipList::insert(int key, int value)
{
    // TODO
    SKNode *x = head;
    SKNode *update[max_level];
    for (int i = 0; i < max_level; i++)
        update[i] = nullptr;

    for (int i = max_level - 1; i >= 0; i--) {
        while (x->forwards[i]->key < key) {x = x->forwards[i];}
        update[i] = x;
    }

    auto *newNode = new SKNode(key, value, NORMAL, max_level);
    int grow = randomLevel();
    for (int i = 0; i < grow; i++) {
        newNode->forwards[i] = update[i]->forwards[i];
        update[i]->forwards[i] = newNode;
    }
}

void SkipList::Search(int key, int &length)
{
    // TODO
    SKNode* x = head;
    for (int i = max_level - 1; i >= 0; i--) {
        while (x->forwards[i]->key < key) {
            length++;
            x = x->forwards[i];
        }

        length++;
    }
    length++;
}

void SkipList::Delete(int key)
{
    // TODO
    SKNode *x = head, *tmp = nullptr;
    for (int i = max_level - 1; i >= 0; i--) {
        while (x->forwards[i]->key < key)
            x = x->forwards[i];
        if (x->forwards[i]->key == key) {
            if (!i) tmp = x->forwards[i];
            x->forwards[i] = x->forwards[i]->forwards[i];
        }
    }
    delete tmp;
}

void SkipList::Reset() {
    SKNode *n1 = head->forwards[0];
    SKNode *n2;
    while (n1->key < NIL->key)
    {
        n2 = n1->forwards[0];
        delete n1;
        n1 = n2;
    }
    for (int i = 0; i < max_level; ++i)
        head->forwards[i] = NIL;
}

void SkipList::Display()
{
    for (int i = max_level - 1; i >= 0; --i)
    {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL)
        {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }
        std::cout << "-->N" << std::endl;
    }
}