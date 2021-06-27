<!-- GFM-TOC -->



<!-- GFM-TOC -->

---

# C++ Primer

##  一.变量与基本类型

### 1.变量声明和定义区别？

- 声明仅仅是把变量的声明的位置及类型提供给编译器，并不分配内存空间；定义要在定义的地方为其分配存储空间。
- 相同变量可以在多处声明（外部变量extern），但只能在一处定义。

---

### 2.哪几种情况必须用到初始化成员列表？

- 初始化一个const成员。
- 初始化一个reference成员。
- 调用一个基类的构造函数，而该函数有一组参数。
- 调用一个数据成员对象的构造函数，而该函数有一组参数。

---

### 3.常量指针和指针常量区别？

- 常量指针是一个指针，读成常量的指针，指向一个只读变量。如`int const *p`或`const int *p`。
- 指针常量是一个不能给改变指向的指针。指针是个常量，不能中途改变指向，如`int *const p`。

---

### 4.野指针和悬空指针

都是是指向无效内存区域(这里的无效指的是"不安全不可控")的指针，访问行为将会导致未定义行为。

**野指针**，指的是没有被初始化过的指针

```c++
int main(void) { 
  int * p;
  std::cout<<*p<<std::endl;
  return 0;
}
```

因此，为了防止出错，对于指针初始化时都是赋值为 `nullptr`，这样在使用时编译器就会直接报错，产生非法内存访问。

**悬空指针**，指针最初指向的内存已经被释放了的一种指针。

```c++
int main(void) { 
  int * p = nullptr;
  int* p2 = new int; 
  p = p2;
  delete p2;
}
```

此时 p和p2就是悬空指针，指向的内存已经被释放。继续使用这两个指针，行为不可预料。需要设置为`p=p2=nullptr`。此时再使用，编译器会直接保错。

避免野指针比较简单，但悬空指针比较麻烦。c++引入了智能指针，C++智能指针的本质就是避免悬空指针的产生。

**产生原因及解决办法：**

野指针：指针变量未及时初始化 => 定义指针变量及时初始化，要么置空。

悬空指针：指针free或delete之后没有及时置空 => 释放操作后立即置空。

---

### 5.区别以下指针类型？

```c++
int *p[10]
int (*p)[10]
int *p(int)
int (*p)(int)
```

- `int *p[10]`表示指针数组， **强调数组概念** ，是一个数组变量，数组大小为10，数组内每个元素都是指向int类型的指针变量。
- `int (*p)[10]`表示数组指针， **强调是指针** ，只有一个变量，是指针类型，不过指向的是一个int类型的数组，这个数组大小是10。
- `int *p(int)`是函数声明，函数名是p，参数是int类型的，返回值是int *类型的。
- `int (*p)(int)`是函数指针，强调是指针，该指针指向的函数具有int类型参数，并且返回值是int类型的。

---

### 6.指针和引用的区别

#### :large_orange_diamond: 性质方面

- 指针是一个变量，存储的是一个地址，引用跟原来的变量实质上是同一个东西，是原变量的别名
- 指针可以有 **多级** ，引用只有一级
- 指针可以为空，引用不能为`NULL`且在定义时 **必须初始化**
- 指针在初始化后 **可以改变指向** ，而引用在初始化之后 **不可再改变**

#### :large_orange_diamond: ​内存

- `sizeof`指针得到的是本指针的大小，`sizeof`引用得到的是引用所指向变量的大小

- 引用只是别名，不占用具体存储空间，只有声明没有定义；指针是具体变量，需要占用存储空间。

#### :large_orange_diamond: ​使用

+ 引用一旦初始化之后就不可以再改变（变量可以被引用为多次，但引用只能作为一个变量引用）；指针变量可以重新指向别的变量。

+ 引用在声明时必须初始化为另一变量，一旦出现必须为`typename refname &varname`形式；指针声明和定义可以分开，可以先只声明指针变量而不初始化，等用到时再指向具体变量。

+ **当把指针作为参数进行传递时，也是<font color = red>将实参的一个拷贝传递给形参，两者指向的地址相同，但不是同一个变量，在函数中改变这个变量的指向不影响实参，而引用却可以。</font>**

```c++
void test(int *p)
{
　　int a=1;
　　p=&a;
　　cout<<p<<" "<<*p<<endl;
}

int main(void)
{
    int *p=NULL;
    test(p);
    if(p==NULL)
    	cout<<"指针p为NULL"<<endl;
    return 0;
}
//运行结果为：
//0x22ff44 1
//指针p为NULL
//*******************************	
void testPTR(int* p) {
    int a = 12;
    p = &a;

}

void testREFF(int& p) {
    int a = 12;
    p = a;

}
void main()
{
    int a = 10;
    int* b = &a;
    testPTR(b);//改变指针指向，但是没改变指针的所指的内容
    cout << a << endl;// 10
    cout << *b << endl;// 10

    a = 10;
    testREFF(a);
    cout << a << endl;//12
}
```

---

### 7.C++中const和static的作用

#### **static**

**:diamonds:  不考虑类的情况**

:small_blue_diamond: **存储区：**

