#include<iostream>
#include <vector>

using namespace std;
template<typename T>
class MaxHeap {
private:
    T *data;
    int count;
    int capacity;
    void shiftDown(int k) {                                 //调整堆元素
        
        while(2 * k <= count) {                             //如果下标所对应的元素还有左孩子，那就肯定还有孩子
            int j = 2 * k;                                  //在此轮循环中，判断它是否需要和自己的孩子进行交换位置来调整
            if(j + 1 <= count && data[j + 1] > data[j])     //如果有右孩子而且右孩子的值比左孩子大，那就和右孩子进行交换
                j += 1;
            if(data[k] >= data[j])                          //如果比自己的孩子大，那就不需要进行交换
                break;
            swap(data[k], data[j]);                         //否则进行交换
            k = j;
        }
    }
public:
    MaxHeap(vector<T>& nums) {                              //初始化构建最大堆
        data = new T[nums.size() + 1];                      //初始化元素
        capacity = nums.size();
      	count = nums.size();
        for(int i = 0; i < nums.size(); ++i) 
            data[i + 1] = nums[i];
        for (int i = count / 2; i >= 1 ; --i)               //构建最大堆：将所有非叶子结点，从左往右，从上往下进行调整
            shiftDown(i);
    }
    T extractMax() {                                        //取出最大值的元素,然后进行重构
        assert(count > 0);                                  //保证数组的个数大于0
        T item = data[1];
        swap(data[1], data[count]);                         //保证数组的个数大于0
        --count;                                            //交换完成，最后一个元素不算在堆中
        shiftDown(1);                                       //交换完成，最后一个元素不算在堆中
        return item;
    }
    ~MaxHeap() {                                            //析构函数，释放内存
        delete[] data;
    }
};

template<typename T>
void HeapSort(vector<T>& nums) {
    MaxHeap<T> maxHeap = MaxHeap<T>(nums);                 //1.用无需数组创建最大堆
    for (int i = nums.size() - 1; i >= 0 ; --i)            //2.不断将堆顶元素与末尾元素进行交换，将最大元素"沉"到数组末尾
        nums[i] = maxHeap.extractMax();
    for (auto i : nums)                                    //3.输出结果
        cout << i << " ";
    cout << endl;
}

int main() {
	vector<int> nums{2,12,4,5,6,32,-1,34,56,0};
	HeapSort(nums);
	for (auto num : nums){
		cout << num << " ";
	}
	cout << endl;
	return 0;
}