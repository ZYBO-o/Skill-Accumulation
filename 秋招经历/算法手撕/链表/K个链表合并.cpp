#include"List.h"



class Solution {
private:
    //合并两个链表
    ListNode* mergeTwoLists(ListNode* pHead1, ListNode* pHead2) {
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

    ListNode* merge(vector <ListNode*> &lists, int l, int r) {
        if (l == r) 
            return lists[l];
        if (l > r) 
            return nullptr;
        int mid = l + (r - l) / 2;
        return mergeTwoLists( merge(lists, l, mid), merge(lists, mid + 1, r) );
    }

public:

    ListNode* mergeKLists(vector<ListNode*>& lists) {
        return merge(lists, 0, lists.size() - 1);
    }
};



