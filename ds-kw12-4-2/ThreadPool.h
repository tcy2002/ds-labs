#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <atomic>

typedef std::function<void ()> Task;

class ThreadPool {
private:
    std::vector<std::thread> pool;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv_task;
    int init;

    Task get_task(){
        std::unique_lock<std::mutex> lock{mtx};
        cv_task.wait(lock, [this](){ return !tasks.empty(); });
        Task task {std::move(tasks.front())};
        tasks.pop();
        return task;
    }

    void thread_wrapper() {
        while (init) { if(Task task = get_task()) task(); }
    }

public:
    explicit ThreadPool(size_t size = 4): init(0) {
        size = size < 1 ? 1 : size;
        for(size_t i = 0; i< size; ++i){
            pool.emplace_back(&ThreadPool::thread_wrapper, this);
        }
    }

    ~ThreadPool() {
        cv_task.notify_all();
        for(std::thread& th: pool)
            th.join();
    }

    void stop() {
        while (!tasks.empty()) ;
        init = 0;
    }

    void start() { init = 1; }

    template<class Function, class... Args>
    void add(Function fn, Args... args) {
        if(!init)
            throw std::runtime_error("committing is banned");

        std::lock_guard<std::mutex> lock{mtx};
        tasks.template emplace(std::bind(fn, args...));
        cv_task.notify_all();
    }
};