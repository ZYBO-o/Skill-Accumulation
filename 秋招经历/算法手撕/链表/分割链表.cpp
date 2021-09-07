/*
    首先找到第一个大于等于分割点的结点，标记为LeftNode，然后往下遍历
    每次找到比分割点 x 小的结点时，将结点移到dummryNode结点之后
*/


#include "List.h"

ListNode* partition(ListNode* head, int x) {
    if(head == nullptr) return nullptr;
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = head;

    ListNode* LeftNode = dummryNode;
    ListNode* RightNode = nullptr;

    while(LeftNode->next != nullptr && LeftNode->next->value < x) {
        LeftNode = LeftNode->next;
    }
    RightNode = LeftNode->next; //找到第一个比x大的结点

    while(RightNode != nullptr && RightNode->next != nullptr) {
        if(RightNode->next->value >= x)
            RightNode = RightNode->next;
        else {
            //将这个结点进行移位
            ListNode* temp = RightNode->next;
            RightNode->next = temp->next;
            //将结点插入dummryNode结点之后
            temp->next = dummryNode->next;
            dummryNode->next = temp;
        }
    }
    return dummryNode->next;
}