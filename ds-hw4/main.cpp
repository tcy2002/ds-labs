#include "avlTree.h"
#include "skipList.h"
#include <random>
#include <iostream>
#include <windows.h>

bool cmp(int a, int b) {return b < a;}
bool duplicated(const int a[], int b, int size) {
    for (int i = 0; i < size; ++i) if (a[i] == b) return true;
    return false;
}

void AvlInsert(int insert[], int n) {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    avlTree avl;
    double total = 0;

    for (int i = 0; i < 50; i++) {
        avl.Reset();

        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < n; j++) {
            avl.insert(insert[j], 0);
        }
        QueryPerformanceCounter(&end_time);

        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }

    std::cout << n << ":\t" << total / 50 << "us " << "(avlTree)" << std::endl;
}

void SkipInsert(int insert[], int n) {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    SkipList skip(n, 0.5);
    double total = 0;

    for (int i = 0; i < 50; i++) {
        skip.Reset();

        QueryPerformanceCounter(&begin_time);
        for (int j = 0; j < n; j++) {
            skip.insert(insert[j], 0);
        }
        QueryPerformanceCounter(&end_time);

        total += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;
    }

    std::cout << n << ":\t" << total / 50 << "us " << "(skipList)" << std::endl;
}

int main(){
    int size = 50000;
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 100000);
    int *insert = new int[size];
    for (int i = 0; i < size; i++)
        while (duplicated(insert, insert[i] = u(e), i));

    std::cout << "Normal insert: " << std::endl;
    AvlInsert(insert, 1000);
    AvlInsert(insert, 2000);
    AvlInsert(insert, 5000);
    AvlInsert(insert, 10000);
    AvlInsert(insert, 20000);
    AvlInsert(insert, 50000);
    SkipInsert(insert, 50000);

    std::sort(insert, insert + size);
    std::cout << "\nAscend insert: " << std::endl;
    AvlInsert(insert, 50000);
    SkipInsert(insert, 50000);

    std::sort(insert, insert + size, cmp);
    std::cout << "\nDescend insert: " << std::endl;
    AvlInsert(insert, 50000);
    SkipInsert(insert, 50000);


    return 0;
}
