#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include <iostream>
#include <vector>

std::mutex g_mtx;
std::condition_variable g_cv;
std::queue<int> g_queue;
std::atomic<int> g_producers_active{0}; // 活跃生产者计数

void producer(int id, int count)
{
    g_producers_active.fetch_add(1); // 注册生产者
    for (int i = 0; i < count; i++)
    {
        {
            std::unique_lock<std::mutex> lock(g_mtx);
            g_queue.push(i + id * 100);
            std::cout << "Producer " << id << " pushed " << (i + id * 100) << std::endl;
        }
        g_cv.notify_one(); // 通知消费者
    }
    g_producers_active.fetch_sub(1); // 生产者完成
    g_cv.notify_all();               // 通知所有等待的消费者检查队列
}

void consumer(int id)
{
    while (true)
    {
        int value = 0;
        std::unique_lock<std::mutex> lock(g_mtx);
        g_cv.wait(lock, []()
                  { return !g_queue.empty() || g_producers_active.load() == 0; });
        if (g_queue.empty() && g_producers_active.load() == 0)
        {
            break; // 所有生产者结束且队列为空
        }
        if (!g_queue.empty())
        {
            value = g_queue.front();
            g_queue.pop();
            std::cout << "Consumer " << id << " popped " << value << std::endl;
        }
    }
}

int main()
{
    const int producer_count = 3;
    const int consumer_count = 2;
    const int items_per_producer = 5;

    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;

    // 启动生产者
    for (int i = 0; i < producer_count; ++i)
    {
        producers.emplace_back(producer, i + 1, items_per_producer);
    }

    // 启动消费者
    for (int i = 0; i < consumer_count; ++i)
    {
        consumers.emplace_back(consumer, i + 1);
    }

    for (auto &p : producers)
        p.join();
    for (auto &c : consumers)
        c.join();

    std::cout << "Final size: " << g_queue.size() << std::endl;
    return 0;
}