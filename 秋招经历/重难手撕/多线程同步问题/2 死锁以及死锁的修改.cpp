#include <iostream>
#include <thread>
#include <mutex>

void work1(std::mutex& mylock1, std::mutex& mylock2) {
	for (int i = 0; i < 100000; i++) {
		mylock1.lock();
		mylock2.lock();
		std::cout << "work1 : " << i << std::endl;
		mylock2.unlock();
		mylock1.unlock();
	}
}

void work2(std::mutex& mylock1, std::mutex& mylock2) {
	for (int i = 0; i < 100000; i++) {
        /*
            std::lock(mylock1, mylock2);
		    std::lock_guard<std::mutex> lock1(mylock1, std::adopt_lock);
		    std::lock_guard<std::mutex> lock2(mylock2, std::adopt_lock);
            std::cout << "work2 : " << i << std::endl;
        */
		mylock2.lock();
		mylock1.lock();
		std::cout << "work2 : " << i << std::endl;
		mylock1.unlock();
		mylock2.unlock();
	}
}

int main(){
	std::mutex mylock1, mylock2;
	std::thread t1(work1, std::ref(mylock1), std::ref(mylock2));
	std::thread t2(work2, std::ref(mylock1), std::ref(mylock2));
	t1.join();
	t2.join();
	return 0;
}