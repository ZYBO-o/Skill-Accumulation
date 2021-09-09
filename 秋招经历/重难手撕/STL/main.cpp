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
    std::cout << (str1 == str2) << std::endl;
    std::cout << (str1 != str2) << std::endl;
    std::cout << str3[0] << std::endl;
    str3[0] = 'H';
    std::cout << str3 << std::endl;
    return 0;
}