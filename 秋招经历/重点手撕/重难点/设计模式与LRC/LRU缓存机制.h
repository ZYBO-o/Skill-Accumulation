/*  介绍：
        计算机的缓存容量有限，如果缓存满了就要删除一些内容，给新内容腾位置。
        LRU Least Recently Used 缓存淘汰算法认为最近使用过的数据应该是是「有用的」，很久都没用过的数据应该是无用的，内存满了就优先删那些很久没用过的数据。
    算法实现：
        双向链表按照被使用的顺序存储了这些键值对，靠近头部的键值对是最近使用的，而靠近尾部的键值对是最久未使用的。
        哈希表即为普通的哈希映射（HashMap），通过缓存数据的键映射到其在双向链表中的位置。
*/

/*
    LFU（Least Frequently Used)即最近不经常使用算法。
    算法核心是记录每一个元素的访问频率，当容器满了的时候，每次移出的都是访问频率较小的元素，如果两个元素的频率相同的话，就先移除添加容器较早的元素。
*/

#include<unordered_map>
#include <iostream>

struct DLinkedNode {
    int key, value;
    DLinkedNode* next;
    DLinkedNode* pre;
    DLinkedNode():key(0),value(0),next(nullptr),pre(nullptr) {}
    DLinkedNode(int key, int value) : key(key), value(value), next(nullptr),pre(nullptr) {}
};

class LRUCache {
private:
    //结点哈希表
    std::unordered_map<int, DLinkedNode*> cache;
    //头尾结点
    DLinkedNode* head;
    DLinkedNode* tail;
    int capacity;
    int size;

public:
    //创见头尾结点，并相互指向
    LRUCache(int capacity) : size(0), capacity(capacity) {
        //创建头尾结点，并相互指向
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->pre = head;
    }
    
    //如果有结点，则返回，并将结点重新放入至链表头部
    //如果没有结点，则返回-1
    int get(int key) {
        if(cache.count(key) == 0) 
            return -1;
        DLinkedNode* node = cache[key];
        moveToHead(node);
        return node->value;

    }
    
    void put(int key, int value) {
        if(cache.count(key) == 0) {
            DLinkedNode* node = new DLinkedNode(key, value);
            // 添加进哈希表
            cache[key] = node;
            // 添加至双向链表的头部
            addToHead(node);
            ++size;
            if (size > capacity) {
                // 如果超出容量，删除双向链表的尾部节点
                DLinkedNode* removed = removeTail();
                // 删除哈希表中对应的项
                cache.erase(removed->key);
                // 防止内存泄漏
                delete removed;
                --size;
            }
        } else {
            // 如果 key 存在，先通过哈希表定位，再修改 value，并移到头部
            DLinkedNode* node = cache[key];
            node->value = value;
            moveToHead(node);
        }
    }

    //功能函数：添加至头部
    void addToHead(DLinkedNode* node) {
        node->next = head->next;
        head->next->pre = node;
        head->next = node;
        node->pre = head;
    }

    //移动结点
    void removeNode(DLinkedNode* node){
        node->pre->next = node->next;
        node->next->pre = node->pre;
    }
    //功能函数：移动至头部
    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    //功能函数: 删除尾部结点
    DLinkedNode* removeTail() {
        //指向尾结点的上一个结点
        DLinkedNode* node = tail->pre;
        removeNode(node);
        return node;
    }
};
