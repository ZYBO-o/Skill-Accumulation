/*
    定义list1,list2 指针指向l1与l2的头结点，pre指向list1的上一个结点(为了好插入)

    在遍历结点都不为空的情况下进行比较
        如果list1 <= list2，list1继续迭代
        如果list1 > list2，则将list2结点插入到list1前面
    遍历结束后，有一个不为空

    list1不为空则不用管
    list2不为空，将list2后面的结点直接移到list1后面
*/

#include "List.h"

ListNode* Merge(ListNode* pHead1, ListNode* pHead2) {
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = pHead1;
    
    ListNode* pre = dummryNode;
    ListNode* list1 = pHead1;
    ListNode* list2 = pHead2;
    
    while(list1 && list2) {
        if(list1->value <= list2->value) {
            list1 = list1->next;
            pre = pre->next;
        } else {
            ListNode* temp = list2;
            list2 = list2->next;
            
            pre->next = temp;
            temp->next = list1;
            pre = temp;
        }
    }
    
    if(list2) {
        pre->next = list2;
    }
    return dummryNode->next;
}