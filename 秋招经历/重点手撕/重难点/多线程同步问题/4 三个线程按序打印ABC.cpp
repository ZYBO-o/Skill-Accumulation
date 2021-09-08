#include <thread>
#include <condition_variable>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

std::mutex mtx;
std::condition_variable cond;
char g_ch = 0;

void print_fun(char ch){
	int count = 10;
	char ch_ = ch - 'A';

	for (int i = 0; i < count; i++){
		unique_lock<mutex>lock(mtx);
	
		cond.wait(lock, [ch_] {return ch_ == g_ch; });//条件变量锁起来
		cout << (char)(ch_ + 'A') << endl;;
		g_ch = (ch_ + 1) % 3;

		cout << "pthread =  " <<  this_thread::get_id() << endl;
		this_thread::sleep_for(chrono::seconds(1));
		
		lock.unlock();
		cond.notify_all();
	}
}

int main(){
	std::vector<std::thread> threads;
	threads.push_back(std::thread(print_fun, 'A'));
	threads.push_back(std::thread(print_fun, 'B'));
	threads.push_back(std::thread(print_fun, 'C'));
	
	//std::for_each(threads.begin(), threads.end(), mem_fn(&std::thread::join));
	//每个线程调用join函数
	for(decltype(threads.size()) i = 0; i < threads.size(); ++i) {
		threads[i].thread::join();
	}

	system("pause");
	return 0;
}