+ 静态变量都在全局数据区分配内存，包括静态局部变量。

  > 一般程序的由new产生的动态数据存放在堆区，函数内部的自动变量存放在栈区。自动变量一般会随着函数的退出而释放空间，静态数据（即使是函数内部的静态局部变量）也存放在全局数据区。全局数据区的数据并不会因为函数的退出而释放空间。

:small_blue_diamond: **初始化：**

+ 全局static变量的初始化在编译的时候进行。在main函数被调用之前初始化并且只初始化一次 。
+ 默认初始化为0，包括未初始化的全局静态变量与局部静态变量，都存在 **全局未初始化区**

:small_blue_diamond: **作用域：**

+ 静态变量在函数内定义，始终存在，且只进行一次初始化，具有记忆性，其作用范围与局部变量相同，函数退出后仍然存在，但不能使用

  ```c++
  int main(void)
  {    
      for(int i = 0;i < 5;i++)
      {
        	static int m = 7;
       	 	cout<< "m=" << m--;
      } 
      cout << m << endl;//这里会显示错误
          return 0;
  ```

+  普通全局变量作用域是整个源程序，当一个程序有多个源文件组成时候，普通全局变量在每个源文件中都是有效的，而 **静态全局变量则限制了作用域，只在定义该变量的源文件中有效。在同一个源程序中的其他源文件不能使用。** 由于静态全局变量的作用域限于一个源文件内，只能为该源文件内的函数公用， 因此可以避免在其它源文件中引起错误。

  

:diamonds:  **考虑类的情况**

:small_blue_diamond:  **static成员变量** 

```c++

#include<iostream>
using namespace std;
 
class Myclass
{
private:
    int a , b , c;
    static int sum;  //声明静态数据成员
public:
    Myclass(int a , int b , int c) : a(a), b(b), c(c){
        sum += a+b+c;
    }
    void GetSum();
};
 
int Myclass::sum = 0;   //定义并初始化静态数据成员

void Myclass::GetSum() {
		cout<<"sum="<<sum<<endl;
}

int main(void)
{
    Myclass M(1 , 2 , 3);
    M.GetSum();						//6
    Myclass N(4 , 5 , 6);
    N.GetSum();						//21
    M.GetSum();						//21
    return 0;
}
```

+ 对于非静态数据成员，每个类对象都有自己的拷贝。而静态数据成员被当作是类的成员。无论这个类的对象被定义了多少个，静态数据成员在程序中也只有一份拷贝，由该类型的所有对象共享访问。也就是说，静态数据成员是该类的所有对象所共有的。对该类的多个对象来说，静态数据成员只分配一次内存(定义时就分配)，供所有对象共用。所以，静态数据成员的值对每个对象都是一样的，它的值可以更新；
+ 静态数据成员存储在全局数据区。静态数据成员定义时要分配空间，所以不能在类声明中定义。在Example中，语句`int Myclass::Sum=0;`是定义静态数据成员；
+ 因为静态数据成员在全局数据区分配内存，属于本类的所有对象共享，所以，它不属于特定的类对象，在没有产生类对象时其作用域就可见，即在没有产生类的实例时，我们就可以操作它；
+ 同全局变量相比，使用静态数据成员有两个优势：
  + 静态数据成员没有进入程序的全局名字空间，因此不存在与程序中其它全局名字冲突的可能性；
  + 可以实现信息隐藏。静态数据成员可以是private成员，而全局变量不能；

:small_blue_diamond:  **static成员函数**

```c
#include<iostream>
using namespace std;
 
class Myclass
{
private:
	int a , b , c;
	static int sum;  //声明静态数据成员
public:
	Myclass(int a , int b , int c) : a(a), b(b), c(c) {
		this->a = a;
		this->b = b;
		this->c = c;
		sum += a+b+c;    //非静态成员函数可以访问静态数据成员
	}
	static void GetSum();  //声明静态成员函数
};
 
int Myclass::sum = 0;   //定义并初始化静态数据成员

void Myclass::GetSum(){    //静态成员函数的实现

	//cout<<a<<endl;    //错误代码，a是非静态数据成员
	cout<<"sum="<<sum<<endl;
}
 
int main(void)
{
	Myclass M(1 , 2 , 3);
	M.GetSum();							//6
	Myclass N(4 , 5 , 6);
	N.GetSum();							//21
	Myclass::GetSum();			//21
	return 0;
}
```



+ 与静态数据成员一样，我们也可以创建一个静态成员函数，它为类的全部服务而不是为某一个类的具体对象服务。静态成员函数与静态数据成员一样，都是类的内部实现，属于类定义的一部分。
+ 普通的成员函数一般都隐含了一个this指针，this指针指向类的对象本身，因为普通成员函数总是具体的属于某个类的具体对象的。通常情况下，this是缺省的。如函数fn()实际上是this->fn()。 **但是与普通函数相比，静态成员函数由于不是与任何的对象相联系，因此它不具有this指针。从这个意义上讲，它无法访问属于类对象的非静态数据成员，也无法访问非静态成员函数，它只能调用其余的静态成员函数。**

