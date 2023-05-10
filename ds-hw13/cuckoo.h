#pragma once

#include <iostream>

class cuckooHash {
private:
    struct pair {
        long long key;
        long long val;
        pair(): key(-1), val(0) {}
        pair(long long key, long long val): key(key), val(val) {}
        ~pair() = default;
        bool isAvailable() const { return key == -1; }
        void makeEmpty() { key = -1; }
    };

    pair *tab[2]{};
    long long size;

    long long H1(long long key) const { return key % size; }
    long long H2(long long key) const { return key / size; }

    void insert(long long key, long long val);
    void insert(long long key, long long val, int idx, int depth);
    void resize();

public:
    explicit cuckooHash(long long total): size(total / 2) {
        tab[0] = new pair[size]{};
        tab[1] = new pair[size + total % 2]{};
    }
    ~cuckooHash() {
        delete []tab[0];
        delete []tab[1];
    }

    void Insert(long long key, long long val) { insert(key, val); }
    long long Lookup(long long key);
    void Delete(long long key);
};
