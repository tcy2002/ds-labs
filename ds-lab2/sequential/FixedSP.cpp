#include "FixedSP.h"

vector<int> FixedSP::getFixedPointShortestPath(int source, vector<int> intermediates)
{
    vector<int> path;
    
    // TODO
    LARGE_INTEGER freq_;
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    QueryPerformanceFrequency(&freq_);

    if (intermediates.empty()) {
        path.push_back(source);
        return path;
    }

    size_t len = intermediates.size();
    vector<int> distance_to_source(len);
    vector<vector<int>> path_to_source(len, vector<int>());
    vector<vector<int>> distances(len, vector<int>(len));
    vector<vector<vector<int>>> paths(len, vector<vector<int>>(len));

    QueryPerformanceCounter(&begin_time);
    int i, j;
    for (i = 0; i < len; i++) {
        if ((distance_to_source[i] =
                getShortestDistance(path_to_source[i], source, intermediates[i])
                ) > INF - 1) {
            return path;
        }
        reverse(path_to_source[i].begin(), path_to_source[i].end());
    }

    for (i = 0; i < len - 1; i++) {
        for (j = i + 1; j < len; j++) {
            if ((distances[j][i] = distances[i][j] =
                    getShortestDistance(paths[i][j], intermediates[i], intermediates[j])
                    ) > INF - 1) {
                return path;
            }
            paths[j][i] = paths[i][j];
            reverse(paths[i][j].begin(), paths[i][j].end());
        }
    }
    QueryPerformanceCounter(&end_time);
    total1 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;

    vector<int> permutation(len);
    for (i = 0; i < len; i++) permutation[i] = i;
    int path_len, min_path_len = INF + 1;

    QueryPerformanceCounter(&begin_time);
    vector<int> tmp;
    do {
        path_len = distance_to_source[permutation[0]];
        for (i = 0; i < len - 1; i++)
            path_len += distances[permutation[i]][permutation[i + 1]];
        path_len += distance_to_source[permutation.back()];
        if (path_len < min_path_len) {
            min_path_len = path_len;
            tmp = permutation;
        }
    } while (next_permutation(permutation.begin(), permutation.end()));

    std::vector<int> *p = &path_to_source[tmp[0]];
    for (auto &it : *p)
        path.push_back(it);
    for (i = 0; i < len - 1; i++) {
        p = &paths[tmp[i]][tmp[i + 1]];
        auto it = p->begin();
        while (++it != p->end())
            path.push_back(*it);
    }
    p = &path_to_source[tmp.back()];
    len = p->size();
    for (i = (int)len - 2; i >= 0; i--)
        path.push_back((*p)[i]);
    QueryPerformanceCounter(&end_time);
    total2 += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;

    return path;
}

int FixedSP::getShortestDistance(vector<int> &path, int det, int ter) {
    if (det == ter) return 0;

    vector<pair<int, int>> S(1, {det, 0});
    vector<pair<int, int>> path_tmp(1, {det, -1});
    vector<int> T;
    for (int i = 0; i < size; i++) {
        if (i == det) continue;
        T.push_back(i);
    }

    int target = det, distance, i, j, idx, x, y, distance_tmp, min_distance, parent;
    size_t SSize = 1, TSize = size - 1;

    while (true)  {
        if (target == ter || TSize == 0) break;

        min_distance = INF + 1;
        for (i = 0; i < SSize; i++) {
            x = S[i].first;
            distance_tmp = S[i].second;
            for (j = 0; j < TSize; j++) {
                y = T[j];
                distance = distance_tmp + mat[x][y];
                if (distance <= min_distance) {
                    min_distance = distance;
                    parent = x;
                    idx = j;
                }
            }
        }

        target = T[idx];
        S.emplace_back(target, min_distance);
        path_tmp.emplace_back(target, parent);
        T.erase(T.begin() + idx);
        SSize++;
        TSize--;
    }

    auto it = path_tmp.end() - 1;
    int tmp;
    while (true) {
        tmp = it->second;
        path.push_back(it->first);
        if (tmp == -1) break;
        while (it->first != tmp) it--;
    }

    return min_distance;
}