+ 出现在类体外的函数定义不能指定关键字static；
+ 静态成员之间可以相互访问，包括静态成员函数访问静态数据成员和访问静态成员函数；
+ 非静态成员函数可以任意地访问静态成员函数和静态数据成员；
+ 静态成员函数不能访问非静态成员函数和非静态数据成员；
+ 由于没有this指针的额外开销，因此静态成员函数与类的全局函数相比速度上会有少许的增长；
+ 调用静态成员函数，可以用成员访问操作符(.)和(->)为一个类的对象或指向类对象的指针调用静态成员函数，也可以直接使用如下格式：
  `＜类名＞::＜静态成员函数名＞（＜参数表＞）`
  调用类的静态成员函数。



#### **const**

:diamonds:   **不考虑类的情况**

- const常量在定义时必须初始化，之后无法更改

- const形参可以接收const和非const类型的实参，例如

  ```c++
  // i 可以是 int 型或者 const int 型
  void fun(const int& i){
      //...
  }
  ```

:diamonds:   **不考虑类的情况**

:small_blue_diamond:  **const成员变量**

- 不能在类定义外部初始化，只能通过构造函数初始化列表进行初始化
- 并且必须有构造函数；
- 不同类对其const数据成员的值可以不同，所以不能在类中声明时初始化

:small_blue_diamond: **const成员函数**

+ const对象不可以调用非const成员函数；
+ 非const对象都可以调用；
+ 不可以改变非mutable（用该关键字声明的变量可以在const成员函数中被修改）数据的值

---

### 8.C++的顶层const和底层const

#### **概念区分**

>  **指针本身是不是一个常量** 以及 **指针所指的是不是一个常量** 是两个独立的问题。

- **顶层**const：指的是 **指针本身是一个常量**
- **底层**const：指的是 **指针所指的对象是一个常量**

#### **举个例子**

```c++
int i= 0；
int* const p1 = &i; 				//不能改变p1的值，顶层const
const int ci = 42;					//不能改变ci的值，顶层const
const int *p2 = &ci; 				//允许改变P2的值，底层const
const int *const p3 = p2;		//靠右的const是顶层const，靠左的const是底层const
const int &r = ci;					//用于声明引用的const都是底层const
```

#### **区分作用**

- 执行对象拷贝时有限制，常量的底层const不能赋值给非常量的底层const
- 使用命名的强制类型转换函数const_cast时，只能改变运算对象的底层const

```c++
const int a;
int const a;
const int *a;
int *const a;
```

- `int const a`和`const int a`均表示定义常量类型a。
- `const int *a`，其中a为指向int型变量的指针，const在 * 左侧，表示a指向不可变常量。(看成`const (*a)`，对引用加const)
- `int *const a`，依旧是指针类型，表示a为指向整型数据的常指针。(看成`const(a)`，对指针const)

---

### 9.constexpr与常量表达式

#### (1).常量表达式

**常量表达式** 是指 **值不会改变** 并且在 **编译过程就能得到计算结果** 的表达式。一个对象（或表达式）是不是常量表达式由它的数据类型和初始值共同决定。

```c++
const int max_files = 20;			// 是常量表达式
const int limit = max_files + 1;	// 是常量表达式
int staff_size = 2;					// 不是常量表达式
//sz本身是一个常量，但是具体值直到运行时才能获取，所以不是常量表达式
const int sz = get_size();			// 不是常量表达式
```

#### (2).constexpr变量

+ C++允许将 **变量声明为constexpr类型以便由编译器来验证变量的值是否是一个常量表达式**
+  **声明为constexpr的变量一定是一个常量，而且必须用常量表达式初始化**
  + 尽管不能使用普通函数作为constexpr变量的初始值，但是 **新标准允许定义一种特殊的 constexpr 函数** 。这种函数应该足够简单以使得编译时就可以计算其结果，这样就能用constexpr函数去初始化constexpr变量了

#### (3).constexpr与指针

+ 一个`constexpr`指针的初始值必须是`nullptr`或者`0`,或者是存储于某个固定地址中的对象；
+ `constexpr`引用必须绑定在存储于某个固定地址中的对象

> - 例如，函数体内定义的变量一般来说并非存放在固定地址中， 因此`constexpr`指针不能指向这样的变量。
> - 相反，**全局变量 以及 局部静态变量地址固定不变，能用来初始化`constexpr`指针**

在constexpr声明中如果定义了一个指针， **限定符constexpr仅对指针有效，与指针所指的对象无关。**

```c++
const int *p = nullptr;
constexpr int *q = nullptr;
```

+ p是一个指向常量的指针

+ q是一个常量指针。

  > constexpr把它所定义的对象置为了顶层const

---

### 10.auto、decltype和decltype(auto)的用法

**（1）auto**

C++11新标准引入了auto类型说明符，用它就能让编译器替我们去分析表达式所属的类型。和原来那些只对应某种特定的类型说明符(例如 int)不同，

auto 让编译器通过初始值来进行类型推演。从而获得定义变量的类型，所以说 auto 定义的变量必须有初始值。举个例子：

