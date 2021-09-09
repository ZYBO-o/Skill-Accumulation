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

// 抽象工厂
class ShoesFactory{
public:
    virtual Shoes *CreateShoes() = 0;
    virtual ~ShoesFactory() {}
};

// 具体工厂生产
class AntaProducer : public ShoesFactory{
public:
    Shoes *CreateShoes(){
        return new AntaShoes();
    }
};
class ErkeProducer : public ShoesFactory{
public:
    Shoes *CreateShoes(){
        return new ErkeShoes();
    }
};

int main(){
    // ================ Anta流程 ==================== //
    ShoesFactory *antaProducer = new AntaProducer();
    Shoes *antaShoes = antaProducer->CreateShoes();
    antaShoes->Show();
    delete antaShoes;
    delete antaProducer;
    // ================ Erke流程 ==================== //
    ShoesFactory *erkeProducer = new ErkeProducer();
    Shoes *erkeShoes = erkeProducer->CreateShoes();
    erkeShoes->Show();
    delete erkeShoes;
    delete erkeProducer;

    return 0;
}