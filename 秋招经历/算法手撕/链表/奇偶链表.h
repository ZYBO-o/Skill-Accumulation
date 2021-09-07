/*
    Even指向Odd的next指针，然后Even进行更新
    Odd指向Even的next指针，然后Odd进行更新
    直到有一方的next为空
*/

#include "List.h"
ListNode* oddEvenList(ListNode* head) {
    if(head == nullptr) return head;
    
    ListNode* even = head;
    ListNode* old = head->next;
    ListNode* evenNext = old;

    while(even->next != nullptr && old->next != nullptr) {
        even->next = old->next;
        even = even->next;
        old->next = even->next;
        old = old->next;
    }
    even->next = evenNext;
    return head;
}