```c++
 //普通；类型
 int a = 1, b = 3;
 auto c = a + b;// c为int型
 
 //const类型
 const int i = 5;
 auto j = i; // 变量i是顶层const, 会被忽略, 所以j的类型是int
 auto k = &i; // 变量i是一个常量, 对常量取地址是一种底层const, 所以b的类型是const int*
 const auto l = i; //如果希望推断出的类型是顶层const的, 那么就需要在auto前面加上cosnt
 
 //引用和指针类型
 int x = 2;
 int& y = x;
 auto z = y; //z是int型不是int& 型
 auto& p1 = y; //p1是int&型
 auto p2 = &x; //p2是指针类型int*
```

**（2）decltype**

有的时候我们还会遇到这种情况， **希望从表达式中推断出要定义变量的类型，但却不想用表达式的值去初始化变量。还有可能是函数的返回类型为某表达式的值类型。** 在这些时候auto显得就无力了，所以C++11又引入了第二种类型说明符decltype，它的作用是选择并返回操作数的数据类型。在此过程中，编译器只是分析表达式并得到它的类型，却不进行实际的计算表达式的值。

```c++
 int func() {return 0};
 
 //普通类型
 decltype(func()) sum = 5; // sum的类型是函数func()的返回值的类型int, 但是这时不会实际调用函数func()
 int a = 0;
 decltype(a) b = 4; // a的类型是int, 所以b的类型也是int
 
 //不论是顶层const还是底层const, decltype都会保留   
 const int c = 3;
 decltype(c) d = c; // d的类型和c是一样的, 都是顶层const
 int e = 4;
 const int* f = &e; // f是底层const
 decltype(f) g = f; // g也是底层const
 
 //引用与指针类型
 //1. 如果表达式是引用类型, 那么decltype的类型也是引用
 const int i = 3, &j = i;
 decltype(j) k = 5; // k的类型是 const int&
 
 //2. 如果表达式是引用类型, 但是想要得到这个引用所指向的类型, 需要修改表达式:
 int i = 3, &r = i;
 decltype(r + 0) t = 5; // 此时是int类型
 
 //3. 对指针的解引用操作返回的是引用类型
 int i = 3, j = 6, *p = &i;
 decltype(*p) c = j; // c是int&类型, c和j绑定在一起
 
 //4. 如果一个表达式的类型不是引用, 但是我们需要推断出引用, 那么可以加上一对括号, 就变成了引用类型了
 int i = 3;
 decltype((i)) j = i; // 此时j的类型是int&类型, j和i绑定在了一起
```

**（3）decltype(auto)**

decltype(auto)是C++14新增的类型指示符，可以用来声明变量以及指示函数返回类型。在使用时，会将“=”号左边的表达式替换掉auto，再根据decltype的语法规则来确定类型。举个例子：

```c++
 int e = 4;
 const int* f = &e; // f是底层const
 decltype(auto) j = f;//j的类型是const int* 并且指向的是e
```

> 《auto和decltype的用法总结》：https://www.cnblogs.com/XiangfeiAi/p/4451904.html
>
> 《C++11新特性中auto 和 decltype 区别和联系》：https://www.jb51.net/article/103666.htm



## 二.字符串，向量和数组

### 11.a和&a有什么区别？

```c++
//假设数组int a[10];
int a[10] = {1,2,3,4,5,6,7,8,9,10};
int (*p)[10] = &a;
//int (*p)[10] = a;会报错
cout << *(*p + 1)<< endl;//2
cout << *(a + 1) << endl;//2
```

- a是数组名，是数组首元素地址，+1表示地址值加上一个int类型的大小，如果a的值是0x00000001，加1操作后变为0x00000005。*(a + 1) = a[1]。
- &a是数组的指针，其类型为int (*)[10]（就是前面提到的数组指针），其加1时，系统会认为是数组首地址加上整个数组的偏移（10个int型变量），值为数组a尾元素后一个元素的地址。
- 若(int *)p ，此时输出 *p时，其值为a[0]的值，因为被转为int *类型，解引用时按照int类型大小来读取。

---

### 12.数组名和指针（这里为指向数组首元素的指针）区别？

- 二者均可通过增减偏移量来访问数组中的元素。
- 数组名不是真正意义上的指针，可以理解为常指针，所以数组名没有自增、自减等操作。
- 当数组名当做形参传递给调用函数后，就失去了原有特性，退化成一般指针，多了自增、自减操作，但sizeof运算符不能再得到原数组的大小了。

---

### 13.拷贝初始化和直接初始化

+ 当用于类类型对象时，初始化的拷贝形式和直接形式有所不同：直接初始化直接调用与实参匹配的构造函数，拷贝初始化总是调用拷贝构造函数。拷贝初始化首先使用指定构造函数创建一个临时对象，然后用拷贝构造函数将那个临时对象拷贝到正在创建的对象。举例如下

```c++
string str1("I am a string");//语句1 直接初始化
string str2(str1);//语句2 直接初始化，str1是已经存在的对象，直接调用构造函数对str2进行初始化
string str3 = "I am a string";//语句3 拷贝初始化，先为字符串”I am a string“创建临时对象，再把临时对象作为参数，使用拷贝构造函数构造str3
string str4 = str1;//语句4 拷贝初始化，这里相当于隐式调用拷贝构造函数，而不是调用赋值运算符函数
```

