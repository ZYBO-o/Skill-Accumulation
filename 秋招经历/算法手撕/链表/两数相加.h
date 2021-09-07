/*
    将list2 作为结果链表进行返回，则先定义dummryNode指向list2,然后定义pre指向dummryNode，定义记录进位的标志
    进行遍历，定义int 遍历，如果结点不为空，则为结点值，否则为 0 
    当 遍历到list 为 空，list 1不为空时会自动创建结点补上
*/

#include "List.h"
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode* list1 = l1;
    ListNode* list2 = l2;
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = list2;
    ListNode* pre = dummryNode;
    int carry = 0;//记录进位
    while(list1 || list2 || carry) {
        int num1 = list1 == nullptr ? 0 : list1->value;
        int num2 = list2 == nullptr ? 0 : list2->value;

        if(list2 == nullptr) {
            ListNode* node = new ListNode((num1 + num2 + carry) % 10);
            pre->next = node;
            node->next = nullptr;
            pre = node;
        } else {
            list2->value = (num1 + num2 + carry) % 10;
        }

        if(num1 + num2 + carry > 9) 
            carry = 1;
        else carry = 0;

        if(list1)
            list1 = list1->next;
        if(list2) {
            list2 = list2->next;
            pre = pre->next;
        }
    }
    return l2;
}