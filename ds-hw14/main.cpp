#include "kruskal.h"
#include <windows.h>


int main() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    double total;

    int size[] = {1000, 2000, 5000, 10000};
    int multi[] = {10, 20, 40};
    char path[30];

    for (auto &it : size) {
        for (auto &i : multi) {
            sprintf(path, "../input/insert_%d_%d.txt", it, i);

//                kruskalSequential k(path);
            kruskalParallel k(path, 8);

            QueryPerformanceCounter(&begin_time);
            auto res = k.generate();
            QueryPerformanceCounter(&end_time);
            total = (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;
            std::cout << "vertexes: " << it << " edges: " << i * it << " time: " << total << "ms" << std::endl;

//            if (it == 1000 && i == 10)
//                for (auto &edge : res)
//                    std::cout << edge.u << "\t" << edge.v << "\t" << edge.w << std::endl;
        }
    }

    return 0;
}
