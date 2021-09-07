

#include "List.h"
#include <unordered_map>

/*
    初始化： 哈希表unordered_map<Node*, Node*> map ， 定义节点 cur 指向头节点；
    
    建立新节点，并向 map 添加键值对 (原 cur 节点, 新 cur 节点） ；

    构建新节点的 next 和 random 引用指向；
*/
class Node {
public:
    int val;
    Node* next;
    Node* random;
    
    Node(int _val) {
        val = _val;
        next = NULL;
        random = NULL;
    }
};

Node* copyRandomList(Node* head) {
    if(head == nullptr) return nullptr;
    Node* cur = head;
    unordered_map<Node*, Node*> map;
    // 3. 复制各节点，并建立 “原节点 -> 新节点” 的 Map 映射
    while(cur != nullptr) {
        map[cur] = new Node(cur->val);
        cur = cur->next;
    }
    cur = head;
    // 4. 构建新链表的 next 和 random 指向
    while(cur != nullptr) {
        map[cur]->next = map[cur->next];
        map[cur]->random = map[cur->random];
        cur = cur->next;
    }
    // 5. 返回新链表的头节点
    return map[head];
}