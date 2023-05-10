#include <iostream>
#include <random>
#include <windows.h>
#include <algorithm>
#include "AVL.h"
#include "splay.h"

bool duplicated(const int a[], int b, int size) {
    for (int i = 0; i < size; ++i) if (a[i] == b) return true;
    return false;
}

int main() {
    int n = 1000, m = 1000000, k[] = {5, 10, 20, 200, 500, 800};
    std::default_random_engine e;
    std::uniform_int_distribution<int> u(0, 10000);
    int insert[n], order[n];
    for (int i = 0; i < n; i++) {
        while (duplicated(insert, insert[i] = u(e), i));
        order[i] = insert[i];
    }
    std::sort(order, order + n);

    AVL *avl = nullptr;
    for (int i : insert) {
        avl = Insert(avl, i);
    }

    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    double totalAvl[6]{}, totalSpl[6]{};

    for (int rate = 0; rate < 6; rate++) {
        std::uniform_int_distribution<int> s(n / 2 - k[rate] / 2, n / 2 + k[rate] / 2);
//        std::uniform_int_distribution<int> s(n - k[rate], n);
//        std::uniform_int_distribution<int> s(0, k[rate]);


        AVL *avl_tmp;
        QueryPerformanceCounter(&begin_time);
        for (int i = 0; i < m; i++) avl_tmp = Search(avl, order[s(e)]);
        QueryPerformanceCounter(&end_time);
        totalAvl[rate] += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;

        splay *spl = nullptr;
        for (int i: insert) {
            spl = Insert(i, spl);
        }

        QueryPerformanceCounter(&begin_time);
        for (int i = 0; i < m; i++)  spl = Search(order[s(e)], spl);
        QueryPerformanceCounter(&end_time);
        totalSpl[rate] += (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq_.QuadPart;

        Reset(spl);
    }

    for (int i = 0; i < 6; i++) {
        std::cout << k[i] / 10.0 << "%: " << totalAvl[i] << "us (avlTree)" << std::endl;
        std::cout << k[i] / 10.0 << "%: " << totalSpl[i] << "us (splayTree)" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
