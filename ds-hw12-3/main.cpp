#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <functional>
#include <windows.h>

class ThreadPool {
private:
    using Task = std::function<void()>;
    uint32_t size;
    std::vector<std::thread> pool;
    std::queue<Task> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stop;

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
    explicit ThreadPool(size_t pool_size = 0):
        stop(false),
        size(pool_size > 0 ? pool_size : std::thread::hardware_concurrency()) {
        std::cout << "pool size: " << size << std::endl;
        for (int i = 0; i < size; i++) {
            pool.emplace_back(&ThreadPool::thread_wrapper, this);
        }
    }
    ~ThreadPool() {
        join();
        stop = true;
        cv.notify_all();
        for (auto& th : pool) {
            if (th.joinable())
                th.join();
        }
    }

    template<typename Function, typename... Args>
    void addTask(Function&& fn, Args&&... args) {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.emplace([fn, &args...]{ fn(args...); });
        cv.notify_one();
    }

    template <typename Iterator, typename Function>
    void forEach(Iterator first, Iterator last, Function&& fn) {
        while (first != last) {
            addTask(fn, *first);
            ++first;
        }
    }

    void join() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            if (tasks.empty()) break;
        }
    }
};

class Test {
public:
    int _val;
    Test(): _val(0) { std::cout << "default constructor" << std::endl; }
    explicit Test(int val): _val(val) { std::cout << "constructor" << std::endl; }
    Test(const Test& t): _val(t._val) { std::cout << "copy constructor" << std::endl; }
    Test(Test&& t) noexcept: _val(t._val) { std::cout << "move constructor" << std::endl; }
};

void exec(Test& t) {
    t._val++;
    Sleep(10);
}

int main() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;

    // to initialize the vector and thread pool
    ThreadPool pool;
    std::vector<Test> vec;
    vec.reserve(100);
    for (int i = 0; i < 100; i++) {
        vec.emplace_back(i);
    }
    std::cout << "init finished" << std::endl;

    // sequential time
    QueryPerformanceCounter(&begin_time);
    for (auto& i : vec) {
        exec(i);
    }
    QueryPerformanceCounter(&end_time);
    auto time1 = (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;
    std::cout << "sequential time: " << time1 << "ms" << std::endl;

    // parallel time
    QueryPerformanceCounter(&begin_time);
    pool.forEach(vec.begin(), vec.end(), exec);
    pool.join();
    QueryPerformanceCounter(&end_time);
    auto time2 = (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;
    std::cout << "parallel time: " << time2 << "ms" << std::endl;

    // parallel ratio
    std::cout << "parallel ratio: " << time1 / time2 << std::endl;

    // to print and check the executed vector
    int sum = 0;
    for (auto& i : vec) {
        sum += i._val;
    }
    std::cout << "sum: " << sum << std::endl;

    return 0;
}
