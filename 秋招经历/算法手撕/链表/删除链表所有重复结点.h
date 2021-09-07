/*
    一边遍历、一边统计相邻节点的值是否相等，如果值相等就继续后移找到值不等的位置，然后删除值相等的这个区间。

    设置 Pre 与 Cur指针，比较Pre->next 与 Cur->next 的值，如果相等，则删除Cur->next的值，这样会出现第一个相等的值删不掉
    
    解决上述问题，可以设置flag标志，只要有相等就让flag=1，这样最后剩下的元素也能被删除，之后再重新设置为0

*/

#include "List.h" 

ListNode* deleteDuplicates(ListNode* head) {
    if(!head) return head;
    ListNode* dummyNode = new ListNode(0);
    dummyNode->next = head;
    ListNode* Cur = head;
    ListNode* Pre = dummyNode;
    //设置标志
    int flag = 0;
    while(Cur != nullptr && Cur->next != nullptr) {
        while(Cur->next != nullptr && Pre->next->value == Cur->next->value ) {
            ListNode* temp = Cur->next;
            Cur->next = temp->next;
            delete temp;
            flag = 1;
        } 
            
        //删除最后一个相等的值
        if(flag) {
            ListNode* temp = Cur;
            Cur = Cur->next;
            Pre->next = Cur;
            delete temp;
            flag = 0;
        } else {
            Cur = Cur->next;
            Pre = Pre->next;
        }
    
    }
    return dummyNode->next;
}