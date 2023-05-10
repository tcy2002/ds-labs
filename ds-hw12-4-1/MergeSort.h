#pragma once

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex mtx;
int on_working = 0;

void increase() {
    mtx.lock();
    on_working++;
    mtx.unlock();
}

void decrease() {
    mtx.lock();
    on_working--;
    mtx.unlock();
}

template<class KEY, class OTHER>
void _mergeSort_parallel(std::pair<KEY, OTHER> a[], int low, int high, int threadNum);
template<class KEY, class OTHER>
void _mergeSort(std::pair<KEY, OTHER> a[], int low, int high);
template<class KEY, class OTHER>
void _merge2(std::pair<KEY, OTHER> a[], int low, int mid, int high);

template<class KEY, class OTHER>
void mergeSort_parallel(std::pair<KEY, OTHER> a[], int size, int threadNum) {
    _mergeSort_parallel(a, 0, size - 1, threadNum - 1);
}

template<class KEY, class OTHER>
void _mergeSort_parallel(std::pair<KEY, OTHER> a[], int low, int high, int threadNum) {
    if (low >= high) return;
    int mid = (low + high) / 2;
    std::vector<std::thread> th;

    if (on_working < threadNum) {
        increase();
        th.template emplace_back(_mergeSort_parallel<KEY, OTHER>, a, low, mid, threadNum);

    } else _mergeSort_parallel(a, low, mid, threadNum);

    if (on_working < threadNum) {
        increase();
        th.template emplace_back(_mergeSort_parallel<KEY, OTHER>, a, mid + 1, high, threadNum);
    } else _mergeSort_parallel(a, mid + 1, high, threadNum);

    for (auto &it: th) {
        it.join();
        decrease();
    }

    _merge2(a, low, mid + 1, high);
}

template<class KEY, class OTHER>
void mergeSort(std::pair<KEY, OTHER> a[], int size) {
    _mergeSort(a, 0, size - 1);
}

template<class KEY, class OTHER>
void _mergeSort(std::pair<KEY, OTHER> a[], int low, int high) {
    if (low >= high) return;
    int mid = (low + high) / 2;
    _mergeSort(a, low, mid);
    _mergeSort(a, mid + 1, high);
    _merge2(a, low, mid + 1, high);
}

template<class KEY, class OTHER>
void _merge2(std::pair<KEY, OTHER> a[], int low, int mid, int high) {
    auto *tmp = new std::pair<KEY, OTHER>[high - low + 1];

    int k = 0, m = low, n = mid;

    while (m < mid && n <= high)
        tmp[k++] = (a[m].first < a[n].first ? a[m++] : a[n++]);
    while (m < mid) tmp[k++] = a[m++];
    while (n <= high) tmp[k++] = a[n++];

    for (int i = low; i <= high; i++) a[i] = tmp[i - low];

    delete []tmp;
}

