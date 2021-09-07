/*
    找到链表的中心，将链表分为两个链表
    将后面的链表进行反转
    最后将两张链表进行合并
*/
#include "List.h"
void Merge(ListNode* head, ListNode*pre) {
    ListNode* p = head;
    while(p->next != nullptr && pre != nullptr) {
        ListNode* temp = p->next;
        p->next = pre;
        pre = pre->next;
        p = p->next;
        p->next = temp;
        p = temp;
    }
    p->next = nullptr;
}

void reorderList(ListNode* head) {
    if(!head) return;
    
    ListNode* firstNode = head;
    ListNode* secondNode = head;
    while(secondNode != nullptr && secondNode->next != nullptr) {
        firstNode = firstNode->next;
        secondNode = secondNode->next->next;
    }
    
    ListNode* firstEnd = firstNode;
    ListNode* pre = firstNode;
    ListNode* cur = firstNode->next;
    firstNode = nullptr;
    while(cur) {
        ListNode* temp = cur->next;
        cur->next = pre;
        pre = cur;
        cur = temp;
    }
    firstEnd->next = nullptr;

    Merge(head, pre);

}
//head:1->2->3
//pre: 4->3
//所以head中的3不能使用，等到p->next = null就截止，最后补上nullptr就行

