
#include <atomic>
#include <stdio.h>
template <typename T> class SharedPtr {
public:
  // 构造函数
  explicit SharedPtr(T *ptr = nullptr) : ptr_(ptr) {
    if (ptr != nullptr) {
      refer_count_ = new std::atomic<int>(1);
    } else {
      refer_count_ = nullptr;
    }
  }
  // 拷贝构造函数
  SharedPtr(const SharedPtr &other) {
    ptr_ = other.ptr_;
    refer_count_ = other.refer_count_;
    if (refer_count_ != nullptr) {
      // 增加引用计数仅需原子性，无需顺序约束
      refer_count_->fetch_add(1, std::memory_order_relaxed);
    }
  }

  // 赋值重载
  SharedPtr &operator=(const SharedPtr &other) {
    if (this != &other) {
      T *new_ptr = other.ptr_;
      std::atomic<int> *new_refer = other.refer_count_;
      if (new_refer != nullptr) {
        new_refer->fetch_add(1, std::memory_order_relaxed);
      }
      //释放资源
      Release();
      //共享新资源
      ptr_ = new_ptr;
      refer_count_ = new_refer;
    }
    return *this;
  }

  SharedPtr(SharedPtr &&other) noexcept{
    ptr_ = other.ptr_;
    refer_count_ = other.refer_count_;
    other.ptr_ = nullptr;
    other.refer_count_ = nullptr;
  }

  SharedPtr &operator=(SharedPtr &&other) noexcept{
    if (this != &other) {
      Release();
      ptr_ = other.ptr_;
      refer_count_ = other.refer_count_;
      other.ptr_ = nullptr;
      other.refer_count_ = nullptr;
    }
    return *this;
  }

  //析构函数
  ~SharedPtr() { Release(); }

  // 指针操作
  // sp->func()：让盒子 “露出” 里面的指针，用指针访问成员；
  T *operator->() const {
    if (!ptr_) {
      fprintf(stderr, "Error: SharedPtr is null, cannot use operator->\n");
      return nullptr;
    }
    return ptr_;
  }
  // *sp：直接打开盒子，拿到里面的对象本身。
  T &operator*() const {
    if (!ptr_) {
      fprintf(stderr, "Error: SharedPtr is null, cannot use operator*\n");
      return nullptr;
    }
    return *ptr_;
  }

  T *GetRawPtr() const {
    if (!ptr_) {
      fprintf(stderr, "Error: SharedPtr is null, cannot use operator->\n");
      return nullptr;
    }
    return ptr_;
  }

  int GetReferCount() const {
    return refer_count_ ? refer_count_->load(std::memory_order_acquire) : 0;
  }

private:
  void Release() {
    if (refer_count_ != nullptr) {
      if ((refer_count_->fetch_sub(1,std::memory_order_relaxed) -1) == 0) {
        delete ptr_;
        delete refer_count_;
      }
    }
    ptr_ = nullptr;
    refer_count_ = nullptr;
  }

  // 调试接口
  // void Release() {
  //   int current_count = 0;
  //   if (refer_count_ != nullptr) {
  //     current_count = --(*refer_count_); // 先记录自减后的计数
  //     if (current_count == 0) {
  //       delete ptr_;
  //       delete refer_count_;
  //       printf("  Release(): 计数减到0，销毁Test对象\n");
  //     } else {
  //       printf("  Release(): 计数减到%d\n", current_count);
  //     }
  //   }
  //   ptr_ = nullptr;
  //   refer_count_ = nullptr;
  // }

private:
  T *ptr_ = nullptr;
  std::atomic<int> *refer_count_ = nullptr;
};

class Test {
public:
  Test() { printf("Test()\n"); }

  ~Test() { printf("~Test()\n"); }
};

int main() {
  // 修复变量命名格式
  SharedPtr<Test> test(new Test);
  // 测试拷贝构造
  SharedPtr<Test> test2 = test;
  printf("引用计数：%d\n", test2.GetReferCount()); // 输出 2
  // 测试赋值重载
  SharedPtr<Test> test3;
  test3 = test2;
  printf("引用计数：%d\n", test3.GetReferCount()); // 输出 3
  return 0;
}