#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <iomanip> 
#include <exception>
#include <typeinfo>
#include <string.h>
using namespace std;

// 定义一个模板类 MAT，用于进行矩阵运算
template <typename T>
class MAT {
    T* const e;             // 指向所有整型矩阵元素的指针
    const int r, c;         // 矩阵的行r和列c大小
public:
    MAT(int r, int c);                      // 矩阵定义
    MAT(const MAT& a);                     // 深拷贝构造
    MAT(MAT&& a) noexcept;                 // 移动构造
    virtual ~MAT() noexcept;
    virtual T* const operator[](int r);     // 取矩阵r行的第一个元素地址，r越界抛异常
    virtual MAT operator+(const MAT& a) const;  // 矩阵加法，不能加抛异常
    virtual MAT operator-(const MAT& a) const;  // 矩阵减法，不能减抛异常
    virtual MAT operator*(const MAT& a) const;  // 矩阵乘法，不能乘抛异常
    virtual MAT operator~() const;              // 矩阵转置
    virtual MAT& operator=(const MAT& a);       // 深拷贝赋值运算
    virtual MAT& operator=(MAT&& a) noexcept;  // 移动赋值运算
    virtual MAT& operator+=(const MAT& a);     // "+=" 运算
    virtual MAT& operator-=(const MAT& a);     // "-=" 运算
    virtual MAT<T>& operator*=(const MAT& a);   // "*=" 运算
    virtual char* print(char* s) const noexcept; // 将矩阵输出到字符数组s，列用空格隔开，行用回车结束
};

// 构造函数，用于创建矩阵对象，指定行数和列数，并动态分配内存来存储矩阵元素
template<typename T>
MAT<T>::MAT(int r, int c) : MAT::r(r), MAT::c(c), e(new T[r * c]) {

}

// 深拷贝构造函数，用于创建一个新的矩阵对象，深度拷贝另一个矩阵对象 a
template<typename T>
MAT<T>::MAT(const MAT& a) : MAT::r(a.r), MAT::c(a.c), e(new T[a.r * a.c]) {
    memcpy(e, a.e, r * c * sizeof T);
}

// 移动构造函数，用于创建一个新的矩阵对象，通过移动语义获取另一个矩阵对象 a 的资源
template<typename T>
MAT<T>::MAT(MAT&& a) noexcept : MAT::e((T*&&)a.e), MAT::c(a.c), MAT::r(a.r) {
    (T*&)a.e = nullptr;
    (int&)a.r = 0;
    (int&)a.c = 0;
}

// 析构函数，用于销毁矩阵对象，释放动态分配的内存
template<typename T>
MAT<T>::~MAT() noexcept {
    if (e != nullptr) {
        delete[] e;
        (T*&)e = nullptr;
        (int&)r = (int&)c = 0;
    }
}

// 重载运算符[]，以获取矩阵的行的首地址
template<typename T>
T* const MAT<T>::operator[](int r) {
    if (r >= 0 && r < MAT::r) {
        return &e[r * c];
    }
    else throw("r超出了取值范围！");
}

// 重载+运算符，用于执行矩阵的加法操作
template<typename T>
MAT<T> MAT<T>::operator+(const MAT& a) const {
    if (r == a.r && c == a.c) {
        MAT<T> tmp(*this);
        for (int i = 0; i < r * c; i++) {
            tmp.e[i] += a.e[i];
        }
        return tmp;
    }
    else throw("行列数不完全相同不能相加！");
}

// 重载-运算符，用于执行矩阵的减法操作
template<typename T>
MAT<T> MAT<T>::operator-(const MAT& a) const {
    if (r == a.r && c == a.c) {
        MAT<T> tmp(*this);
        for (int i = 0; i < r * c; i++) {
            tmp.e[i] -= a.e[i];
        }
        return tmp;
    }
    else throw("行列数不完全相同不能相减！");
}

// 重载*运算符，用于执行矩阵的乘法操作


template <typename T>
MAT<T> MAT<T>::operator*(const MAT& a)const
{
    if (c == a.r) {
        MAT<T>& self = (MAT<T> &)(*this);
        MAT<T>& self_a = (MAT<T> &)(a);
        MAT ans(r, a.c);

        for (int i = 0; i < ans.r; i++)
        {
            for (int j = 0; j < ans.c; j++)
            {
                ans[i][j] = 0;
                for (int k = 0; k < c; k++)
                {
                    ans[i][j] += self[i][k] * self_a[k][j];
                }
            }
        }
        return ans;
    }
    else throw("左矩阵列数不等于右矩阵行数不能相乘！");    
}


// 重载~运算符，用于执行矩阵的转置操作
template <typename T>
MAT<T> MAT<T>::operator~()const
{
    MAT<T>& self = (MAT<T> &)(*this);
    MAT ans(this->c, this->r);
    for (int i = 0; i < ans.r; i++)
    {
        for (int j = 0; j < ans.c; j++)
        {
            ans[i][j] = self[j][i];
        }

    }
    return ans;
}

// 重载=运算符，用于执行深拷贝赋值操作
template<typename T>
MAT<T>& MAT<T>::operator=(const MAT& a) {
    if (this == &a) return *this;
    (int&)r = a.r;
    (int&)c = a.c;
    if (this->e != nullptr) {
        delete[] e;
        (T*&)e = new T[r * c];
    }
    memcpy(this->e, a.e, r * c * sizeof T);
    return *this;
}

// 重载=运算符，用于执行移动赋值操作
template<typename T>
MAT<T>& MAT<T>::operator=(MAT&& a) noexcept {
    if (this == &a) return a;
    (int&)r = a.r;
    (int&)c = a.c;
    (T*&)e = a.e;
    (T*&)a.e = nullptr;
    (int&)a.r = (int&)a.c = 0;
    return *this;
}

// 重载+=运算符，用于执行矩阵的加法赋值操作
template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a) {
    if (r == a.r && c == a.c) {
        for (int i = 0; i < r * c; i++) {
            this->e[i] += a.e[i];
        }
        return *this;
    }
    else throw("行列数不完全相同不能相加！");
}

// 重载-=运算符，用于执行矩阵的减法赋值操作
template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a) {
    if (r == a.r && c == a.c) {
        for (int i = 0; i < r * c; i++) {
            this->e[i] -= a.e[i];
        }
        return *this;
    }
    else throw("行列数不完全相同不能相减！");
}

// 重载*=运算符，用于执行矩阵的乘法赋值操作
template <typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)
{
    if (this->c != a.r)
    {
        throw ("左矩阵列数不等于右矩阵行数不能相乘！");
    }
    *this = (*this) * a;
    return *this;
}

// print函数，用于将矩阵输出到字符数组 s，并返回 s
template <typename T>
char* MAT<T>::print(char* s)const noexcept
{
    s[0] = '\0';
    MAT<T>& self = (MAT<T> &)(*this);
    int delta = 0;
    char  tmp[30];
    if (typeid(T) == typeid(int))
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                sprintf(tmp, "%d", self[i][j]);
                strcat(s, tmp);
                strcat(s, j == c - 1 ? "\n" : "    ");
            }
        }
    }
    else
    {
        for (int i = 0; i < r; i++)
        {
            for (int j = 0; j < c; j++)
            {
                sprintf(tmp, "%lld", self[i][j]);
                strcat(s, tmp);
                strcat(s, j == c - 1 ? "\n" : "   ");
            }
        }
    }
    return s;
}

// 模板实例化，用于实验四，必须放在模板定义文件的尾部，用于强制实例化
template MAT<int>;
template MAT<long long>;
