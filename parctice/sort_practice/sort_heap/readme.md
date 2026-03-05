我用**最直白、最好记、面试能直接说出口**的方式给你讲 **堆排序（Heap Sort）**。

# 堆排序是什么？一句话
**把数组看成一棵完全二叉树（堆），先建成大顶堆，再不断把最大的元素“拎”到最后，逐步形成有序数组。**

---

# 一、堆排序的核心思想（3 步记住）
1. **建堆**
   把无序数组变成一个 **大顶堆**（根节点是整个数组最大值）。

2. **交换堆顶和末尾**
   把最大值（堆顶）放到数组最后面，固定不动。

3. **调整剩下的堆**
   排除最后已经排好的元素，对前面的部分 **重新调整成堆**，重复步骤 2。

直到整个数组有序。

---

# 二、什么是堆？（你只要记住这一句）
堆 = **用数组表示的完全二叉树**
满足：
- **大顶堆**：父节点 ≥ 两个子节点
- **小顶堆**：父节点 ≤ 两个子节点

堆排序**一般用大顶堆**。

---

# 三、堆排序的特点（面试必背）
- 时间复杂度：**始终 O(n log n)**
- 空间复杂度：**O(1)**（原地排序）
- 不稳定排序
- 不用递归，不用额外数组，比快排省空间

---

# 四、最简单的 C++ 堆排序（你直接能跑）
只排 **int**，逻辑清晰，没有多余封装：

```cpp
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
```

运行输出：
```
1 2 3 4 5 6 7 8 9
```

---

# 五、快速对比 4 大排序（你面试直接背）
- **冒泡/插入/选择：O(n²)** 慢，适合小数据
- **快速排序：O(n log n)** 最快，但不稳定、递归
- **归并排序：O(n log n)** 稳定，但要额外空间
- **堆排序：O(n log n)** 不递归、不占额外空间，但比快排慢一点

---

如果你要，我可以马上帮你把
**堆排序 → 放进你的 practice 仓库结构 + 写好 CMakeLists**