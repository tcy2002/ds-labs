#include <windows.h>
#include "cuckoo.h"

void generateData(long long *insert, long long size) {
    long long i;
    for (i = 0; i < size / 2; i++)
        insert[i] = i + 1;
    for (i = size / 2; i < size; i++)
        insert[i] = (i - size / 2 + 1) * (size / 2 + 1);
}

void test(long long size, long long insert[]) {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    double total;
    cuckooHash c(100000);
    long long i;

    for (i = 0; i < size; i++)
        c.Insert(insert[i], 0);

    int dup = 1000;
    QueryPerformanceCounter(&begin_time);
    for (int j = 0; j < dup; j++) {
        for (i = 0; i < size; i++)
            c.Lookup(insert[i]);
    }
    QueryPerformanceCounter(&end_time);
    total = (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;

    std::cout << "load factor: " << (double)size / 100000.0 << " time: " << total / (double)size / dup << "us" << std::endl;
}

int main() {
    long long insert[100000];
    long long num[] = {10000, 50000, 75000, 100000};


    for (long long i : num) {
        generateData(insert, i);
        test(i, insert);
    }

    return 0;
}
