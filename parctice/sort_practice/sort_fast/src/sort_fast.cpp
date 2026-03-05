#include <iostream>
#include <vector>
#include <algorithm> // std::swap

// 分区函数：返回基准元素的最终位置
int partition(std::vector<int>& arr, int left, int right) {
    int pivot = arr[right]; // 选最右侧元素作基准（简单易实现）
    int i = left - 1;       // 小于基准的区域边界（初始在左边界外）

    // 遍历[left, right-1]，划分小于/大于基准的区域
    for (int j = left; j < right; ++j) {
        if (arr[j] <= pivot) { // 小于等于基准，划入左区域
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    // 把基准放到正确位置（i+1是基准的最终下标）
    std::swap(arr[i+1], arr[right]);
    return i + 1;
}

// 快速排序递归函数
void quick_sort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return; // 子数组长度≤1，无需排序

    int pivot_idx = partition(arr, left, right); // 分区，获取基准位置
    quick_sort(arr, left, pivot_idx - 1);        // 递归处理左区间
    quick_sort(arr, pivot_idx + 1, right);       // 递归处理右区间
}

// 封装函数（对外提供简单接口）
void quick_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    quick_sort(arr, 0, arr.size() - 1);
}

// 打印数组
void print_array(const std::vector<int>& arr, const std::string& desc) {
    std::cout << desc << ": ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

// 测试
int main() {
    std::vector<int> arr = {9,3,7,5,6,4,8,2,1};
    print_array(arr, "排序前");
    quick_sort(arr);
    print_array(arr, "排序后");
    return 0;
}