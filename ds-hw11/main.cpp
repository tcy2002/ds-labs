#include <iostream>
#include <cstring>
#include <fstream>
#include <windows.h>

#define MAX_BUF 1024
int *buildNext(const char *p, size_t n_p) {
    size_t j = 0;
    int *n = new int[n_p];
    int t = n[0] = -1;
    while (j < n_p - 1) {
        if (t < 0 || p[j] == p[t])
            n[++j] = ++t;
        else
            t = n[t];
    }
    return n;
}

int kmp(const char *pattern, const char *text, size_t n_p, size_t n_t) {
    int *next = buildNext(pattern, n_p);
    int i = 0, j = 0, n = 0;

    while (i < n_t) {
        if (text[i] == pattern[j]) {
            i++;
            (j < n_p - 1) ? ++j : (++n && (j = 0));
        } else {
            j = next[j];
            if (j == -1) {
                i++;
                j++;
            }
        }
    }

    delete []next;
    return n;
}

int normal(const char *pattern, const char *text, size_t n_p, size_t n_t) {
    int i = 0, j = 0, n = 0;

    while (i < n_t) {
        if (text[i] == pattern[j]) {
            i++;
            (j < n_p - 1) ? ++j : (++n && (j = 0));
        } else {
            i -= (j - 1);
            j = 0;
        }
    }

    return n;
}

int main() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    double time1 = 0, time2 = 0;
    int m, n;
    int dup = 100;

    std::ifstream text("../text.txt", std::ios::binary);
    char buf[MAX_BUF];
    size_t n_t = text.rdbuf()->sgetn (buf,MAX_BUF);
    text.close();
    char **pattern = new char*[5];
    pattern[0] = new char[2];
    strcpy(pattern[0], "1");
    pattern[1] = new char[6];
    strcpy(pattern[1], "11111");
    pattern[2] = new char[11];
    strcpy(pattern[2], "1111111111");
    pattern[3] = new char[16];
    strcpy(pattern[3], "111111111111111");
    pattern[4] = new char[21];
    strcpy(pattern[4], "11111111111111111111");
    pattern[5] = new char[26];
    strcpy(pattern[5], "1111111111111111111111111");


    for (int i = 0; i < 6; i++) {
        size_t n_p = strlen(pattern[i]);

        for (int k = 0; k < dup; k++) {
            QueryPerformanceCounter(&begin_time);
            m = kmp(pattern[i], buf, n_p, n_t);
            QueryPerformanceCounter(&end_time);
            time1 += (double) (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double) freq_.QuadPart;
        }

        for (int k = 0; k < dup; k++) {
            QueryPerformanceCounter(&begin_time);
            n = normal(pattern[i], buf, n_p, n_t);
            QueryPerformanceCounter(&end_time);
            time2 += (double) (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / (double) freq_.QuadPart;
        }

        std::cout << "text size: " << n_t << " bytes" << std::endl;
        std::cout << "pattern: " << pattern[i] << std::endl;
        std::cout << "kmp: " << m << " time: " << time1 / dup << "us" << std::endl;
        std::cout << "normal: " << n << " time: " << time2 / dup << "us" << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
