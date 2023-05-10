#include "FixedSP.h"
#include <iostream>
#include <fstream>
#include <windows.h>

void do_test(const string& input_file_path, uint32_t pool_size)
{
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    double total;

    ifstream inputData;
    inputData.open(input_file_path, ios::in);
    if (!inputData)
    {
        cout << "[error]: file " << input_file_path << " not found." << endl;
        inputData.close();
        return;
    }

    string str;
    inputData >> str;
    int node_num = atoi(str.c_str());

    vector<vector<int>> matrix(node_num, vector<int>(node_num));

    for(int i = 0; i < node_num; ++i){
        for(int j = 0; j < node_num; ++j){
            inputData >> str;
            matrix[i][j] = str == "@" ? INF : atoi(str.c_str());
        }
    }

    FixedSP fsp(matrix, pool_size);

    while (inputData >> str)
    {
        int source = atoi(str.c_str());
        vector<int> intermediates{};
        while(true){
            inputData >> str;
            if(str == "$"){
                break;
            }
            intermediates.emplace_back(atoi(str.c_str()));
        }

        QueryPerformanceCounter(&begin_time);
        vector<int> path = fsp.getFixedPointShortestPath(source, intermediates);
        QueryPerformanceCounter(&end_time);
        total = (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;
//        cout << "num: " << intermediates.size() << "\ttime: " << total << "ms" << endl;

        int dis = 0;
        for(int i = 0; i < path.size(); ++i){
            dis += matrix[path[i]][path[(i + 1) % path.size()]];
        }
        if(dis == 0){
            dis = INF;
        }
//        cout << dis << endl;
    }

    inputData.close();
}

int main() {
    do_test("../my_input_500.txt", 4);
    return 0;
}