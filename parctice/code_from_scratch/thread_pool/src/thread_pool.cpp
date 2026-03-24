#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool
{
public:
    ThreadPool(size_t n) : stopFlag(false)
    {
        for (size_t i = 0; i < n; ++i)
        {
            workers.emplace_back([this]()
                                 {
                while (true) {
                    std::function<void()> task;
                    {   // 作用域内加锁
                        std::unique_lock<std::mutex> lock(this->mtx);
                        cv.wait(lock, [this]{ return stopFlag || !tasks.empty(); });
                        if (stopFlag && tasks.empty()){
                            break;
                        }
                            
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task(); // 执行任务
                } });
        }
    }

    ~ThreadPool()
    {
        { // 停止线程池
            std::unique_lock<std::mutex> lock(mtx);
            stopFlag = true;
        }
        cv.notify_all();
        for (std::thread &t : workers)
            t.join();
    }

    void enqueue(std::function<void()> task)
    {
        { // 加锁推任务
            std::unique_lock<std::mutex> lock(mtx);
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool stopFlag;
};

// 使用示例
int main()
{
    ThreadPool pool(4); // 4 个工作线程
    for (int i = 0; i < 8; ++i)
    {
        pool.enqueue([i]
                     { std::cout << "Task " << i << " running in thread "
                                 << std::this_thread::get_id() << std::endl; });
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}