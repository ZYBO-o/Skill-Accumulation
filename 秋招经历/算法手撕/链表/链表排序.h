#include "List.h"
ListNode* middleNode(ListNode* head) {
    ListNode* fast = head;
    ListNode* slow = head;
    ListNode* pre = slow;
    while(fast != nullptr && fast->next != nullptr) {
        pre = slow;
        fast = fast->next->next;
        slow = slow->next;
    }
    pre->next = nullptr;
    return slow;
}

ListNode* Merge(ListNode* list1, ListNode* list2) {
    ListNode* dummyNode = new ListNode(0);
    ListNode* p = dummyNode;
    while(list1 && list2) {
        if(list1->value <= list2->value) {
            p->next = list1;
            list1 = list1->next;
        } else {
            p->next = list2;
            list2 = list2->next;
        }
        p = p->next;
    }
    p->next = list1 ? list1 : list2;
    return dummyNode->next;
}

ListNode* Sort(ListNode* head) {
    if(head == nullptr || head->next == nullptr)
        return head;
    ListNode* mid = middleNode(head);
    ListNode* left = Sort(head);
    ListNode* right = Sort(mid);
    return Merge(left, right); 
}

