- **为了提高效率，允许编译器跳过创建临时对象这一步，**直接调用构造函数构造要创建的对象，这样就完全等价于**直接初始化了**（语句1和语句3等价）。但是需要辨别两种情况。
- 当拷贝构造函数为private时：语句3和语句4在编译时会报错
- 使用explicit修饰构造函数时：如果构造函数存在隐式转换，编译时会报错

---

### 14.strlen和sizeof区别？

- sizeof是运算符，并不是函数，结果在编译时得到而非运行中获得；strlen是字符处理的库函数。
- sizeof参数可以是任何数据的类型或者数据（sizeof参数不退化）；strlen的参数只能是字符指针且结尾是'\0'的字符串。
- 因为sizeof值在编译时确定，所以不能用来得到动态分配（运行时分配）存储空间的大小。

```c++
int main(int argc, char const *argv[]){   
      const char* str = "name";
      sizeof(str); // 取的是指针str的长度，是8(64位为8字节，32位还是4字节，int*无论是32位还是64位都是4个字节)
      strlen(str); // 取的是这个字符串的长度，不包含结尾的 \0。大小是4
      return 0;
 }
```

---

## 三.函数

### 15宏定义和函数有何区别？

- 宏在编译时完成替换，之后被替换的文本参与编译，相当于直接插入了代码，运行时不存在函数调用，执行起来更快；函数调用在运行时需要跳转到具体调用函数。
- 宏定义属于在结构中插入代码，没有返回值；函数调用具有返回值。
- 宏定义参数没有类型，不进行类型检查；函数参数具有类型，需要检查类型。
- 宏定义不要在最后加分号。

---

### 16.数组形参需要注意的问题

由于 不允许拷贝数组 和 使用数组时通常会将其转换为指针的性质，函数传递数组时，实际上传递的是指向数组首元素的指针。不过可以写成类似数组的形式。

```c++
//这三个函数等价
void print(const int *);
void print(const int[]);
void print(const int[10]);
```

+ **对于数组引用形参**

  + 函数形参可以是数组的引用，而数组的维度也是类型的一部分

    ```c++
    void print(int (&arr)[10]) {
      	for(auto i : arr)
          cout << i << endl;
    }
    ```

    > &arr两端的括号必不可少。
    >
    > ```c++
    > f(int (&arr)[10]) ;	//arr是具有10个整数的整形数组的引用
    > f(int &arr[10]) ;		//将arr声明成引用的数组
    > ```
    >
    > 但是对于上述函数，使用时，传入的数组维度必须严格匹配。

+ **对于传递多维数组**

  + 将多维数组传递给函数时，实际上传递的是指向数组首元素的指针。因为处理的是数组的数组，所以首元素本身就是一个数组，指针就是一个指向数组的指针。

  + 数组第二维的大小都是数组类型的一部分，不能忽略

    ```c++
    //matrix指向数组的首元素，该数组的元素是由10个整数构成的数组
    void print((*matrix)[10], int rowSize);
    //括号必不可少
    /*
    *		(*matrix)[10] 指向含有10个整数的数组的指针
    *		*matrix[10] 	10个指针构成的数组
    */
    void print(matrix[][10], int rowSize);
    ```



### 17.含有可变形参的函数

+ **可变参模板**

+ **使用initializer_list标准库**

  + 函数的形参数量未知但是类型相同时可用。
  + 与vector不同的是，initializer_list对象中的元素永远是常量。
  + 如果想向initializer_list形参中传递一个值的序列，则必须把序列放在一对花括号内。

  ```c++
  void print(initializer_list<string> s) {
      for(auto i : s )
          cout << i << endl;
  }
  int main() {
      print({"hello", "initializer_list"});
      return 0;
  ;}
  ```

---

### 18.尾置返回类型与函数指针

#### 尾置返回类型

尾置返回类型跟在形参列表后面并以一个`->`符号开头。为了表示函数真正的返回类型跟在形参列表之后，我们在本该出现返回类型的地方放置一个auto:

```c
//func接受一个int类型的实参，返回一个指针，该指针指向含有10个整数的数组。
auto func(int i) -> int(*)[10];
```

```c++
auto func(int i) -> int * {
    static int num[10];
    for (int j = 0; j < 10; ++j) {
        num[j] = i;
    }
    return num;
}
int main() {
    int *p= func(2);
    for (int i = 0; i < 10; ++i) {
        cout << *p << endl;
    }
    return 0;
}
```

#### 函数指针

想要声明一个可以指向该函数的指针，只需要用指针代替函数名即可。

```c++
bool print(const string s1, const string s2) {
    if(s1 > s2)
        return true;
}
int main() {
    bool (*pf)(const string, const string );
    //pf = &print;
    pf = print;
    cout << pf("hello", "world");
}
```

和数组类似，虽然不能返回一个函数，但是能返回指向函数类型的指针。把返回类型写成指针形式，编译器不会自动地将函数返回类型当成对应的指针类型处理。

```c++
using F = int (int*, int);			//F是函数类型，不是指针
using PF = int(*)(int*, int);		//PF是指针类型
//显示将返回类型指定为指针
PF f1(int);		//正确：PF是指向函数的指针，f1返回指向函数的指针
F f1(int);		//错误：F是函数类型，f1不能返回一个函数
F *f1(int);		//正确：显示地返回类型是指向函数的指针
//也可以直接声明f1
int (*f1(int)) (int*, int);
//也可以使用尾置返回类型的方式
auto f1(int) -> (int*) (int*, int);
```

