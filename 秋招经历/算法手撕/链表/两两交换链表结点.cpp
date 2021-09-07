/*
    定义dummryNode结点，用pre来进行指向
    当 pre->next 与 pre->next->next 不为空时，定义node1 与 node2 进行指向
    让这两个结点进行交换，并更新 pre 向前
*/

#include "List.h"
ListNode* swapPairs(ListNode* head) {
    if(head == nullptr) return head;
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = head;
    ListNode* pre = dummryNode;

    while(pre->next != nullptr && pre->next->next != nullptr) {
        ListNode* node1 = pre->next;
        ListNode* node2 = pre->next->next;
        pre->next = node2;
        node1->next = node2->next;
        node2->next = node1;
        pre = node1;
    }
    return dummryNode->next;
}