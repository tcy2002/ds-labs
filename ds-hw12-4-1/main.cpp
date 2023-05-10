#include "MergeSort.h"
#include <random>
#include <vector>
#include <windows.h>

int main() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    std::default_random_engine e(0);
    LARGE_INTEGER end_time;
    double time;
    std::uniform_int_distribution<int> u(0, 5000000);
    std::vector<std::pair<int, std::string>> a;

    int size = 500000;
    for (int i = 0; i < size; i++) {
        a.emplace_back(u(e), "123");
    }

    QueryPerformanceCounter(&begin_time);
    mergeSort_parallel(a.data(), size, 16);
//    mergeSort(a.data(), size);
    QueryPerformanceCounter(&end_time);
    time = (double) (end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double) freq_.QuadPart;

    //正确性检查
//    for (auto &it : a) {
//        std::cout << it.first << " " << it.second << std::endl;
//    }

    std::cout << "\ntime: " << time << " ms" << std::endl;

    return 0;
}
