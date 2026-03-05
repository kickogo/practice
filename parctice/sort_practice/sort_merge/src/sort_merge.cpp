#include <iostream>
#include <vector>
using namespace std;

// 合并两个有序区间
void merge(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp(right - left + 1);
    int i = left, j = mid + 1, k = 0;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i <= mid) temp[k++] = arr[i++];
    while (j <= right) temp[k++] = arr[j++];

    // 拷回原数组
    for (i = left, k = 0; i <= right; i++, k++)
        arr[i] = temp[k];
}

// 归并排序
void merge_sort(vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = (left + right) / 2;
    merge_sort(arr, left, mid);
    merge_sort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

int main() {
    vector<int> arr = {9,3,7,5,6,4,8,2,1};
    merge_sort(arr, 0, arr.size()-1);

    for (int x : arr) cout << x << " ";
    return 0;
}