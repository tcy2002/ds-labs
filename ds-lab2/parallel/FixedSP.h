#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stack>
#include "ThreadPool.h"
#include "windows.h"

using namespace std;

#define INF 1e8

class FixedSP
{
private:
    double total1{0};
    double total2{0};

    vector<vector<int>> mat;
    ThreadPool pool;
    size_t size;
    void getShortestDistance(vector<int> *path, int *path_len, int det, int ter);

public:
    explicit FixedSP(vector<vector<int>> &matrix, uint32_t pool_size): pool(pool_size),
    size(matrix.size()), mat(matrix) {}
    ~FixedSP() {
        cout << "pre-com: " << total1 << "ms" << endl;
        cout << "select: " << total2 << "ms" << endl;
    }

    vector<int> getFixedPointShortestPath(int source, vector<int> intermediates);
};
