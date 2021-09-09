/**
 * 将待排序序列构造成一个大顶堆（父节点比子结点大），此时，整个序列的最小值就是堆顶的根节点。
 * 将其与末尾元素进行交换，此时末尾就为最大值。
 * 然后将剩余n-1个元素重新构造成一个堆，这样会得到n个元素的次小值。
 * 如此反复执行，便能得到一个有序序列了
 */

#include <iostream>
#include <vector>
using namespace std;
template<class T>
class MinHeap{
private:
    T* data;
    int count;
    int capacity;
    
    //调整元素
    void ShiftDown(int k) {
        while(2 * k <= count) {                             //如果下标所对应的元素还有左孩子，那就肯定还有孩子
            int j = 2 * k;                                  //在此循环中判断是否需要与自己的孩子进行交换位置来调整
            if(j + 1 <= count && data[j + 1] < data[j])     //如果有右孩子，且右孩子比自己小，则让右孩子进行调整   
                j += 1;
            if(data[k] <= data[j])
                break;                                       //如果比自己的孩子小，则不需要交换
            swap(data[k], data[j]);                          //否则就进行交换
            k = j;                                           //接着往下调整
        }
    }
public:
    //构造函数，创建最小堆
    MinHeap(vector<T>& nums) {
        data = new T[nums.size() + 1];  //从 1 开始
        count = nums.size();
        capacity = nums.size();
        for(int i = 0; i < nums.size(); ++i) 
            data[i + 1] = nums[i];
        for(int i = count / 2; i >= 1; --i) //构建最小堆，从所有非叶子结点开始，从左往右，从上到下进行调整
            ShiftDown(i);
    }
    //取出最小根，并重新构建最小堆
    T extractMin() {
        assert(count > 0);                                    //保证需要调整的个数大于0
        T temp = data[1];
        swap(data[1], data[count]);                           //将第一个元素与调整数组中的最后一个元素交换
        -- count;
        ShiftDown(1);                                         //将第一个元素下移
        return temp;
    }

};

template<typename T>
void HeapSort(vector<T>& nums) {
    MinHeap<T> minHeap = MinHeap<T>(nums);          //1.用无需数组创建最大堆
    for (int i = nums.size() - 1; i >= 0 ; --i)     //2.不断将堆顶元素与末尾元素进行交换，将最大元素"沉"到数组末尾
        nums[i] = minHeap.extractMin();
}

int main() {
    vector<int> nums{2,12,4,5,6,32,-1,34,56,0};
    HeapSort(nums);
    for(auto i : nums)
        cout << i << " ";
    cout << endl;
}