---

### 19.内联函数与constexpr函数

+ **内联函数**

将函数指定为内联函数，通常是将它在每一个调用点上"内联地"展开。这样会消除函数的运行开销。一般来说，内联机制用于优化规模较小，流程直接，频繁调用的函数。

+ **constexpr函数**

constexpr函数是指用于常量表达式的函数。此函数的返回类型和所有形参的类型都得是字面值类型，而且函数体中必须有且只有一条return语句。

在能在编译过程中展开，constexpr函数被隐式地指定为内联函数。

---

### 20.内联函数和宏定义的区别

内联(inline)函数和普通函数相比可以加快程序运行的速度，因为不需要中断调用，在编译的时候内联函数可以直接嵌入到目标代码中。

#### **内联函数适用场景**

- 使用宏定义的地方都可以使用inline函数
- 作为类成员接口函数来读写类的私有成员或者保护成员，会提高效率

#### **为什么不能把所有的函数写成内联函数**

内联函数以代码复杂为代价，它以省去函数调用的开销来提高执行效率。所以一方面如果内联函数体内代码执行时间相比函数调用开销较大，则没有太大的意义；另一方面每一处内联函数的调用都要复制代码，消耗更多的内存空间，因此以下情况不宜使用内联函数：

- 函数体内的代码比较长，将导致内存消耗代价
- 函数体内有循环，函数执行时间要比函数调用开销大

#### **主要区别**

- 内联函数在编译时展开，宏在预编译时展开
- 内联函数直接嵌入到目标代码中，宏是简单的做文本替换
- 内联函数有类型检测、语法判断等功能，而宏没有
- 内联函数是函数，宏不是
- 宏定义时要注意书写（参数要括起来）否则容易出现歧义，内联函数不会产生歧义
- 内联函数代码是被放到符号表中，使用时像宏一样展开，没有调用的开销，效率很高；

> 《inline函数和宏定义区别 整理》：https://blog.csdn.net/wangliang888888/article/details/77990650

- 在使用时，宏只做简单字符串替换（编译前）。而内联函数可以进行参数类型检查（编译时），且具有返回值。
- 内联函数本身是函数，强调函数特性，具有重载等功能。
- 内联函数可以作为某个类的成员函数，这样可以使用类的保护成员和私有成员，进而提升效率。而当一个表达式涉及到类保护成员或私有成员时，宏就不能实现了。

---

## 四.类

### 21.初始化和赋值的区别

- 对于简单类型来说，初始化和赋值没什么区别

  + 成员的初始化顺序与它们在类定义中的出现顺序一致，但是构造函数初始值列表中初试值的先后位置关系并不会影响实际的初始化顺序。

- 对于类和复杂数据类型来说，这两者的区别就大了，举例如下：

  ```c++
  class A{
  public:
      int num1;
      int num2;
  public:
      A(int a=0, int b=0):num1(a),num2(b){};
      A(const A& a){};
      //重载 = 号操作符函数
      A& operator=(const A& a){
          num1 = a.num1 + 1;
          num2 = a.num2 + 1;
          return *this;
      };
  };
  int main(){
  
      A a(1,1);
      A a1 = a; //拷贝初始化操作，调用拷贝构造函数
      A b;
      b = a;//赋值操作，对象a中，num1 = 1，num2 = 1；对象b中，num1 = 2，num2 = 2
      return 0;
  }
  ```

---

### 22. C中结构体内存对齐问题？

#### 对齐问题

当定义结构体为：

```c++
struct number_2
{
    char a;
    int b;
    double c;
}st2;
```

> st2所占内存并非1+4+8=13个，而是16个>13个。

结构体中包含有相同类型或不同类型的数据类型，如char（1字节），int（4字节），double（8字节），所以结构体内存出现了一个对齐原则：

**结构体变量中元素是按照定义顺序一个一个放到内存中去的，但并不是紧密排列的。从结构体存储的首地址开始，每一个元素放置到内存中时，它都会认为内存是以它自己的大小来划分的，因此元素放置的位置一定会在自己宽度的整数倍上开始（以结构体变量首地址为0计算）。**

即，当char a时，以char自身大小（一字节）划分，a占有了首地址0一个字节。但是当int b时，以int自身大小（四字节）划分，所以b从四字节的非负整数倍开始存储占用四个字节，由于首地址0已被占用，所以b从第四个地址开始占用四个字节。同理，当double c时，double也以自身大小（八字节）划分内存，而前八个（0——7）已被占用，故c从第八个地址开始占用八个字节。如下左图：

> 如果去掉中间定义的int b，st2仍然占16个字节。因为，double c时，首地址0已被占用，c不能从0（8的0倍）开始，就只能从8（8的1倍）开始。如下右图：

<div align="center">
    <img src="../图片/C:C++1.png"width="400px" />
</div>


而且，double b之前空着的内存，在对齐下原则可以随意增加变量，内存是不会增加的。如下图：

