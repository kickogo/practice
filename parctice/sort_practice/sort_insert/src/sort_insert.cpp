#include <iostream>
#include <vector>

// 插入排序（整数升序，核心逻辑）
void sort_insert(std::vector<int> &arr) {
  for (int i = 1; i < arr.size(); i++) {
    int value = arr[i];
    int j = i - 1;
    while (value < arr[j]) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = value;
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
  sort_insert(arr);
  print_array(arr, "排序后");

  return 0;
}