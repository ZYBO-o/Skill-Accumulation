#include<iostream>

//抽象产品类
class Shoes{                                
public:
    virtual ~Shoes() {}
    virtual void Show() = 0;
};
class Clothe{  // 基类 衣服
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
class ErkeClothe : public Clothe{           
public:
    void Show() {
        std::cout << "我是鸿星尔克衣服，我是新增产品" << std::endl;
    }
};

// 抽象工厂
class Factory{
public:
    virtual Shoes *CreateShoes() = 0;
	virtual Clothe *CreateClothe() = 0;
    virtual ~Factory() {}
};

//具体工厂
class ErkeProducer : public Factory{
public:
    Shoes *CreateShoes(){
        return new ErkeShoes();
    }
	Clothe *CreateClothe(){
        return new ErkeClothe();
    }
};

int main(){
 
    Factory *erkeProducer = new ErkeProducer();
    
    Shoes *erkeShoes = erkeProducer->CreateShoes();
    Clothe *erkeClothe = erkeProducer->CreateClothe();
    erkeShoes->Show();
    erkeClothe->Show();
	
    delete erkeShoes;
	delete erkeClothe;
    delete erkeProducer;

    return 0;
}