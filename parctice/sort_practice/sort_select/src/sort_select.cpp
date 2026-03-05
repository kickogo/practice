#include <iostream>
#include <vector>

// 选择排序（整数升序，核心逻辑）
void select_sort(std::vector<int> &arr) {
  // 空数组/单元素数组直接返回
  if (arr.size() <= 1)
    return;

  // 外层循环：确定第i个位置的最小元素
  for (int i = 0; i < arr.size() - 1; ++i) {
    int min_idx = i; // 假设当前i是最小元素下标
    // 内层循环：找i之后的最小元素
    for (int j = i + 1; j < arr.size(); ++j) {
      if (arr[j] < arr[min_idx]) {
        min_idx = j;
      }
    }
    std::swap(arr[i], arr[min_idx]);
  }
}

// 打印数组辅助函数
void print_array(const std::vector<int> &arr, const std::string &desc) {
  std::cout << desc << ": ";
  for (int num : arr) {
    std::cout << num << " ";
  }
  std::cout << std::endl;
}

// 主函数测试
int main() {
  // 测试用例
  std::vector<int> arr = {9, 3, 7, 5, 6, 4, 8, 2, 1};

  print_array(arr, "排序前");
  select_sort(arr);
  print_array(arr, "排序后");

  return 0;
}