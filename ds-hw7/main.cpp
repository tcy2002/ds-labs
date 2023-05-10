#include <iostream>
#include <fstream>
#include <algorithm>
#include <windows.h>
#include "RBTree.h"
#include "AvlTree.h"

LARGE_INTEGER freq_;
LARGE_INTEGER begin_time;
LARGE_INTEGER end_time;
double total{};
int dup = 50;

void timeInsert(const int insert[], int size, RBTree &tree) {
    total = 0;
    QueryPerformanceFrequency(&freq_);
    for (int i = 0; i < dup; i++) {
        tree.Reset();
        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < size; j++) {
            tree.insert(insert[j], insert[j]);
        }
        QueryPerformanceCounter(&end_time);
        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }
    std::cout << "RBTree: \t" << total / dup << "us" << "\tRotate time: " << tree.getRotate() << std::endl;
}

void timeInsert(const int insert[], int size, AvlTree &tree) {
    total = 0;
    QueryPerformanceFrequency(&freq_);
    for (int i = 0; i < dup; i++) {
        tree.Reset();
        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < size; j++) {
            tree.insert(insert[j], insert[j]);
        }
        QueryPerformanceCounter(&end_time);
        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }
    std::cout << "AvlTree:\t" << total / dup << "us" << "\tRotate time: " << tree.getRotate() << std::endl;
}

void timeSearch(const int search[], int size, const RBTree &tree) {
    total = 0;
    QueryPerformanceFrequency(&freq_);
    for (int i = 0; i < dup; i++) {
        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < size; j++) {
            tree.search(search[j]);
        }
        QueryPerformanceCounter(&end_time);
        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }
    std::cout << "RBTree: \t" << total / dup << "us" << std::endl;
}

void timeSearch(const int search[], int size, const AvlTree &tree) {
    total = 0;
    QueryPerformanceFrequency(&freq_);
    for (int i = 0; i < dup; i++) {
        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < size; j++) {
            tree.search(search[j]);
        }
        QueryPerformanceCounter(&end_time);
        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }
    std::cout << "AvlTree:\t" << total / dup << "us" << std::endl;
}

int main() {
    int sizeOfInsert[] = {100, 500, 2000, 5000, 10000};
    AvlTree avl;
    RBTree rbt;

    for (int i : sizeOfInsert) {
        int insert[i];
        std::string path = "../data/insert" + std::to_string(i) + ".txt";
        std::ifstream file(path);
        for (int &n : insert)
            file >> n;
        file.close();

        std::cout << "random insert " << i << std::endl;
        timeInsert(insert, i, rbt);
        timeInsert(insert, i, avl);
        std::sort(insert, insert + i);
        std::cout << "sequential insert " << i << std::endl;
        timeInsert(insert, i, rbt);
        timeInsert(insert, i, avl);
        std::cout << std::endl;
    }

    avl.Reset();
    rbt.Reset();
    int sizeOfSearch[] = {100, 500, 2000, 5000, 8000};

    std::string path1 = "../data/insert10000.txt";
    std::ifstream file1(path1);
    int insert[100000];
    for (int &i : insert) {
        file1 >> i;
    }
    file1.close();
    for (int i : insert) {
        avl.insert(i, i);
        rbt.insert(i, i);
    }

    std::cout << "random insert" << std::endl;
    for (int i : sizeOfSearch) {
        int search[i];
        std::string path = "../data/search" + std::to_string(i) + ".txt";
        std::ifstream file(path);
        for (int &n : search)
            file >> n;
        file.close();

        std::cout << "search " << i << std::endl;
        timeSearch(search, i, avl);
        timeSearch(search, i, rbt);
        std::sort(search, search + i);
    }

    avl.Reset();
    rbt.Reset();
    std::sort(insert, insert + 100000);
    for (int i : insert) {
        avl.insert(i, i);
        rbt.insert(i, i);
    }

    std::cout << "\nsequential insert" << std::endl;
    for (int i : sizeOfSearch) {
        int search[i];
        std::string path = "../data/search" + std::to_string(i) + ".txt";
        std::ifstream file(path);
        for (int &n : search)
            file >> n;
        file.close();

        std::cout << "search " << i << std::endl;
        timeSearch(search, i, avl);
        timeSearch(search, i, rbt);
        std::sort(search, search + i);
    }

    return 0;
}
