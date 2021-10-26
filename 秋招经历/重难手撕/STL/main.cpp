#include <iostream>
#include "String.h"

int main() {
	String str1("hello");
    String str2;
    std::cin >> str2;
    std::cout << str2.size() << std::endl;
    String str3 = str1 + str2;
    std::cout << str1 << std::endl;
    std::cout << str2 << std::endl;
    std::cout << str3 << std::endl;
    String str4(std::move(str3));
    std::cout << str4 << std::endl;
    return 0;
}