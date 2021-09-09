#include"BTree.h"
#include<queue>
#include<iostream>
#include<sstream>

class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if(!root) return"#,";
        string res=to_string(root->val)+",";
        res+=serialize(root->left);
        res+=serialize(root->right);
        return res;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {
        queue<string> q;//使用队列来存储前序遍历的节点
        stringstream ss(data);
        string s;
        while(getline(ss,s,',')){//使用string流来读取对象
            q.push(s);
        }
        return reconPreOrder(&q);
    }

    TreeNode* reconPreOrder(queue<string>* q){//这里一定要记得是传址，使用引用或者指针！！！切记！！
        string res=q->front();
        q->pop();
        if(res=="#") return nullptr;
        TreeNode* tmp=new TreeNode(stoi(res));
        tmp->left=reconPreOrder(q);
        tmp->right=reconPreOrder(q);
        return tmp;
    }
};