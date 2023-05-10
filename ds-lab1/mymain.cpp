#include <iostream>
#include <windows.h>
#include <fstream>
#include <iomanip>
#include "ScapegoatTree.h"
#include "AvlTree.h"

#define SCOPE 100000
#define DUP 10

int insert[SCOPE];
int search[SCOPE];

using namespace std;

void test(double alpha) {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    double total{}, total1{}, total2{}, dep{};
    int reb1, reb2;
    for (int i = 0; i < DUP; i++) {
        ScapegoatTree sgt(alpha);

        QueryPerformanceCounter(&begin_time);
        for (int &val : insert) sgt.Insert(val);
        QueryPerformanceCounter(&end_time);
        total += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;
        reb1 = sgt.GetRebalanceTimes();

        QueryPerformanceCounter(&begin_time);
        for (int &val : search) dep += sgt.Search(val);
        QueryPerformanceCounter(&end_time);
        total1 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;

        QueryPerformanceCounter(&begin_time);
        for (int &val : insert) sgt.Delete(val);
        QueryPerformanceCounter(&end_time);
        total2 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;
        reb2 = sgt.GetRebalanceTimes() - reb1;
    }
    total /= DUP;
    total1 /= DUP;
    total2 /= DUP;
    dep /= (DUP * SCOPE);

    cout << "alpha: " << fixed << setprecision(2) << alpha;
    cout << "\tinsert: " << fixed << setprecision(1) << total << "us";
    cout << "\tsearch: " << total1 << "us";
    cout << "\tdelete: " << total2 << "us" << endl;
    cout << "\t\tI-reb: " << reb1;
    cout << "\t\tS-dep: " << fixed << setprecision(3) << dep;
    cout << "\t\tD-reb: " << reb2 << endl;
}

void testAvl() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    double total{}, total1{}, total2{}, dep{};
    int reb1, reb2;
    for (int i = 0; i < DUP; i++) {
        AvlTree avl;

        QueryPerformanceCounter(&begin_time);
        for (int &val : insert) avl.insert(val, 0);
        QueryPerformanceCounter(&end_time);
        total += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;
        reb1 = avl.getRotate();

        QueryPerformanceCounter(&begin_time);
        for (int &val : search) dep += avl.search(val);
        QueryPerformanceCounter(&end_time);
        total1 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;

        QueryPerformanceCounter(&begin_time);
        for (int &val : insert) avl.Delete(val);
        QueryPerformanceCounter(&end_time);
        total2 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double)freq_.QuadPart;
        reb2 = avl.getRotate() - reb1;
    }
    total /= DUP;
    total1 /= DUP;
    total2 /= DUP;
    dep /= (DUP * SCOPE);

    cout << "avltree:\tinsert: " << fixed << setprecision(1) << total << "us";
    cout << "\tsearch: " << total1 << "us";
    cout << "\tdelete: " << total2 << "us" << endl;
    cout << "\t\tI-reb: " << reb1;
    cout << "\t\tS-dep: " << fixed << setprecision(3) << dep;
    cout << "\t\tD-reb: " << reb2 << endl;
}

int main() {
    ifstream ifs("../insert.txt");
    for (int &i : insert) ifs >> i;
    ifs.close();
    ifs.open("../search.txt");
    for (int &i : search) ifs >> i;
    ifs.close();

    double alpha[]{0.6, 0.64, 0.68, 0.72, 0.76};
    for (double &i : alpha) test(i);
    testAvl();

    return 0;
}

