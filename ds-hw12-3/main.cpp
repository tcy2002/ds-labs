#include <iostream>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <condition_variable>
#include <functional>
#include <windows.h>

class MyThreadPool {
private:
    using Task = std::function<void()>;
    uint32_t size;
    std::vector<std::thread*> pool;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;

    void thread_wrapper() {
        while(true) {
            std::unique_lock<std::mutex> lock(mtx);
            while (tasks.empty()) {
                if (stop) return;
                cv.wait(lock);
            }
            Task task(std::move(tasks.front()));
            tasks.pop();
            lock.unlock();

            task();
        }
    }

public:
    explicit MyThreadPool(size_t pool_size = 0):
            stop(false),
            size(pool_size > 0 ? pool_size : std::thread::hardware_concurrency()) {
        std::cout << "pool size: " << size << std::endl;
        for (int i = 0; i < size; i++) {
            pool.push_back(new std::thread(&MyThreadPool::thread_wrapper, this));
        }
    }
    ~MyThreadPool() {
        join();
        stop = true;
        cv.notify_all();
        for (auto th : pool) {
            if (th->joinable())
                th->join();
            delete th;
        }
    }

    template<typename Function, typename... Args>
    void addTask(Function fn, Args&&... args) {
        mtx.lock();
        tasks.emplace([fn, &args...]{ fn(args...); });
        mtx.unlock();
        cv.notify_one();
    }

    template <typename Iterator, typename Function>
    void forEach(Iterator first, Iterator last, Function fn) {
        mtx.lock();
        while (first != last) {
            tasks.emplace([fn, &first]{ fn(*first); });
            ++first;
        }
        mtx.unlock();
        cv.notify_all();
    }

    void join() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            if (tasks.empty()) break;
        }
    }
};

void exec(uint64_t& t) {
    std::default_random_engine e(GetTickCount());
    std::uniform_int_distribution<int> u(500, 1000);
    auto mat = new uint64_t[100];

    for (int i = 0; i < 100; i++) {
        mat[i] = u(e) % 111;
    }
    for (int i = 0; i < u(e); i++) {
        for (int j = 0; j < 100; j++) {
            for(int k = 0; k < 100; k++) {
                mat[j] = mat[j] * mat[k];
            }
        }
    }

    delete[] mat;
}

int main() {
    DWORD start, time1, time2;

    MyThreadPool pool(16);
    int vec_size = 256;
    auto vec = new uint64_t[vec_size]{};
    std::cout << "start test" << std::endl;

    // sequential time
    start = GetTickCount();
    for (int i = 0; i < vec_size; i++) {
        exec(vec[i]);
    }
    time1 = GetTickCount() - start;
    std::cout << "sequential time: " << time1 << "ms" << std::endl;

    // parallel time
    start = GetTickCount();
    pool.forEach(&vec[0], &vec[vec_size - 1], exec);
    pool.join();
    time2 = GetTickCount() - start;
    std::cout << "parallel time: " << time2 << "ms" << std::endl;

    // parallel ratio
    std::cout << "parallel ratio: " << 1.0 * time1 / time2 << std::endl;

    return 0;
}
