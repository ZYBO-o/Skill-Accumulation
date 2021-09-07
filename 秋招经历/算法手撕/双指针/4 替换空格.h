#include <string>
using namespace std;

string replaceSpace(string s) {
    //统计空格的个数
    int spacecount = 0;
    for(auto temp : s) 
        if(temp == ' ')
            ++spacecount;
    //原来字符串的个数
    int sptr = s.size() - 1;
    //空充后字符串的个数
    s.resize(s.size() + 2 * spacecount);
    int resultptr = s.size() - 1;
    //填充字符串
    while(sptr >= 0 && resultptr >= 0) {
        if(s[sptr] == ' ') {
            s[resultptr] = '0';
            s[--resultptr] = '2';
            s[--resultptr] = '%';
            --sptr;
            --resultptr;
        } else {
            s[resultptr] = s[sptr];
            --sptr;
            --resultptr;
        }
    }
    return s;
}



