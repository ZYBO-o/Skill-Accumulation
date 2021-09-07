#include "BTree.h"

class Solution {
public:    
  TreeNode* mergeTrees(TreeNode* t1, TreeNode* t2) {        
    if (t1 == NULL) return t2; // 如果t1为空，合并之后就应该是t2        
    if (t2 == NULL) return t1; // 如果t2为空，合并之后就应该是t1        
    // 修改了t1的数值和结构        
    t1->val += t2->val;                             // 中        
    t1->left = mergeTrees(t1->left, t2->left);      // 左        
    t1->right = mergeTrees(t1->right, t2->right);   // 右        
    return t1;    
  }
};

