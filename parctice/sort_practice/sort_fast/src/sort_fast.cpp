#include <iostream>
#include <vector>
#include <algorithm> // std::swap
// 分区函数：直观版（注释更详细，步骤更清晰）
int partition(std::vector<int>& arr, int left, int right) {
    // 1. 选择基准（这里仍选最右侧，标注清楚目的）
    int pivot_val = arr[right]; // 基准值：选区间最后一个元素（简单易理解）
    int small_idx = left - 1;   // 「小于基准区」的右边界（初始在区间外）

  

    // 2. 遍历区间[left, right-1]，划分「小于基准区」和「大于基准区」
    for (int current_idx = left; current_idx < right; ++current_idx) {
        // 如果当前元素 ≤ 基准值 → 划入「小于基准区」
        if (arr[current_idx] <= pivot_val) {
            small_idx++; // 「小于基准区」右边界右移
            std::swap(arr[small_idx], arr[current_idx]); // 把当前元素放进「小于基准区」
        }
    }

    // 3. 把基准值放到「小于基准区」的右侧（基准的最终正确位置）
    int pivot_final_idx = small_idx + 1;
    std::swap(arr[pivot_final_idx], arr[right]);


    return pivot_final_idx;
}

// 快速排序递归函数（直观版）
void quick_sort(std::vector<int>& arr, int left, int right) {
    // 递归终止条件：区间长度≤1（天然有序）
    if (left >= right) {
        return;
    }

    // 2. 分区：把数组分成「小于基准」和「大于基准」两部分，返回基准位置
    int pivot_idx = partition(arr, left, right);

    // 3. 递归处理左区间（小于基准）和右区间（大于基准）
    std::cout << "递归处理左区间: [" << left << "," << pivot_idx - 1 << "]\n";
    quick_sort(arr, left, pivot_idx - 1);
    std::cout << "递归处理右区间: [" << pivot_idx + 1 << "," << right << "]\n";
    quick_sort(arr, pivot_idx + 1, right);
}

// 封装函数（对外简单接口）
void quick_sort(std::vector<int>& arr) {
    if (arr.empty()) return;
    std::cout << "===== 快速排序开始 =====" << std::endl;
    quick_sort(arr, 0, arr.size() - 1);
    std::cout << "===== 快速排序结束 =====" << std::endl;
}

// 打印数组
void print_array(const std::vector<int>& arr, const std::string& desc) {
    std::cout << desc << ": ";
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << "\n" << std::endl;
}

// 测试
int main() {
    std::vector<int> arr = {9,3,7,1,6,4,8,2,5};
    print_array(arr, "排序前");
    
    quick_sort(arr);
    
    print_array(arr, "排序后");
    return 0;
}