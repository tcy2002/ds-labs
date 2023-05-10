#include "cuckoo.h"


void cuckooHash::insert(long long key, long long val) {
    long long k1{H1(key)}, k2{H2(key)};
    if (tab[0][k1].isAvailable() || tab[0][k1].key == key) {
        tab[0][k1] = {key, val};
        return;
    }
    if (tab[1][k2].isAvailable() || tab[1][k2].key == key) {
        tab[1][k2] = {key, val};
        return;
    }
    insert(key, val, 0, 0);
}

void cuckooHash::insert(long long key, long long val, int idx, int depth) {
    if (depth >= 2 * size) {
        resize();
        insert(key, val);
        return;
    }

    long long k{idx ? H2(key) : H1(key)};
    long long key_tmp;
    long long val_tmp;

    if (tab[idx][k].isAvailable()) {
        tab[idx][k] = {key, val};
        return;
    }

    key_tmp = tab[idx][k].key;
    val_tmp = tab[idx][k].val;

    tab[idx][k] = {key, val};
    insert(key_tmp, val_tmp, !idx, ++depth);
}

long long cuckooHash::Lookup(long long key) {
    long long k1{H1(key)}, k2{H2(key)};
    if (tab[0][k1].key == key) return tab[0][k1].val;
    else if (tab[1][k2].key == key) return tab[1][k2].val;
    return -1;
}

void cuckooHash::Delete(long long key) {
    long long k1{H1(key)}, k2{H2(key)};
    if (tab[0][k1].key == key) tab[0][k1].makeEmpty();
    else if (tab[1][k2].key == key) tab[1][k2].makeEmpty();
}

void cuckooHash::resize() {
    size *= 2;
    auto tmp0 = tab[0];
    auto tmp1 = tab[1];
    tab[0] = new pair[size]{};
    tab[1] = new pair[size]{};

    int i;
    for (i = 0; i < size / 2; i++)
        if (!tmp0[i].isAvailable())
            insert(tmp0[i].key, tmp0[i].val);
    for (i = 0; i < size / 2; i++)
        if (!tmp1[i].isAvailable())
            insert(tmp1[i].key, tmp1[i].val);
}
