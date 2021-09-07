/*
    定义leftnode指针和rightnode指针分别指向需要反转链表的两端结点
    定义pre,cur,temp指针，将left与right的链表进行反转
    最后反转完的链表重新指回leftnode与rightnode
*/

 ListNode* reverseBetween(ListNode* head, int left, int right) {
    if(head->next == nullptr) return head;
    //防止left指向head结点，所以定义空节点
    ListNode* dummryNode = new ListNode(0);
    dummryNode->next = head;

    ListNode* LeftNode = dummryNode;    //指向需要反转链表的左结点
    ListNode* RightNode = head;         //指向需要反转链表的右节点

    while(--left)
        LeftNode = LeftNode->next;
    while(right--)
        RightNode = RightNode->next;

    ListNode* pre = RightNode;          //最开始前结点直接指向 RightNode
    ListNode* cur = LeftNode->next;
    ListNode* temp = nullptr;
    while(cur != RightNode) {
        temp = cur->next;
        cur->next = pre;
        pre = cur;
        cur = temp;
    }
    LeftNode->next = pre;               //最后，左结点指向反转后的第一个结点
    return dummryNode->next;
        
}