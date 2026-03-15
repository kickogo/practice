#include <unordered_map>
using namespace std;

// 双向链表节点定义
struct DLinkedNode {
    int key, value;
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int _key, int _value) : key(_key), value(_value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, DLinkedNode*> cache; // 哈希表：key→节点
    DLinkedNode* head; // 虚拟头节点（最近使用）
    DLinkedNode* tail; // 虚拟尾节点（最久未使用）
    int size; // 当前缓存大小
    int capacity; // 缓存最大容量

public:
    // 初始化：虚拟头尾节点相连，容量初始化
    LRUCache(int _capacity) : capacity(_capacity), size(0) {
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    // 获取缓存
    int get(int key) {
        // 1. key不存在，返回-1
        if (!cache.count(key)) {
            return -1;
        }
        // 2. key存在，找到节点并移到头部（标记为最近使用）
        DLinkedNode* node = cache[key];
        moveToHead(node);
        return node->value;
    }

    // 插入/更新缓存
    void put(int key, int value) {
        // 1. key已存在：更新value，移到头部
        if (cache.count(key)) {
            DLinkedNode* node = cache[key];
            node->value = value;
            moveToHead(node);
            return;
        }

        // 2. key不存在：新建节点
        DLinkedNode* newNode = new DLinkedNode(key, value);
        cache[key] = newNode; // 哈希表记录
        addToHead(newNode); // 插入头部
        size++; // 容量+1

        // 3. 容量超限：删除最久未使用的节点（尾节点前驱）
        if (size > capacity) {
            DLinkedNode* removed = removeTail(); // 删除尾节点
            cache.erase(removed->key); // 哈希表删除对应key
            delete removed; // 释放内存（避免泄漏）
            size--; // 容量-1
        }
    }

private:
    // 辅助：将节点添加到头部（虚拟头节点后）
    void addToHead(DLinkedNode* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    // 辅助：删除指定节点
    void removeNode(DLinkedNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 辅助：将节点移到头部（先删除，再添加）
    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    // 辅助：删除尾节点（虚拟尾节点前驱），返回被删除的节点
    DLinkedNode* removeTail() {
        DLinkedNode* node = tail->prev;
        removeNode(node);
        return node;
    }
};

// 测试示例
int main() {
    LRUCache lRUCache(2); // 容量2
    lRUCache.put(1, 1);   // 缓存：{1=1}
    lRUCache.put(2, 2);   // 缓存：{1=1, 2=2}
    lRUCache.get(1);      // 返回1，缓存：{2=2, 1=1}（1移到头部）
    lRUCache.put(3, 3);   // 容量满，淘汰2，缓存：{1=1, 3=3}
    lRUCache.get(2);      // 返回-1（2已被淘汰）
    lRUCache.put(4, 4);   // 容量满，淘汰1，缓存：{3=3, 4=4}
    lRUCache.get(1);      // 返回-1（1已被淘汰）
    lRUCache.get(3);      // 返回3，缓存：{4=4, 3=3}
    lRUCache.get(4);      // 返回4，缓存：{3=3, 4=4}
    return 0;
}