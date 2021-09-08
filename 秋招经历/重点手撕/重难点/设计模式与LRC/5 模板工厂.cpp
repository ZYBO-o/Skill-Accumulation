#include<iostream>

//抽象产品类
class Shoes{                                
public:
    virtual ~Shoes() {}
    virtual void Show() = 0;
};
class Clothe{  
public:
    virtual void Show() = 0;
    virtual ~Clothe() {}
};

 //具体产品类
class ErkeShoes : public Shoes{           
public:
    void Show() {
        std::cout << "我是鸿星尔克球鞋，我的广告语：To be No.1" << std::endl;
    }
};
class AntaClothe : public Clothe{           
public:
    void Show() {
        std::cout << "我是安踏衣服，我的广告语:Keep Moving" << std::endl;
    }
};

// 抽象模板工厂类
// 模板参数：AbstractProduct_t 产品抽象类
template <class AbstractProduct_t>
class AbstractFactory{
public:
    virtual AbstractProduct_t *CreateProduct() = 0;
    virtual ~AbstractFactory() {}
};

// 具体模板工厂类
template <class AbstractProduct_t, class ConcreteProduct_t>
class ConcreteFactory : public AbstractFactory<AbstractProduct_t>{
public:
    AbstractProduct_t *CreateProduct(){
        return new ConcreteProduct_t();
    }
};

int main(){

    ConcreteFactory<Shoes, ErkeShoes> erkeFactory;
    Shoes *pErkeShoes = erkeFactory.CreateProduct();
    pErkeShoes->Show();

    ConcreteFactory<Clothe, AntaClothe> antaFactory;
    Clothe *pAntaClothe = antaFactory.CreateProduct();
    pAntaClothe->Show();

    delete pErkeShoes;
    pErkeShoes = nullptr;
    delete pAntaClothe;
    pAntaClothe = nullptr;

    return 0;
}