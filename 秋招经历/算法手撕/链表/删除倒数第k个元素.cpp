/*
    使用两个指针 first 和 second 同时对链表进行遍历，
    first 比 second 超前 n+1 个节点。为了更好的做删除操作
    当 first 遍历到链表的末尾时，second 就恰好处于倒数第 n 个节点。
    
*/

#include "List.h"

ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = head;
    
    ListNode* fast = dummryNode;
    ListNode* slow = dummryNode;
    while(n-- && fast != nullptr) {
        fast = fast->next;
    }
    fast = fast->next;//再提前走一步，好操作删除
    while(fast != nullptr) {
        fast = fast->next;
        slow = slow->next;
    }
    ListNode* temp = slow->next;
    slow->next = slow->next->next;
    delete temp;
    return dummryNode->next;
}