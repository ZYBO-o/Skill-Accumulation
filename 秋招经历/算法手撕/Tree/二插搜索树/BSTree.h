#include <iostream>


using namespace std;

class BSTreeNode {
public:
    int val;
    BSTreeNode* left;
    BSTreeNode* right;
    BSTreeNode() : val(0), left(nullptr), right(nullptr) {}
    BSTreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    BSTreeNode(int x, BSTreeNode *left, BSTreeNode *right) : val(x), left(left), right(right) {}
};

void CreatBSTree(BSTreeNode* &root) {
    int value;
    while(cin >> value) {

        auto newNode = new BSTreeNode(value);
        if(root == nullptr)//根结点
            root = newNode;
        else {             //非根节点
            auto pTemp = root;
            while(true) {
                //输入的树小于该结点，向左
                if(value < pTemp->val) {
                    if(pTemp->left == nullptr) {//如果结点左子树为空，则直接插入
                        pTemp->left = newNode;
                        break;
                    } else {
                        pTemp = pTemp->left;    //不为空，往左下走
                    }
                //输入的数大于该结点，向右
                } else {
                    if(pTemp->right == nullptr) {
                        pTemp->right = newNode;
                        break;
                    } else {
                        pTemp = pTemp->right;
                    }
                }
            }
        }
        if(cin.get() == '\n')
            break;
    }
}


void PrintTree(BSTreeNode* root) {
    if(root != nullptr) {
        PrintTree(root->left);
        cout << root->val << endl;
        PrintTree(root->right);
    }
}

// int main() {
//     int value;
//     cin >> value;
//     auto root = new BSTreeNode(value);
//     CreatBSTree(root);
//     PrintTree(root);
// }
