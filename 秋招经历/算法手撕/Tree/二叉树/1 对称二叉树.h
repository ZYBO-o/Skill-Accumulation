#include "BTree.h"

bool isMirror(TreeNode* left, TreeNode* right) {
    //返回情况：空节点与非空节点
    //空节点
    if(left == nullptr && right != nullptr) 
        return false;
    if(left != nullptr && right == nullptr)
        return false;
    if(left == nullptr && right == nullptr)
        return true;
    //非空节点
    if(left->val != right->val)
        return false;
    //递归逻辑
    bool Compareoutside = isMirror(left->left,right->right);
    bool Compareinside  = isMirror(left->right,right->left);
    return Compareoutside&&Compareinside;
}

bool isSymmetric(TreeNode* root) {
    if(root == nullptr) return true;
    return isMirror(root->left,root->right);
}

//队列来实现：
bool isSymmetric2(TreeNode* root) {
    queue<TreeNode*> queue;
  	
    queue.push(root->left);// 将左子树头结点加入队列
    queue.push(root->right);// 将右子树头结点加入队列
  	
    while(!queue.empty()) {// 接下来就要判断这这两个树是否相互翻转
        TreeNode* ltree = queue.front();
        queue.pop();
        TreeNode* rtree = queue.front();
        queue.pop();
        
        if(ltree==nullptr && rtree==nullptr)// 左节点为空、右节点为空，此时说明是对称的,进行下一轮
            continue;
        if(ltree==nullptr || rtree==nullptr)
            return false;
        if(ltree->val != rtree->val)
            return false;
      	
        queue.push(ltree->left);// 加入左节点左孩子
        queue.push(rtree->right);// 加入右节点右孩子	
        queue.push(ltree->right);// 加入左节点右孩子
        queue.push(rtree->left);// 加入右节点左孩子
    }
    return true;
}   























