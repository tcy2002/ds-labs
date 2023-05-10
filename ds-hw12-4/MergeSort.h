#pragma once

#include <iostream>
#include <vector>
#include <thread>

template<class KEY, class OTHER>
void _mergeSort(std::pair<KEY, OTHER> a[], int low, int high);
template<class KEY, class OTHER>
void _merge2(std::pair<KEY, OTHER> a[], int low, int mid, int high);
template<class KEY, class OTHER>
void _mergeN(std::pair<KEY, OTHER> a[], int *start, const int *end, int size, int way);

/**
 * 多线程归并，首层采用N路归并，申请N个线程，每层递归采用2路归并
 */
template<class KEY, class OTHER>
void mergeSortN(std::pair<KEY, OTHER> a[], int size, int threadNum) {
    std::vector<std::thread> working_buffer;
    auto start = new int[threadNum];
    auto end = new int[threadNum];

    int block_size = size / threadNum;
    int way;
    for (way = 0; way < threadNum; way++) {
        start[way] = way * block_size;
        end[way] = start[way] + block_size - 1;
    }
    end[threadNum - 1] = size - 1;

    for (way = 0; way < threadNum; way++)
        working_buffer.template emplace_back(_mergeSort<KEY, OTHER>, a, start[way], end[way]);

    for (auto &it : working_buffer) it.join();

    _mergeN(a, start, end, size, threadNum);
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

template<class KEY, class OTHER>
void _mergeN(std::pair<KEY, OTHER> a[], int *start, const int *end, int size, int way) {
    auto *tmp = new std::pair<KEY, OTHER>[size];

    int k = 0;
    int idx, x, min, min_idx;

    while (true) {
        min = INT32_MAX;
        for (idx = 0; idx < way; idx++) {
            x = start[idx];
            if (x <= end[idx] && a[x].first < min) {
                min_idx = idx;
                min = a[x].first;
            }
        }
        tmp[k++] = a[start[min_idx]];
        start[min_idx]++;
        if (k == size) break;
    }

    for (k = 0; k < size; k++) a[k] = tmp[k];

    delete []tmp;
}

