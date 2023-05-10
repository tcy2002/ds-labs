#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <atomic>
#include <functional>
#include <windows.h>

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
            cv.wait(lock, [this]{ return !init.load() || !tasks.empty(); });
            if (!init.load()) break;

            Task task{std::move(tasks.front())};
            tasks.pop();
            lock.unlock();

            task();
        }
    }

public:
    explicit ThreadPool(size_t pool_size=0): init(false),
    size(pool_size > 0 ? pool_size : std::thread::hardware_concurrency()) {std::cout << size << std::endl;}

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
    void push(Function fn, Args&... args) {
        if(!init.load()) throw std::runtime_error("task executor have closed commit.");

        std::lock_guard<std::mutex> lock{mtx};
        tasks.emplace(std::bind(fn, std::ref(args)...));

        cv.notify_all();
    }
};

template <class Iterator, class Function>
Function for_each(Iterator first, Iterator last, Function fn, int N) {
    ThreadPool pool(N);

    pool.start();
    while (first != last) {
        pool.push(fn, *first);
        ++first;
    }
    pool.stop();

    return std::move(fn);
}

void print(std::pair<int, int> &a) {
    a.first -= 100;
    a.second += 100;
    Sleep(10);
}

int main() {
    LARGE_INTEGER freq_;
    QueryPerformanceFrequency(&freq_);
    LARGE_INTEGER begin_time;
    LARGE_INTEGER end_time;
    double total;

    std::vector<std::pair<int, int>> a;
    for (int i = 0; i < 100; i++)
        a.emplace_back(i, i);

    QueryPerformanceCounter(&begin_time);
    for_each(a.begin(), a.end(), print, 8);
    QueryPerformanceCounter(&end_time);
    total += (double)(end_time.QuadPart - begin_time.QuadPart) * 1000.0 / (double)freq_.QuadPart;
    std::cout << "total time: " << total << "ms" << std::endl;

//    for (auto &it : a)
//        std::cout << it.first << " " << it.second << std::endl;

    return 0;
}
