//
// Created by 张永斌 on 2021/8/4.
//
#include <iostream>
using namespace std;
class Test
{
public:
    Test(string s) : str(s){
        cout<<"Test creat\n";
    }

    ~Test(){
        cout<<"Test delete:"<<str<<endl;
    }

    string& getStr(){
        return str;
    }

    void setStr(string s){
        str = s;
    }

    void print(){
        cout<<str<<endl;
    }
private:
    string str;
};

void UseSharePointer()
{
    shared_ptr<Test> ptest(new Test("123"));
    shared_ptr<Test> ptest2(new Test("456"));

    cout<<ptest2->getStr()<<endl;
    cout<<ptest2.use_count()<<endl;

    ptest = ptest2;//"456"引用次数加1，“123”销毁

    ptest->print();

    cout<<ptest2.use_count()<<endl;//2
    cout<<ptest.use_count()<<endl;//2

    ptest.reset();
    ptest2.reset();//此时“456”销毁

    cout<<"done !\n";

}