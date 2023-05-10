#include <iostream>
#include <functional>
#include <cstring>

using namespace std;

typedef unsigned char byte;

class BFilter {
private:
    byte *table;
    int n;
    int m;
    int k;
    size_t myHash(int key);
public:
    BFilter(int numOfKey, int sizeOfTable, int numOfHash):n(numOfKey), m(sizeOfTable), k(numOfHash) {
        table = new byte[sizeOfTable]();
    }
    ~BFilter()=default;
    void insert(int low, int high);
    int find(int key);
    double faultRate(int low, int high);
};

size_t BFilter::myHash(int key){
    hash<string> h;
    return h(to_string(key)) % m;
}

void BFilter::insert(int low, int high) {
    for (int key = low; key <= high; key++) {
        for (int i = 0; i < k; i++) {
            table[myHash( key + n * i)] = 1;
        }
    }
}

int BFilter::find(int key) {
    int sum = 0;
    for (int i = 0; i < k; i++) {
        if (table[myHash(key + n * i)]) sum++;
    }
    if (sum == k) return 1;
    return 0;
}

double BFilter::faultRate(int low, int high) {
    int size = high - low + 1;
    int faults = 0;
    for (int key = low; key <= high; key++) {
        faults += find(key);
    }
    return 1.0 * faults / size;
}

int main() {
    int mn[4] = {2, 3, 4, 5};
    int k[5] = {1, 2, 3, 4, 5};
    int n = 10000;

    cout << "m/n\t";
    for (int i : k) cout << "k=" << i << "\t";
    cout << endl;
    for (int i : mn) {
        cout << i << "\t";
        for (int j : k) {
            BFilter bf(n, n * i, j);
            if (j > i) break;

            bf.insert(0, n - 1);
            double result = bf.faultRate(n * 10, n * 11 - 1);
            cout << result << "\t";
        }
        cout << endl;
    }

    return 0;
}
