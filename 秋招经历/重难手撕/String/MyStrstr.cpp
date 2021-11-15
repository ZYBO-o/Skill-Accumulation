/*
 * Created by Chimomo
 *
 * 函数名：strStr
 * 功能：找出字符串str2在字符串str1中第一次出现的位置（不包括str2的串结束符）。
 * 返回值：若找到，返回指向该位置的指针；否则，返回空指针。
 */
 
#include <iostream>
 
#define NULL 0
 
using namespace std;
 
char *strStr(const char *str1, const char *str2) {
    if (*str2) {
        while (*str1) {
            for (int i = 0; *(str1 + i) == *(str2 + i); i++) {
                if (!*(str2 + i + 1)) {
                    return (char *) str1;
                }
            }
            str1++;
        }
        return NULL;
    } else {
        return (char *) str1;
    }
}
 
int main() {
    char str1[20] = "453456789123456789";
    char str2[6] = "34567";
    char *str = strStr(str1, str2);
    cout << str << endl;
    return 0;
}
 
// Output:
/*
3456789123456789
*/