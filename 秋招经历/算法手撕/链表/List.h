#include <iostream>
#include <vector>
using namespace std;

class ListNode {
public:
	int value;
	ListNode * next;

	ListNode():value(0), next(nullptr) {}
	ListNode(int val): value(val), next(nullptr) {}
	ListNode(int val, ListNode* node) : value(val), next(node) {}

	
};

ListNode* CreateList(vector<int>& nums){
	ListNode* dummryNode = new ListNode(0);
	ListNode* p = dummryNode;
	for(auto num : nums) {
		ListNode* node = new ListNode(num);
		p->next = node;
		p = node;
	}
	return dummryNode->next;
}

void PrintList(ListNode* root) {
	ListNode* p = root;
	while(p != nullptr) {
		cout << p->value << "->";
		p = p->next;
	}
	cout << endl;
}
