#pragma once

#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <functional>

typedef std::function<void()> Task;

class ThreadPool{
private:
    uint32_t size;
    std::vector<std::thread> pool;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> init;

    void thread_wrapper() {
        while(true) {
            std::unique_lock<std::mutex> lock{mtx};
            cv.wait(lock, [this](){ return !init.load() || !tasks.empty(); });
            if (!init.load()) break;

            Task task{std::move(tasks.front())};
            tasks.pop();
            lock.unlock();

            task();
        }
    }

public:
    explicit ThreadPool(size_t pool_size=0): init(false),
    size(pool_size > 0 ? pool_size : std::thread::hardware_concurrency()) {}

    ~ThreadPool() { if (init.load()) stop(); }

    void start() {
        init.store(true);
        for(size_t i = 0; i < size; ++i)
            pool.emplace_back(&ThreadPool::thread_wrapper, this);
    }

    void stop() {
        while (!tasks.empty());
        init.store(false);

        for (auto &th : pool) {
            cv.notify_all();
            if (th.joinable())
                th.join();
        }
        pool.clear();
    }

    template<class Function, class... Args>
    void push(Function fn, Args... args) {
        if(!init.load()) throw std::runtime_error("task executor have closed commit.");

        std::lock_guard<std::mutex> lock{mtx};
        tasks.emplace(std::bind(fn, args...));

        cv.notify_all();
    }
};
