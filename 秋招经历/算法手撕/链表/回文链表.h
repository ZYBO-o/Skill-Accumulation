/*
    首先找到中心结点，将链表等分为二
    右半链表进行反转操作
    左链表与反转后的右链表进行比较
*/
#include "List.h"

bool isPalindrome(ListNode* head) {
    if(!head) return true;
    
    int count = 0;
    bool ret = true;
    
    ListNode* firstNode = head;
    ListNode* secondNode = head;
    while(firstNode!= nullptr && firstNode->next != nullptr ) {
        ++ count;
        firstNode = firstNode->next->next;
        secondNode = secondNode->next;
    }
    if(firstNode) {
        count++;
    }
        
    //找到右半链表
    //first链表为需要反转的链表
    ListNode* secondEnd = secondNode;
    ListNode* pre = secondNode;
    ListNode* cur = secondNode->next;
    ListNode* temp;
    while(cur != nullptr) {
        temp = cur->next;
        cur->next = pre;
        pre = cur;
        cur = temp;
    }
    secondEnd->next = nullptr;
    
    //比较左半链表与右半链表
    while(count -- ) {
        if(pre->value == head->value) {
            head = head->next;
            pre = pre->next;
        } else {
            ret = false;
            break;
        }
    }
    return ret;
} 