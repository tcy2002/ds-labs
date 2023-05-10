#include <vector>
#include <string>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <stack>
#include <windows.h>

using namespace std;

#define INF 1e8

class FixedSP
{
private:
    double total1{0};
    double total2{0};

    vector<vector<int>> mat;
    size_t size;
    int getShortestDistance(vector<int> &path, int det, int ter);

public:
    explicit FixedSP(vector<vector<int>> &matrix): mat(matrix), size(matrix.size()) {}
    ~FixedSP() {
        cout << "calculation: " << total1 << "ms" << endl;
        cout << "permutation: " << total2 << "ms" << endl;
    }

    vector<int> getFixedPointShortestPath(int source, vector<int> intermediates);
};
