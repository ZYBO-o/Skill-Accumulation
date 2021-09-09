#include "BTree.h"
#include<vector>

vector<vector<int>> result;    
vector<int> path;    
// 递归函数不需要返回值，因为我们要遍历整个树    
void traversal(TreeNode* cur, int count) {        
  	if (!cur->left && !cur->right && count == 0) { 
      // 遇到了叶子节点切找到了和为sum的路径            
      result.push_back(path);            
      return;       
  	}        
    if (!cur->left && !cur->right) 
      	return ; 
    // 遇到叶子节点而没有找到合适的边，直接返回        
    if (cur->left) { // 左 （空节点不遍历）            
        path.push_back(cur->left->val);            
        count -= cur->left->val;            
        traversal(cur->left, count);    // 递归            
        count += cur->left->val;        // 回溯            
        path.pop_back();                // 回溯        
    }        
    if (cur->right) { // 右 （空节点不遍历）            
        path.push_back(cur->right->val);            
        count -= cur->right->val;            
        traversal(cur->right, count);   // 递归            
        count += cur->right->val;       // 回溯            
        path.pop_back();                // 回溯        
    }        
    	return ;    
}
vector<vector<int>> pathSum(TreeNode* root, int sum) {        
    result.clear();        
    path.clear();        
    if (root == NULL) return result;       
    path.push_back(root->val); // 把根节点放进路径        
    traversal(root, sum - root->val);        
    return result;    
}
