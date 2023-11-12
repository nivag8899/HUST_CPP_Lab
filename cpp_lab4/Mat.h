#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>
#include <iomanip> 
#include <exception>
#include <typeinfo>
#include <string.h>
using namespace std;

// ����һ��ģ���� MAT�����ڽ��о�������
template <typename T>
class MAT {
    T* const e;             // ָ���������;���Ԫ�ص�ָ��
    const int r, c;         // �������r����c��С
public:
    MAT(int r, int c);                      // ������
    MAT(const MAT& a);                     // �������
    MAT(MAT&& a) noexcept;                 // �ƶ�����
    virtual ~MAT() noexcept;
    virtual T* const operator[](int r);     // ȡ����r�еĵ�һ��Ԫ�ص�ַ��rԽ�����쳣
    virtual MAT operator+(const MAT& a) const;  // ����ӷ������ܼ����쳣
    virtual MAT operator-(const MAT& a) const;  // ������������ܼ����쳣
    virtual MAT operator*(const MAT& a) const;  // ����˷������ܳ����쳣
    virtual MAT operator~() const;              // ����ת��
    virtual MAT& operator=(const MAT& a);       // �����ֵ����
    virtual MAT& operator=(MAT&& a) noexcept;  // �ƶ���ֵ����
    virtual MAT& operator+=(const MAT& a);     // "+=" ����
    virtual MAT& operator-=(const MAT& a);     // "-=" ����
    virtual MAT<T>& operator*=(const MAT& a);   // "*=" ����
    virtual char* print(char* s) const noexcept; // ������������ַ�����s�����ÿո���������ûس�����
};

// ���캯�������ڴ����������ָ������������������̬�����ڴ����洢����Ԫ��
template<typename T>
MAT<T>::MAT(int r, int c) : MAT::r(r), MAT::c(c), e(new T[r * c]) {

}

// ������캯�������ڴ���һ���µľ��������ȿ�����һ��������� a
template<typename T>
MAT<T>::MAT(const MAT& a) : MAT::r(a.r), MAT::c(a.c), e(new T[a.r * a.c]) {
    memcpy(e, a.e, r * c * sizeof T);
}

// �ƶ����캯�������ڴ���һ���µľ������ͨ���ƶ������ȡ��һ��������� a ����Դ
template<typename T>
MAT<T>::MAT(MAT&& a) noexcept : MAT::e((T*&&)a.e), MAT::c(a.c), MAT::r(a.r) {
    (T*&)a.e = nullptr;
    (int&)a.r = 0;
    (int&)a.c = 0;
}

// �����������������پ�������ͷŶ�̬������ڴ�
template<typename T>
MAT<T>::~MAT() noexcept {
    if (e != nullptr) {
        delete[] e;
        (T*&)e = nullptr;
        (int&)r = (int&)c = 0;
    }
}

// ���������[]���Ի�ȡ������е��׵�ַ
template<typename T>
T* const MAT<T>::operator[](int r) {
    if (r >= 0 && r < MAT::r) {
        return &e[r * c];
    }
    else throw("r������ȡֵ��Χ��");
}

// ����+�����������ִ�о���ļӷ�����
template<typename T>
MAT<T> MAT<T>::operator+(const MAT& a) const {
    if (r == a.r && c == a.c) {
        MAT<T> tmp(*this);
        for (int i = 0; i < r * c; i++) {
            tmp.e[i] += a.e[i];
        }
        return tmp;
    }
    else throw("����������ȫ��ͬ������ӣ�");
}

// ����-�����������ִ�о���ļ�������
template<typename T>
MAT<T> MAT<T>::operator-(const MAT& a) const {
    if (r == a.r && c == a.c) {
        MAT<T> tmp(*this);
        for (int i = 0; i < r * c; i++) {
            tmp.e[i] -= a.e[i];
        }
        return tmp;
    }
    else throw("����������ȫ��ͬ���������");
}

// ����*�����������ִ�о���ĳ˷�����


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
    else throw("����������������Ҿ�������������ˣ�");    
}


// ����~�����������ִ�о����ת�ò���
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

// ����=�����������ִ�������ֵ����
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

// ����=�����������ִ���ƶ���ֵ����
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

// ����+=�����������ִ�о���ļӷ���ֵ����
template<typename T>
MAT<T>& MAT<T>::operator+=(const MAT& a) {
    if (r == a.r && c == a.c) {
        for (int i = 0; i < r * c; i++) {
            this->e[i] += a.e[i];
        }
        return *this;
    }
    else throw("����������ȫ��ͬ������ӣ�");
}

// ����-=�����������ִ�о���ļ�����ֵ����
template<typename T>
MAT<T>& MAT<T>::operator-=(const MAT& a) {
    if (r == a.r && c == a.c) {
        for (int i = 0; i < r * c; i++) {
            this->e[i] -= a.e[i];
        }
        return *this;
    }
    else throw("����������ȫ��ͬ���������");
}

// ����*=�����������ִ�о���ĳ˷���ֵ����
template <typename T>
MAT<T>& MAT<T>::operator*=(const MAT& a)
{
    if (this->c != a.r)
    {
        throw ("����������������Ҿ�������������ˣ�");
    }
    *this = (*this) * a;
    return *this;
}

// print���������ڽ�����������ַ����� s�������� s
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

// ģ��ʵ����������ʵ���ģ��������ģ�嶨���ļ���β��������ǿ��ʵ����
template MAT<int>;
template MAT<long long>;
