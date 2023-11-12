#include "def.h"

// 构造函数：初始化队列和队列q，指定容量为m
STACK::STACK(int m) : QUEUE(m), q(m) {}

// 拷贝构造函数：创建一个新的栈对象作为源对象s的拷贝
STACK::STACK(const STACK& s) : QUEUE((QUEUE&)s), q(s.q) {}

// 移动构造函数：创建一个新的栈对象作为源对象s的移动版本
STACK::STACK(STACK&& s) noexcept : QUEUE((QUEUE&&)s), q((QUEUE&&)s.q) {}


int STACK::size() const noexcept
{
    return QUEUE::size() + q.size();
}

STACK::operator int() const noexcept
{
    return QUEUE::operator int() + q.operator int();
}

// 将元素e入栈
STACK& STACK::operator<<(int e)
{
    if (QUEUE::operator int() < QUEUE::size() - 1)
    {
        QUEUE::operator<<(e); // 如果基类队列有足够空间，将元素入队
        return *this;
    }
    else if (q.operator int() < q.size() - 1)
    {
        int tmp = 0;
        QUEUE::operator>>(tmp); // 否则，从基类队列出队一个元素
        q.operator<<(tmp); // 然后将该元素入队到队列q
        QUEUE::operator<<(e); // 最后将新元素入队到基类队列
        return *this;
    }
    else
    {
        throw("STACK is full!"); // 如果两个队列都满了，抛出异常
    }
}

// 将栈顶元素出栈，赋值给e
STACK& STACK::operator>>(int& e)
{
    if (QUEUE::operator int())
    {
        int sizebase = QUEUE::operator int(), tmp = 0;
        for (int i = 0; i < sizebase - 1; i++)
        {
            QUEUE::operator>>(tmp); // 从基类队列逐个出队元素
            QUEUE::operator<<(tmp); // 然后入队到基类队列
        }
        QUEUE::operator>>(e); // 最后出队栈顶元素，赋值给e
        return *this;
    }
    else if (q.operator int())
    {
        int sizebase = q.operator int(), tmp = 0;
        for (int i = 0; i < sizebase - 1; i++)
        {
            q.operator>>(tmp); // 如果基类队列为空，从队列q出队元素
            q.operator<<(tmp); // 然后入队到队列q
        }
        q.operator>>(e); // 最后出队栈顶元素，赋值给e
        return *this;
    }
    else
    {
        throw("STACK is empty!"); // 如果两个队列都为空，抛出异常
    }
}

// 拷贝赋值运算符：用于复制一个栈对象
STACK& STACK::operator=(const STACK& s)
{
    QUEUE::operator=((QUEUE&)s); // 调用基类队列的拷贝赋值运算符
    q.operator=((QUEUE&)s.q); // 调用队列q的拷贝赋值运算符
    return *this;
}

// 移动赋值运算符：用于移动赋值一个栈对象
STACK& STACK::operator=(STACK&& s) noexcept
{
    QUEUE::operator=((QUEUE&&)s); // 调用基类队列的移动赋值运算符
    q.operator=((QUEUE&&)s.q); // 调用队列q的移动赋值运算符
    return *this;
}

// 打印栈的内容到字符数组b
char* STACK::print(char* b) const noexcept
{
    char* s = (char*)malloc(sizeof(char) * size()); // 为字符数组分配内存
    s = QUEUE::print(s); // 调用基类队列的打印函数，将结果存储在s中
    b = q.print(b); // 调用队列q的打印函数，将结果追加到字符数组b中
    strcat(b, s); // 将s连接到b中
    free(s); // 释放s的内存
    return b;
}

// 析构函数：销毁栈对象，分别调用基类队列和队列q的析构函数
STACK::~STACK() noexcept
{
}}
