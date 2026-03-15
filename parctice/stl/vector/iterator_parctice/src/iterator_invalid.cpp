#include <iostream>
#include <vector>

int main() {
    std::vector<int> vec = {1, 2, 3, 4};
    auto it = vec.begin() + 1;  // 指向元素 2
    // std::cout << *it << std::endl;  // 未定义行为！it 已失效


    // // 操作1：push_back 导致扩容，所有迭代器失效
    // vec.push_back(5);
    // std::cout << *it << std::endl;  // 未定义行为！it 已失效

    // // 重置迭代器后测试 erase
    // it = vec.begin() + 1;
    // vec.erase(it);  // 删除元素 2，it 失效
    // ++it;  // 未定义行为！
    // std::cout << *it << std::endl;  // 未定义行为！it 已失效

    // 正确做法：用 erase 的返回值更新迭代器
    it = vec.begin() + 1;
    it = vec.erase(it);  // erase 返回指向删除位置下一个元素的有效迭代器
    std::cout << *it << std::endl;  // 输出 4（正确）
    return 0;
}