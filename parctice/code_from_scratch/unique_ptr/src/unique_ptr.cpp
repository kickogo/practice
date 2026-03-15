#include <stdexcept>
#include <stdio.h>

template <typename T> class UniquePtr {
public:
  explicit UniquePtr(T *ptr = nullptr) : ptr_(ptr) {}
  ~UniquePtr() noexcept { delete ptr_; }

  UniquePtr(const UniquePtr &other) = delete;
  UniquePtr &operator=(const UniquePtr &other) = delete;

  UniquePtr(UniquePtr &&other) noexcept : ptr_(other.ptr_) {
    other.ptr_ = nullptr;
  }
  UniquePtr &operator=(UniquePtr &&other) noexcept {
    if (this != &other) {
      if (ptr_ != nullptr) {
        delete ptr_;
      }
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }

  //指针操作
  T *operator->() const {
    if (ptr_ != nullptr) {
      return ptr_;
    }
    throw std::runtime_error("UniquePtr: access null pointer.\n");
  }

  T &operator*() const {
    if (ptr_ != nullptr) {
      return *ptr_;
    }
    throw std::runtime_error("UniquePtr:dereference null pointer.\n");
  }

  T *GetRawPtr() const noexcept { return ptr_; }

  T *Release() noexcept {
    T *temp = ptr_;
    ptr_ = nullptr;
    return temp;
  }

  void Reset(T *new_ptr = nullptr) noexcept {
    if (ptr_ != new_ptr) {
      T *old_ptr = ptr_;
      ptr_ = new_ptr;
      if (old_ptr) {
        delete old_ptr;
      }
    }
  }

  void Swap(UniquePtr &other) noexcept { std::swap(ptr_, other.ptr_); }

  // 判空
  explicit operator bool() const noexcept { return ptr_ != nullptr; }

private:
  T *ptr_ = nullptr;
};