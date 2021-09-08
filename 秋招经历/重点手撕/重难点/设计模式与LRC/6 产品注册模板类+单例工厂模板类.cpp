#include<iostream>
#include <map>
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

// 基类，产品注册模板接口类
// 模板参数 ProductType_t 表示的类是产品抽象类
template <class ProductType_t>
class IProductRegistrar
{
public:
   // 获取产品对象抽象接口
   virtual ProductType_t *CreateProduct() = 0;

protected:
   // 禁止外部构造和虚构, 子类的"内部"的其他函数可以调用
   IProductRegistrar() {}
   virtual ~IProductRegistrar() {}

private:
   // 禁止外部拷贝和赋值操作
   IProductRegistrar(const IProductRegistrar &);
   const IProductRegistrar &operator=(const IProductRegistrar &);
};

// 工厂模板类，用于获取和注册产品对象
template <class ProductType_t> // 模板参数 ProductType_t 表示的类是产品抽象类
class ProductFactory{
public:
    static ProductFactory<ProductType_t> &Instance(){ // 获取工厂单例，工厂的实例是唯一的
        static ProductFactory<ProductType_t> instance;
        return instance;
    }

    void RegisterProduct(IProductRegistrar<ProductType_t> *registrar, std::string name)  {// 产品注册
        m_ProductRegistry[name] = registrar;
    }

    ProductType_t *GetProduct(std::string name){// 根据名字name，获取对应具体的产品对象
   
    if (m_ProductRegistry.find(name) != m_ProductRegistry.end()){// 从map找到已经注册过的产品，并返回产品对象
        return m_ProductRegistry[name]->CreateProduct();
    }

    std::cout << "No product found for " << name << std::endl;// 未注册的产品，则报错未找到

    return nullptr;
   }

private:
   ProductFactory() {}// 禁止外部构造和虚构
   ~ProductFactory() {}
   ProductFactory(const ProductFactory &);
   const ProductFactory &operator=(const ProductFactory &);

   std::map<std::string, IProductRegistrar<ProductType_t> *> m_ProductRegistry;// 保存注册过的产品，key:产品名字 , value:产品类型
};

// 产品注册模板类，用于创建具体产品和从工厂里注册产品
// 模板参数 ProductType_t 表示的类是产品抽象类（基类），ProductImpl_t 表示的类是具体产品（产品种类的子类）
template <class ProductType_t, class ProductImpl_t>
class ProductRegistrar : public IProductRegistrar<ProductType_t>
{
public: 
   explicit ProductRegistrar(std::string name){ // 构造函数，用于注册产品到工厂，只能显示调用
      ProductFactory<ProductType_t>::Instance().RegisterProduct(this, name);// 通过工厂单例把产品注册到工厂
   }
  
   ProductType_t *CreateProduct() { // 创建具体产品对象指针
      return new ProductImpl_t();
   }
};

int main(){

   ProductRegistrar<Shoes, ErkeShoes> erkeShoes("erke");
   Shoes *pErkeShoes = ProductFactory<Shoes>::Instance().GetProduct("erke");
   pErkeShoes->Show();
   if (pErkeShoes) {
      delete pErkeShoes;
   }

   ProductRegistrar<Clothe, AntaClothe> adidasShoes("anta");
   Clothe *pAntaClothe = ProductFactory<Clothe>::Instance().GetProduct("anta");
   pAntaClothe->Show();
   if (pAntaClothe){
      delete pAntaClothe;
   }

   return 0;
}