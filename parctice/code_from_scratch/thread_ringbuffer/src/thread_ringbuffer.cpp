#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>

template <typename T>
class RingBuffer
{
public:
    RingBuffer(size_t capacity) : capacity_(capacity + 1), head_(0), tail_(0)
    {
        buffer_.resize(capacity_);
    }
    ~RingBuffer() = default;
    RingBuffer(const RingBuffer& other) = delete;
    RingBuffer &operator=(const RingBuffer &other) = delete;
    RingBuffer(RingBuffer &&other) = delete;
    RingBuffer &operator=(RingBuffer &&other) = delete;

    void Push(const T &value) noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_not_full_.wait(lock, [this]()
                            { return (tail_ + 1) % capacity_ != head_; });

        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % capacity_;
        cond_not_empty_.notify_one(); // 通知消费者
    }

    T Pop() noexcept
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_not_empty_.wait(lock, [this]
                             { return tail_ != head_; });

        T value = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        cond_not_full_.notify_one(); // 通知生产者
        return value;
    }

    size_t GetSize()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        return (tail_ - head_ + capacity_) % capacity_;
    }

    size_t GetCapacity() const
    {
        return capacity_ - 1;
    }

private:
    std::vector<T> buffer_;
    size_t head_ = 0;
    size_t tail_ = 0;
    size_t capacity_ = 0;
    std::mutex mutex_;
    std::condition_variable cond_not_full_;
    std::condition_variable cond_not_empty_;
    // size_t 是C++中表示"大小/索引"的标准类型，无符号、可表示范围大、语义明确，比 int 更适合描述环形缓冲区的位置和容量
};

// 全局环形缓冲区
RingBuffer<int> rb(5);

// 生产者线程
void producer(int id, int count)
{
    for (int i = 0; i < count; i++)
    {
        int value = i + id * 100;
        rb.Push(value);
        {
            static std::mutex cout_mutex;
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Producer " << id << " pushed " << value << std::endl;
        }
    }
}

// 消费者线程
void consumer(int id, int count)
{
    for (int i = 0; i < count; i++)
    {
        int value = rb.Pop();
        {
            static std::mutex cout_mutex;
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Consumer " << id << " popped " << value << std::endl;
        }
    }
}

int main()
{
    int items_per_producer = 10;

    std::thread t1(producer, 1, items_per_producer);
    std::thread t2(producer, 2, items_per_producer);
    std::thread t3(consumer, 1, items_per_producer);
    std::thread t4(consumer, 2, items_per_producer);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Final size: " << rb.GetSize() << std::endl;
    return 0;
}