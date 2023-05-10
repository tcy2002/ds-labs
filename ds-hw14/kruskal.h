#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <windows.h>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <thread>

class kruskalSequential {
private:
    struct Edge {
        int u;
        int v;
        int w;
        bool operator<(const Edge &e) const {
            return this->w < e.w;
        }
    };
    std::vector<Edge> edges;
    int n{};

    void makeSet(std::vector<int> &uSet) const {
        uSet.assign(n, 0);
        for (int i = 0; i < n; i++)
            uSet[i] = i;
    }

    static int findSet(std::vector<int> &uSet, int u) {
        int i = u;
        while (uSet[i] != i) i = uSet[i];
        return i;
    }

public:
    explicit kruskalSequential(const char *path) {
        std::ifstream ifs(path);
        int m;
        ifs >> n >> m;
        edges.assign(m, Edge());
        for (int i = 0; i < m; i++)
            ifs >> edges[i].u >> edges[i].v >> edges[i].w;
        std::sort(edges.begin(), edges.end());
        ifs.close();
    }
    ~kruskalSequential() = default;

    std::vector<Edge> generate() {
        std::vector<int> uSet;
        std::vector<Edge> spanTree;
        int cost{}, e1, e2;

        makeSet(uSet);
        size_t size = edges.size();
        for (int i = 0; i < size; i++) {
            e1 = findSet(uSet, edges[i].u);
            e2 = findSet(uSet, edges[i].v);
            if (e1 != e2) {
                spanTree.push_back(edges[i]);
                cost += edges[i].w;
                uSet[e1] = e2;
            }
        }

        return spanTree;
    }
};

class kruskalParallel {
private:
    struct Edge {
        int u;
        int v;
        int w;
        int tid;
        bool operator<(const Edge &e) const {
            return this->w < e.w;
        }
        Edge(int u, int v, int w, int tid): u(u), v(v), w(w), tid(tid) {}
    };
    std::vector<Edge> *edges;
    int n{};
    int thread_num;
    std::vector<Edge> edge_queue;
    std::mutex mtx;
    std::condition_variable *cv;
    std::condition_variable cv_main;
    std::atomic<int> init;
    std::atomic<int> hasInited;
    std::atomic<bool> hasFinished;
    std::atomic<bool> *hasPrepared;
    std::vector<std::thread> pool;

    void makeSet(std::vector<int> &uSet) const {
        uSet.assign(n, 0);
        for (int i = 0; i < n; i++)
            uSet[i] = i;
    }

    static int findSet(std::vector<int> &uSet, int u) {
        int i = u;
        while (uSet[i] != i) i = uSet[i];
        return i;
    }

    void send_edge(int tid) {
        if (!edges[tid].empty()) {
            edge_queue.push_back(edges[tid].front());
            edges[tid].erase(edges[tid].begin());
        }
    }

    static void add_edge(std::vector<int> &uSet, std::vector<Edge> &res, Edge &tmp) {
        int e1, e2;
        e1 = findSet(uSet, tmp.u);
        e2 = findSet(uSet, tmp.v);
        if (e1 != e2) {
            res.push_back(tmp);
            uSet[e1] = e2;
        }
    }

    void thread_wrapper(int tid) {
        std::unique_lock<std::mutex> lock(mtx);
        send_edge(tid);
        hasInited++;
        lock.unlock();

        while (true) {
            std::unique_lock<std::mutex> lk(mtx);
            cv[tid].wait(lk, [this, &tid] { return !hasPrepared[tid].load() || hasFinished.load(); });
            if (hasFinished) return;
            send_edge(tid);
            hasPrepared[tid].store(true);
            cv_main.notify_all();
        }
    }

public:
    explicit kruskalParallel(const char *path, int thread_num):
    cv(new std::condition_variable[thread_num]),
    edges(new std::vector<Edge>[thread_num]),
    init(0), hasInited(0), hasFinished(false),
    hasPrepared(new std::atomic<bool>[thread_num]{}),
    thread_num(thread_num) {
        std::ifstream ifs(path);
        int m, u, v, w, tid, i;
        ifs >> n >> m;

        for (i = 0; i < m; i++) {
            ifs >> u >> v >> w;
            tid = i % thread_num;
            edges[tid].emplace_back(u, v, w, tid);
        }

        for (i = 0; i < thread_num; i++) {
            init += !edges[i].empty();
            std::sort(edges[i].begin(), edges[i].end());
            hasPrepared[i].store(true);
            pool.emplace_back(&kruskalParallel::thread_wrapper, this, i);
        }
        ifs.close();
    }
    ~kruskalParallel() {
        delete []cv;
        delete []edges;
        delete []hasPrepared;
    }

    std::vector<Edge> generate() {
        std::vector<int> uSet;
        std::vector<Edge> spanTree;
        int tid;

        makeSet(uSet);
        while (hasInited < init);
        while (spanTree.size() < n - 1) {
            if (edge_queue.empty()) {
                hasPrepared[tid].store(false);
                hasFinished.store(true);
                break;
            }

            auto tmp = edge_queue.front();
            edge_queue.erase(edge_queue.begin());

            tid = tmp.tid;
            hasPrepared[tid].store(false);
            cv[tid].notify_all();

            add_edge(uSet, spanTree, tmp);

            std::unique_lock<std::mutex> lock(mtx);
            cv_main.wait(lock, [this, &tid] { return hasPrepared[tid].load(); });
        }

        for (int i = 0; i < thread_num; i++) {
            cv[i].notify_all();
            pool[i].join();
        }

        return spanTree;
    }
};