#include <stdexcept>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <memory>
#include <thread>
#include <iostream>
#include <atomic>
#include <unistd.h>

template <typename T>
class RingBuffer
{
public:
    RingBuffer(size_t capacity) : capacity_(capacity + 1), head_(0), tail_(0)
    {
        buffer_.resize(capacity_);
    }
    ~RingBuffer() = default;
    RingBuffer(const RingBuffer &other) = delete;
    RingBuffer &operator=(const RingBuffer &other) = delete;
    RingBuffer(RingBuffer &&other) = delete;
    RingBuffer &operator=(RingBuffer &&other) = delete;

    bool IsEmpty() noexcept
    {

        return head_ == tail_;
    }

    bool IsFull() noexcept
    {

        return (tail_ + 1) % capacity_ == head_;
    }

    bool Push(const T &value) noexcept
    {
        if (!IsFull())
        {
            buffer_[tail_] = value;
            tail_ = (tail_ + 1) % capacity_;
            return true;
        }
        else
        {
            std::cout<<"RingBuffer is full."<<std::endl;
            return false;
        }
    }

    bool Pop(T &value) noexcept
    {
        if (!IsEmpty())
        {
            value = buffer_[head_];
            head_ = (head_ + 1) % capacity_;
            return true;
        }
        else
        {
            std::cout<<"RingBuffer is empty."<<std::endl;
            return false;
        }
    }

    size_t GetSize() const
    {

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
    // size_t 是C++中表示"大小/索引"的标准类型，无符号、可表示范围大、语义明确，比 int 更适合描述环形缓冲区的位置和容量
};

RingBuffer<int> rb(5);
void producer(int id)
{
    for (int i = 0; i < 10; i++)
    {
        rb.Push(i + id * 100);
        std::cout << "Producer " << id << " pushed " << (i + id * 100) << std::endl;
    }
}

void consumer(int id)
{
    int val;
    for (int i = 0; i < 10; i++)
    {
        if (rb.Pop(val))
        {
            std::cout << "Consumer " << id << " popped " << val << std::endl;
        }
    }
}

int main()
{
    /* 单线程功能正常 */
    // // 写入数据
    // for (int i = 1; i <= 6; i++)
    // {
    //     rb.Push(i);
    //     std::cout << "Pushed: " << i << ", size=" << rb.GetSize() << std::endl;
    // }

    // // 读取数据
    // int val;
    // while (rb.Pop(val))
    // {
    //     std::cout << "Popped: " << val << ", size=" << rb.GetSize() << std::endl;
    // }

    /* 多进程场景出现资源竞争 */
    std::thread t1(producer, 1);
    std::thread t2(producer, 2);
    std::thread t3(consumer, 1);
    std::thread t4(consumer, 2);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    std::cout << "capacity: " << rb.GetCapacity() << ", size=" << rb.GetSize() << std::endl;

    return 0;
}