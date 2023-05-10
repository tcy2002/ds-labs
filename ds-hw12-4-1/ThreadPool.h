#pragma once

#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

class ThreadPool {
private:
    uint32_t init;
    uint32_t pool_size;
    uint32_t on_working;
    std::mutex mtx;
    std::vector<std::thread> working_buffer;

    void increase() {
        mtx.lock();
        on_working++;
        mtx.unlock();
    }

    void decrease() {
        mtx.lock();
        on_working--;
        mtx.unlock();
    }

    template <class Function, class... Arg>
    void thread_wrapper(Function fn, Arg... x) {
        increase();
        fn(x...);
        decrease();
    }

public:
    explicit ThreadPool(int size): init(0), on_working(0) {
        uint32_t max_con = std::thread::hardware_concurrency();
        pool_size = (size > 0 && size <= max_con) ? size : max_con;
    }
    ~ThreadPool() { if (init) stop(); }

    void start() { init = 1; }

    bool isFull() const { return on_working == pool_size; }

    template <class Function, class... Arg>
    std::thread *add(Function fn, Arg... x) {
        try {
            if (init) {
                while (on_working == pool_size);
                working_buffer.emplace_back(&ThreadPool::thread_wrapper<Function, Arg...>, this, fn, x...);
                return &working_buffer.back();
            } else throw std::runtime_error("ThreadPool not initialized");
        } catch (std::exception &e) {
            std::cout << "error: " << e.what() << std::endl;
            exit(-1);
        }
    }

    void stop() {
        if (init) {
            for (auto &th : working_buffer) {
                th.join();
            }
            init = 0;
        }
    }
};