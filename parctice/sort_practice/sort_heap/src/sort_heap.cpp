#include <iostream>
#include <vector>
#include <algorithm> // std::swap
using namespace std;

// 调整以 i 为根的子树为大顶堆
void heap_adjust(vector<int>& arr, int i, int len) {
    int largest = i;       // 父节点
    int left = 2 * i + 1;  // 左孩子
    int right = 2 * i + 2; // 右孩子

    if (left < len && arr[left] > arr[largest])
        largest = left;
    if (right < len && arr[right] > arr[largest])
        largest = right;

    if (largest != i) {
        swap(arr[i], arr[largest]);
        heap_adjust(arr, largest, len); // 递归调整受影响的子树
    }
}

// 堆排序
void heap_sort(vector<int>& arr) {
    int n = arr.size();

    // 1. 建大顶堆
    for (int i = n / 2 - 1; i >= 0; --i)
        heap_adjust(arr, i, n);

    // 2. 一个个把堆顶放到末尾
    for (int i = n - 1; i > 0; --i) {
        swap(arr[0], arr[i]);  // 堆顶（最大）放到最后
        heap_adjust(arr, 0, i); // 调整剩余的堆
    }
}

int main() {
    vector<int> arr = {9, 3, 7, 5, 6, 4, 8, 2, 1};
    heap_sort(arr);

    for (int x : arr) cout << x << " ";
    return 0;
}