<div align="center">
    <img src="../图片/C:C++2.png"
        	width="200px",hight="400px">
</div>

#### 补齐原则

当定义结构体为：

```c++
struct number_4
{
    char a;
    int b;
    char c;
}st4;
```

> sizeof(st4)=12,而不是4+4+1=9。

这时候就牵扯到一个补齐原则：

**在经过对齐原则分析后，检查计算出的存储单元是否为所有元素中所占内存最大的元素的长度的整数倍，是，则结束；若不是，则补齐为它的整数倍。**

即，当4+4+1=9不是结构体中内存最大元素b的整数倍时，要补齐成其整数倍12。如下图左所示，如果将int换成double，sizeof(st4)=24为最大元素double的整数倍了。如下图右所示：

<div align="center">
    <img src="../图片/C:C++3.png"
        	width="400px">
</div>


---

### 22.C++类的对象存储空间？

对于类实例化一个对象，我们要计算这个对象所占的存储空间大小，其存储空间大小计算为（权威解答）：

+ **非静态成员变量总和；**
+ 加上 **编译器为CPU计算做出的数据对齐处理；**
+ 加上 **支持虚函数所产生的负担的总和。**

以上三种的加和即为当前对象的占有的存储空间的大小。

#### 代码实验

```c++
class Test {
};
int main() {
    Test test0;
    cout << sizeof(test0) << endl;
}
```

> 输出为：1
>
> 空类，没有任何成员变量和成员函数，编译器是支持空类实例化对象的，对象必须要被分配内存空间才有意义，这里编译器默认分配了 1Byte 内存空间(不同的编译器可能不同)

```c++
class Test {
private:
    int i;
    char c;
    double d;
};
int main() {
    Test test1;
    cout << sizeof(test1) << endl;
}
// 输出为：16

class A{};
class Test {
private:
    int i;
    char c;
    double d;
    A a;
};
int main() {
    Test test2;
    cout << sizeof(test2) << endl;
}
//输出为：24

class A {
private:
    double dd;
    int ii;
    int* pp;
};
class Test {
private:
    int i;
    A a;
    double d;
    char* p;
};
int main() {
  	A a;
    Test test3;
    cout << sizeof(test3) << endl;
  	cout << sizeof(a) << endl;
    cout << sizeof(test1.pp) << endl;
}
//输出为：
//48
//24
//8
```

> - 这里的类的内存对齐原则与前面写的结构体的内存对齐原则是一样的(不太了解的可以移步我之前的《C/C++中内存对齐问题的一些理解》查看)
> - 测试三中，32bit 目标平台寻址空间是 4Byte(32bit)，所以指针是 4Byte的；64bit 目标平台寻址空间是 8Byte(64bit)，所以指针是 8Byte
> - 另外，静态成员变量是在编译阶段就在静态区分配好内存的，所以静态成员变量的内存大小不计入类空间

```c++
class A {
public:
    int n;
    char c;
    short s;
};
class Test {
public:
    Test() {
    }
    int func0() {
        return n;
    }
    friend int func1();

    int func2() const {
        return s;
    }
    inline void func3() {
        cout << "inline function" << endl;
    }
    static void func4() {
        cout << "static function" << endl;
    }
    virtual void func5() {
        cout << "virtual function" << endl;
    }
    ~Test() {
    }

private:
    int n;
    char c;
    short s;
};

int func1() {
    Test t;
    return t.c;
}
int main() {
  	A a;
    Test test4;
    cout << sizeof(test4) << endl;
  	cout << sizeof(a) << endl;
    cout << sizeof(test1.n) << endl;
  	cout << sizeof(test1.c) << endl;
  	cout << sizeof(test1.s) << endl;
}
//输出:
//16
//8
//4
//1
//2
```

> - 因 C++中成员函数和非成员函数都是存放在代码区的，故类中一般成员函数、友元函数，内联函数还是静态成员函数都不计入类的内存空间，测试一和测试二对比可证明这一点
> - 测试三中，因出现了虚函数，故类要维护一个指向虚函数表的指针，分别在 x86目标平台和x64目标平台下编译运行的结果可证明这一点，x64下虚函数表占了8字节

---

### 23.C++有哪几种的构造函数

C++中的构造函数可以分为4类：

- **默认构造函数**
- **初始化构造函数（有参数）**
- **拷贝构造函数**
- **移动构造函数（move和右值引用）**
- **委托构造函数**
- **转换构造函数**

举个例子：

```c++
#include <iostream>
using namespace std;

class Student{
public:
    Student(){//默认构造函数，没有参数
        this->age = 20;
        this->num = 1000;
    };  
    Student(int a, int n):age(a), num(n){}; //初始化构造函数，有参数和参数列表
    Student(const Student& s){//拷贝构造函数，这里与编译器生成的一致
        this->age = s.age;
        this->num = s.num;
    }; 
    Student(int r){   //转换构造函数,形参是其他类型变量，且只有一个形参
        this->age = r;
        this->num = 1002;
    };
  	Student(std::istream &is) : Student() {//委托构造函数，他委托给了默认构造函数。当这些受委托的构造函数执行完后，接着执行istream&构造函数体的内容。
      	read(is, *this);
    }
    ~Student(){}
public:
    int age;
    int num;
};

int main(){
    Student s1;
    Student s2(18,1001);
    int a = 10;
    Student s3(a);
    Student s4(s3);

    printf("s1 age:%d, num:%d\n", s1.age, s1.num);
    printf("s2 age:%d, num:%d\n", s2.age, s2.num);
    printf("s3 age:%d, num:%d\n", s3.age, s3.num);
    printf("s2 age:%d, num:%d\n", s4.age, s4.num);
    return 0;
}
//运行结果
//s1 age:20, num:1000
//s2 age:18, num:1001
//s3 age:10, num:1002
//s2 age:10, num:1002
```

