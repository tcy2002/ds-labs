#include <iostream>
#include <stdlib.h>

#include "SkipList.h"

int SkipList::MAX_LEVEL = 12;
double SkipList::p = 0.36787944117144233;

double SkipList::my_rand() {
//    s = (16807 * s) % 2147483647ULL;
//    return (s + 0.0) / 2147483647ULL;
    return rand() / double(RAND_MAX);
}

int SkipList::randomLevel() {
    int result = 1;
    while (result < MAX_LEVEL && my_rand() < p) {
        ++result;
    }
    return result;
}

void SkipList::update_create() {
    for (int i = 0; i < MAX_LEVEL; ++i) {
        update.push_back(nullptr);
    }
}

void SkipList::update_clear() {
    for (int i = 0; i < MAX_LEVEL; ++i) {
        update[i] = nullptr;
    }
}

SkipList::SkipList() {
    search_count = 0;
    move_pointer_count = 0;
    srand((unsigned) time(NULL));
    head = new SKNode(MAX_LEVEL, 0, 0, SKNodeType::HEAD);
    NIL = new SKNode(1, INT_MAX, 0, SKNodeType::NIL);
    level = 1;
    for (int i = 0; i < MAX_LEVEL; i++) {
        head->forwards[i] = NIL;
    }
    update_create();
}

void SkipList::Insert(int key, int value) {
    // TODO
    update_clear();
    SKNode *x = head;
    for (int i = level - 1; i >= 0; i--) {
        search_count++;
        while (x->forwards[i]->key < key) {
            search_count++;
            x = x->forwards[i];
        }
        update[i] = x;
    }
    search_count++;
    x = x->forwards[0];
    if (x->key == key) {
        x->val = value;
        return;
    }

    int v = randomLevel();
    if (v > level) {
        for (int i = level; i < v; i++) {
            update[i] = head;
            move_pointer_count++;
        }
        level = v;
    }
    x = new SKNode(v, key, value, NORMAL);
    for (int i = 0; i < v; i++) {
        x->forwards[i] = update[i]->forwards[i];
        update[i]->forwards[i] = x;
        move_pointer_count += 2;
    }
}

void print(int level, SKNode *x) {
    std::cout << level << ",";
    switch (x->type) {
        case HEAD:
            std::cout << "h ";
            break;
        case NORMAL:
            std::cout << x->key << " ";
            break;
        case SKNodeType::NIL:
            std::cout << "N ";
            break;
    }
}

int SkipList::Search(int key) {
    int count = 0;
    SKNode *x = head;
    for (int i = level - 1; i >= 0; i--) {
        count++;
        while (x->forwards[i]->key < key) {
            x = x->forwards[i];
            count++;
        }
    }
    x = x->forwards[0];
    count++;
//	if(x->key == key){
//		std::cout<<x->val<<std::endl;
//	}else{
//		std::cout<<"Not Found"<<std::endl;
//	}
    return count;
}

void SkipList::Delete(int key) {
    // TODO
    update_clear();
    SKNode *x = head;
    for (int i = level - 1; i >= 0; i--) {
        while (x->forwards[i]->key < key) {
            x = x->forwards[i];
        }
        update[i] = x;
    }
    x = x->forwards[0];
    if (x->key != key) {
        return;
    }
    for (int i = 0; i < level; i++) {
        if (update[i]->forwards[i] != x) {
            break;
        }
        update[i]->forwards[i] = x->forwards[i];
    }
    delete x;
    while (level > 1 && head->forwards[level - 1] == NIL) {
        level -= 1;
    }
}

void SkipList::Display() {
    for (int i = level - 1; i >= 0; --i) {
        std::cout << "Level " << i + 1 << ":h";
        SKNode *node = head->forwards[i];
        while (node->type != SKNodeType::NIL) {
            std::cout << "-->(" << node->key << "," << node->val << ")";
            node = node->forwards[i];
        }

        std::cout << "-->N" << std::endl;
    }
}

int SkipList::getASL() {
    return search_count;
}

int SkipList::getInfo()
{
    return move_pointer_count;
}