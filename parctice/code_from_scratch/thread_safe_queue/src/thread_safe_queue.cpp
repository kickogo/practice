#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional> // C++17及以上，用于返回“可能为空的出队结果”
#include <utility>  // std::move

template <typename T>
class ThreadSafeQueue {
public:
  ThreadSafeQueue() = default;
  ~ThreadSafeQueue() = default;

  // 禁止拷贝（队列所有权独占）
  ThreadSafeQueue(const ThreadSafeQueue&) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

  // 支持移动（可选，提升性能）
  ThreadSafeQueue(ThreadSafeQueue&& other) noexcept {
    std::lock_guard<std::mutex> lock(other.mtx_);
    queue_ = std::move(other.queue_);
  }

  ThreadSafeQueue& operator=(ThreadSafeQueue&& other) noexcept {
    if (this != &other) {
      // 双重锁：先锁自己，再锁对方（避免死锁）
      std::lock_guard<std::mutex> lock1(mtx_, std::adopt_lock);
      std::lock_guard<std::mutex> lock2(other.mtx_, std::adopt_lock);
      queue_ = std::move(other.queue_);
    }
    return *this;
  }

  /**
   * 入队：线程安全，支持右值引用（避免拷贝）
   * @param value 要入队的元素
   */
  void push(T value) {
    std::lock_guard<std::mutex> lock(mtx_); // 自动加锁/解锁
    queue_.push(std::move(value)); // 移动语义提升性能
    cv_.notify_one(); // 唤醒一个等待出队的线程
  }

  /**
   * 出队（非阻塞）：队列为空时返回std::nullopt
   * @return 可选的队列元素（std::optional<T>）
   */
  std::optional<T> try_pop() {
    std::lock_guard<std::mutex> lock(mtx_);
    if (queue_.empty()) {
      return std::nullopt; // 队列为空，返回空值
    }
    T value = std::move(queue_.front()); // 移动元素
    queue_.pop();
    return value;
  }

  /**
   * 出队（阻塞）：队列为空时阻塞，直到有元素入队
   * @return 队列头部元素
   */
  T pop() {
    std::unique_lock<std::mutex> lock(mtx_); // 可解锁的锁（配合条件变量）
    // 等待直到队列非空（防止虚假唤醒，用lambda判空）
    cv_.wait(lock, [this]() { return !queue_.empty(); });
    
    T value = std::move(queue_.front());
    queue_.pop();
    return value;
  }

  /**
   * 判空：线程安全
   * @return 队列是否为空
   */
  bool empty() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.empty();
  }

  /**
   * 获取队列大小：线程安全
   * @return 队列元素个数
   */
  size_t size() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return queue_.size();
  }

private:
  mutable std::mutex mtx_; // mutable：const成员函数可加锁
  std::condition_variable cv_; // 条件变量：用于阻塞等待元素
  std::queue<T> queue_; // 底层存储容器
};

// 测试示例
#include <iostream>
#include <thread>
#include <vector>

int main() {
  ThreadSafeQueue<int> ts_queue;

  // 生产者线程：入队1-10
  std::thread producer([&]() {
    for (int i = 1; i <= 10; ++i) {
      ts_queue.push(i);
      std::cout << "Produced: " << i << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
  });

  // 消费者线程：出队所有元素
  std::thread consumer([&]() {
    for (int i = 1; i <= 10; ++i) {
      int value = ts_queue.pop();
      std::cout << "Consumed: " << value << std::endl;
    }
  });

  producer.join();
  consumer.join();

  return 0;
}