- 默认构造函数和初始化构造函数在定义类的对象，完成对象的初始化工作
- 复制构造函数用于复制本类的对象
- 转换构造函数用于将其他类型的变量，隐式转换为本类对象

> 《浅谈C++中的几种构造函数》：https://blog.csdn.net/zxc024000/article/details/51153743

---

### 24.有关友元

- 类通过增加friend关键字在函数开头进行声明来将其作为友元，这样就允许函数访问自己的非公有成员。
- 友元声明只能在类的内部，但是 **声明友元之外必须再专门对函数进行一次声明。**

```c++
class Sales_data{
    friend Sales_data add (const Sales_data&, const Sales_data&);
  	...
public:
    Sales_data() = default;
    ...

private:
    double avg_price () const
        { return unit_sold ? revenue/unit_sold : 0; }
    string bookNo;
    ...
};
Sales_data add (const Sales_data&, const Sales_data&);
```

- 当一个类指定了友元类，则友元类的成员函数可以访问此类包括非公有成员在内的所有成员。
- 友元不具有传递性。
- 也可以令成员函数作为友元。
- 当类想把一组重载函数声明成友元时，必须每一个都进行声明。

---

###  25.using 与 typedef

> 在用来定义类型的成员必须先定义后使用，这一点和普通成员变量有所区别。

#### typedef 

别名命名方式： `typedef std::string::size_type pos`

#### using

别名命名方式： `using pos = std::string::size_type`

---

### 26.类的静态成员

- 类的`静态成员`只与类本身相关，与其任何对象都无关。
  + 形式是在成员声明前加`static`关键字
  + 可以是public或private，类型可是常量、引用、指针、类类型等
  + 类的静态成员存在于任何对象之外，任何对象中都不包含与之相关的数据
  + 静态成员不与任何对象绑定，故不存在`this指针`。因此既不能在函数体内使用this指针，也不能被声明为const成员函数。
- 静态成员的定义：
  + `静态成员函数`可在类内或类外定义，在类外定义时不可重复static关键字，`static只出现在声明中`。
  + `静态数据成员`并非在创建类时被定义，因此`静态数据成员不由构造函数初始化`。
  + 不能在类内部初始化`静态数据成员`，`静态数据成员必须在类外定义和初始化`，一个静态数据成员只能被定义一次
  + 静态数据成员定义在任何函数之外，一旦被定义就存在于程序整个生命周期。
  + 为确保静态数据成员只被定义一次，最好将其定义与其他非内联函数的定义放在同一头文件
  + `静态成员函数可在类内和类外定义，静态数据成员只能在类外定义和初始化`
- 静态成员的访问方式：
  + 可用类的`作用域运算符`直接访问静态成员，也可用类的对象、引用、指针来访问静态成员
  + 成员函数不用通过作用域运算符就可访问静态成员

- 例子：声明、定义、访问静态成员

  ```c++
  //声明静态成员
  class Account{
  public:
      void calculate() {amount+=amount*interestRate;}
      static double rate() {return interestRate;} //静态成员函数，它可在类内也可在类外定义
      static void rate(double);                   //静态成员函数
  private:
      string owner;
      double amount;
      static double interestRate;                 //静态成员变量
      static double initRate();                   //静态成员函数
  };
  //定义静态成员
  void Account::rate(double newRate){         //定义静态成员函数，它可在类内也可在类外定义
      interestRate=newRate;
  }
  double Account::interestRate=initRate();    //定义静态成员变量，它只能在类外定义和初始化
  //访问静态成员
  double r;
  r=Account::rate();  //通过作用域访问
  Account ac1;
  Account *ac2=&ac1;
  r=ac1.rate();       //通过类引用访问
  r=ac2->rate();      //通过类指针访问
  ```

- 通常，类的静态数据成员不应在类内初始化。特例是，可为静态数据成员提供`const整型`的`类内初始值`，且该静态数据成员必须是`constexpr类型`，初值必须是常量表达式。它们可用到任何需要常量表达式的地方

- 例子：类内初始化的静态数据成员必须是字面值常量类型的constexpr

  ```c++
  class Account{
  public:
      static double rate(){return interestRate;}
      static void rate(double);
  private:
      static constexpr int period=30; //常量表达式
      double daily_tbl[period];       //可用于需要常量表达式的地方
  };
  ```

---

## 五.IO流

[笔记](https://github.com/ZYBO-o/C-plus-plus-Series/tree/main/IO%20library#%E4%B8%80io%E7%B1%BB)

