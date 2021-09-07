#include "List.h"

ListNode* ReverseLink(ListNode* root) {
	ListNode* pre = nullptr;
	ListNode* cur = root;
	ListNode* temp = nullptr;
	while(cur != nullptr) {
		temp = cur->next;
		cur->next = pre;
		pre = cur;
		cur = temp;
	}
	return pre;
}

int main() {
	vector<int> nums {1,2,3,4,5,6,7,8,9};
	ListNode* root = CreateList(nums);
	PrintList(root);
	root = ReverseLink(root);
	PrintList(root);
}

//1->2->3->4