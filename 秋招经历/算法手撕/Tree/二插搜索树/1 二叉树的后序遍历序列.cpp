#include "BSTree.h"
#include<queue>
#include<iostream>
#include<algorithm>
#include<vector>

/**
 *                  9
 *         4                12
 *     3        5       10
 *    [3，5，4，10，12，9]
 */
//后续遍历的最后一个数字一定是根节点，所以数组中最后一个数字9就是根节点
//从前往后找到第一个比9大的数字10，那么10后面的[10，12]（除了9）都是9的右子节点，10前面的[3，5，4]都是9的左子节点，
//后面的需要判断一下，如果有小于9的，说明不是二叉搜索树，直接返回false。然后再以递归的方式判断左右子树。
//然后进行递归


class Solution {
public:
    bool verifyPostorder(vector<int>& postorder) {
        return traversal(postorder,0,postorder.size()-1);   //区间左闭右闭
    }

private:
    bool traversal(vector<int>& postorder,int left,int right){
        // 1.  确认递归返回条件
        if(left >= right) return true;
      
        // 2. 查询第一个大于root节点数值的索引
        int mid = left;
        int root = postorder[right];//根结点找到
        while(postorder[mid] < root)  
            mid++;

        // 3. 确认mid右侧数值始终大于root节点
        int temp = mid;
        while(temp < right){
            if(postorder[temp++] < root)
                return false;
        }

        // 4. 递归查询[left,mid-1]、[mid,right-1] 是否同样满足条件
        return traversal(postorder,left,mid-1) && traversal(postorder,mid,right-1);
    }
};