#include<iostream>

//抽象产品类
class Shoes{                                
public:
    virtual ~Shoes() {}
    virtual void Show() = 0;
};

 //具体产品类
class ErkeShoes : public Shoes{           
public:
    void Show() {
        std::cout << "我是鸿星尔克球鞋，我的广告语：To be No.1" << std::endl;
    }
};
class AntaShoes : public Shoes{
public:
    void Show(){
        std::cout << "我是安踏球鞋，我的广告语:Keep Moving" << std::endl;
    }
};

//工厂类
enum SHOES_TYPE{
    Anta,
    Erke
};
class ShoesFactory{
public:
    Shoes *CreateShoes(SHOES_TYPE type){// 根据鞋子类型创建对应的鞋子对象
        switch (type){
        case Anta:
            return new AntaShoes();
            break;
        case Erke:
            return new ErkeShoes();
            break;
        default:
            return NULL;
            break;
        }
    }
};

int main(){
   
    ShoesFactory shoesFactory; // 构造工厂对象

    Shoes *pAntaShoes = shoesFactory.CreateShoes(Anta);
    if (pAntaShoes != nullptr){
        pAntaShoes->Show();
        delete pAntaShoes;
        pAntaShoes = nullptr;
    }
    Shoes *pErkeShoes = shoesFactory.CreateShoes(Erke);
    if (pErkeShoes != nullptr){
        pErkeShoes->Show();
        delete pErkeShoes;
        pErkeShoes = nullptr;
    }

    return 0;
}
