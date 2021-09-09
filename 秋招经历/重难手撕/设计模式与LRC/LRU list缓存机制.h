#include<unordered_map>
#include <iostream>
#include <list>
using namespace std;
//结构还是双链表结构，只是利用了unordered_map，查找速度更快
class LRUCache {
public:
    list<pair<int, int>> lst;
    unordered_map<int, list<pair<int, int>>::iterator> mem;  // <key, list::iterator>  迭代器形式，实现O(1)
    int cap = 0;

    LRUCache(int capacity):cap(capacity){
        mem.clear();
        lst.clear();
    }
    
    int get(int key) {
        if (mem.find(key) != mem.end()) {
            int value = mem[key]->second; 
            lst.erase(mem[key]);    // 注意别先删后调用
            lst.push_front({key, value}); //更新到最前面
            mem[key] = lst.begin();
            return value;
        }
        else return -1;
    }
    
    void put(int key, int value) {

        bool flag_find = mem.find(key) == mem.end();

        if (flag_find && mem.size() == cap) { // 新元素 且 需要满了，删除last
            mem.erase(lst.back().first);
            lst.pop_back();
            lst.push_front({key, value});
            mem[key] = lst.begin();
        }
        else if (flag_find){// 新元素 但 不需要删除
            lst.push_front({key, value});
            mem[key] = lst.begin();
         }
         else{   // 旧元素
            lst.erase(mem[key]);//删除之前的
            lst.push_front({key, value});
            mem[key] = lst.begin();
        }
    }
};
