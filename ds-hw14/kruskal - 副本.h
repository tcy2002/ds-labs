#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <windows.h>
#include "ThreadPool.h"

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
    struct edge {
        int u;
        int v;
        int w;
        int tid;
        edge *next;
        edge(int u, int v, int w, edge *next=nullptr):
        u(u), v(v), w(w), next(next), tid(-1) {}
    };
    struct vertex {
        int vid;
        edge *next;
        explicit vertex(int v): vid(v), next(nullptr) {}
    };
    std::vector<vertex> graph;
    int thread_num;
    std::multimap<int, edge> edge_queue;
    std::mutex mtx;
    std::condition_variable *cv;
    std::condition_variable cv_main;
    bool isFinished;
    int init;
    int hasInited;
    std::vector<edge> mst;
    std::vector<std::thread> pool;
    std::atomic<bool> *prepared;

    void partition() {
        std::vector<vertex> tmp[thread_num];
        size_t size = graph.size();
        int i;
        for (i = 0; i < size; i++)
            tmp[i % thread_num].push_back(graph[i]);
        for (i = 0; i < thread_num; i++)
            init += (!tmp[i].empty());
        for (i = 0; i < thread_num && !tmp[i].empty(); i++)
            pool.emplace_back(&kruskalParallel::thread_wrapper, this, tmp[i], i);
    }

    /******************子进程算法**********************/
    void send_edge(std::multimap<int, edge> &m) {
        if (!m.empty()) {
            edge_queue.insert(*(m.begin()));
            m.erase(m.begin());
        }
    }

    void thread_wrapper(std::vector<vertex> v, int tid) {
        std::multimap<int, edge> e;
        size_t size = v.size();

        for (int i = 0; i < size; i++) {
            auto tmp = v[i].next;
            while (tmp != nullptr) {
                tmp->tid = tid;
                e.insert(std::make_pair(tmp->w, *tmp));
                tmp = tmp->next;
            }
        }

        std::unique_lock<std::mutex> lock(mtx);
        send_edge(e);
        hasInited++;
        lock.unlock();

        while (true) {
            std::unique_lock<std::mutex> lk(mtx);
            cv[tid].wait(lk);
            if (isFinished) break;
            send_edge(e);
            prepared[tid].store(true);
            cv_main.notify_all();
        }
    }

    /*******************主进程算法***********************/
    void add_edge(edge e, std::map<int, int> &i, std::map<int, std::vector<int>> &rev_i) {
        if (!std::any_of(mst.begin(), mst.end(),
                         [&e](auto &it) { return (e.u == it.u && e.v == it.v) || (e.u == it.v && e.v == it.u); }))
            mst.push_back(e);
        else return;

        int cid1 = i[e.u], cid2 = i[e.v];
        size_t size1{rev_i[cid1].size()}, size2{rev_i[cid2].size()};

        if (cid1 == -1 && cid2 == -1) {
            int cid = (int)rev_i.size() + 1;
            std::vector<int> tmp;
            tmp.push_back(e.u);
            tmp.push_back(e.v);
            rev_i[cid] = tmp;
            i[e.u] = cid;
            i[e.v] = cid;
        } else if (cid1 == -1) {
            i[e.u] = cid2;
            rev_i[cid2].push_back(e.u);
        } else if (cid2 == -1) {
            i[e.v] = cid1;
            rev_i[cid1].push_back(e.v);
        } else if (size1 <= size2) {
            for (int k = 0; k < size1; k++) {
                i[rev_i[cid1][k]] = cid2;
                rev_i[cid2].push_back(rev_i[cid1][k]);
            }
            rev_i.erase(cid1);
        } else {
            for (int k = 0; k < size2; k++) {
                i[rev_i[cid2][k]] = cid1;
                rev_i[cid1].push_back(rev_i[cid2][k]);
            }
            rev_i.erase(cid2);
        }
    }

public:
    explicit kruskalParallel(const char *path, int thread_num):
    thread_num(thread_num), isFinished(false), init(0), hasInited(0),
    prepared(new std::atomic<bool>[thread_num]{}),
    cv(new std::condition_variable[thread_num]) {
        std::ifstream ifs(path);
        int n, m, i, u, v, w;
        ifs >> n >> m;
        for (i = 0; i < n; i++)
            graph.emplace_back(i);
        for (i = 0; i < m; i++) {
            ifs >> u >> v >> w;
            graph[u].next = new edge(u, v, w, graph[u].next);
            graph[v].next = new edge(u, v, w, graph[v].next);
        }
        ifs.close();
    }
    ~kruskalParallel() { delete []cv; }

    std::vector<edge> generate() {
        partition();
        std::map<int, int> index;
        std::map<int, std::vector<int>> rev_index;
        size_t size = graph.size();
        for (int i = 0; i < size; i++) index[i] = -1;

        while (hasInited < init);
        while (mst.size() < size - 1) {
            if (edge_queue.empty()) break;
            std::pair<int, edge> tmp = *(edge_queue.begin());
            edge_queue.erase(edge_queue.begin());

            int tid = tmp.second.tid;
            cv[tid].notify_all();
            std::unique_lock<std::mutex> lock(mtx);
            cv_main.wait(lock, [this, &tid]() { return prepared[tid].load(); });
            prepared[tid].store(false);

            if (index[tmp.second.u] != index[tmp.second.v] || index[tmp.second.u] == -1)
                add_edge(tmp.second, index, rev_index);
        }

        isFinished = true;

        for (int i = 0; i < thread_num; i++) {
            cv[i].notify_all();
            pool[i].join();
        }

        return mst;
    }
};