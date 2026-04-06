#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

std::mutex g_mtx;
std::condition_variable g_cv;
std::queue<int> g_queue;
std::atomic<bool> g_done{false};

void producer(int id, int count) {
  for (int i = 0; i < count; i++) {

    std::unique_lock<std::mutex> lock(g_mtx);
    g_queue.push(i + id * 100);
    std::cout << "Producer " << id << " pushed " << (i + id * 100) << std::endl;
    g_cv.notify_one();
  }
  g_done.store(true);
  g_cv.notify_all();
}

void consumer(int id, int count) {
  int value = 0;
  for (int i = 0; i < count; i++) {
    {
      std::unique_lock<std::mutex> lock(g_mtx);
      g_cv.wait(lock, [&]() { return !g_queue.empty() || g_done.load(); });
      if (g_done.load() && g_queue.empty()) {
        break;
      }
      value = g_queue.front();
      std::cout << "Consumer " << id << " popped " << value << std::endl;
      g_queue.pop();
    }
  }
}

int main() {
  std::thread t1(producer, 1, 10);
  std::thread t3(consumer, 1, 10);
  std::thread t4(consumer, 2, 10);
  t1.join();
  t3.join();
  t4.join();
  std::cout << "Final size: " << g_queue.size() << std::endl;
  return 0;
}
