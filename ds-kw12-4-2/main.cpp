#include "ThreadPool.h"
#include <iostream>
#include <vector>
#include <windows.h>
#include <random>
using namespace std;

template <class KEY, class VALUE>
void merge(std::pair<KEY, VALUE> data[], int low, int mid, int high)
{
    auto *tmp = new std::pair<KEY, VALUE>[high - low + 1];
    int k = 0, m = low, n = mid + 1;

    while (m <= mid && n <= high)
        tmp[k++] = (data[m].first < data[n].first ? data[m++] : data[n++]);
    while (m <= mid) tmp[k++] = data[m++];
    while (n <= high) tmp[k++] = data[n++];

    for (m = 0; m < k; m++) data[low++] = tmp[m];
    delete []tmp;
}

template<class KEY, class VALUE>
void mergeSort(std::pair<KEY, VALUE> data[], int size, int threadNum)
{
    int low, mid, high;
    std::vector<std::thread> th;

    for(int i = 1; i < size; i *= 2) {
        low = 0;
        mid = low + i - 1;
        high = mid + i;

        while(high < size) {
            if (i <= 1024) merge(data, low, mid, high);
            else th.template emplace_back(merge<KEY, VALUE>, data, low, mid, high);
            low = high + 1;
            mid = low + i - 1;
            high = mid + i;
        }
        if(low < size && mid < size) {
            if (i <= 1024) merge(data, low, mid, size - 1);
            else th.template emplace_back(merge<KEY, VALUE>, data, low, mid, size - 1);
        }

        for (auto &it : th) it.join();
        th.clear();
    }
}

int main()
{
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    std::default_random_engine e(0);
    LARGE_INTEGER end_time;
    double time;
    std::uniform_int_distribution<int> u(0, 5000000);
    vector<std::pair<int, std::string>> a;

    int size = 1000000;
    for (int i = 0; i < size; i++) {
        a.emplace_back(u(e), "123");
    }

    QueryPerformanceCounter(&begin_time);
    mergeSort(a.data(), size, 4);
    QueryPerformanceCounter(&end_time);
    time = (double) (end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double) freq_.QuadPart;

    //正确性检查
//    for (auto &it : a) {
//        std::cout << it.first << std::endl;
//    }

    std::cout << "\ntime: " << time << " ms" << std::endl;

    return 0;
}