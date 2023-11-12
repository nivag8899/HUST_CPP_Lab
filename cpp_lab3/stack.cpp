#include "def.h"

// ���캯������ʼ�����кͶ���q��ָ������Ϊm
STACK::STACK(int m) : QUEUE(m), q(m) {}

// �������캯��������һ���µ�ջ������ΪԴ����s�Ŀ���
STACK::STACK(const STACK& s) : QUEUE((QUEUE&)s), q(s.q) {}

// �ƶ����캯��������һ���µ�ջ������ΪԴ����s���ƶ��汾
STACK::STACK(STACK&& s) noexcept : QUEUE((QUEUE&&)s), q((QUEUE&&)s.q) {}


int STACK::size() const noexcept
{
    return QUEUE::size() + q.size();
}

STACK::operator int() const noexcept
{
    return QUEUE::operator int() + q.operator int();
}

// ��Ԫ��e��ջ
STACK& STACK::operator<<(int e)
{
    if (QUEUE::operator int() < QUEUE::size() - 1)
    {
        QUEUE::operator<<(e); // �������������㹻�ռ䣬��Ԫ�����
        return *this;
    }
    else if (q.operator int() < q.size() - 1)
    {
        int tmp = 0;
        QUEUE::operator>>(tmp); // ���򣬴ӻ�����г���һ��Ԫ��
        q.operator<<(tmp); // Ȼ�󽫸�Ԫ����ӵ�����q
        QUEUE::operator<<(e); // �����Ԫ����ӵ��������
        return *this;
    }
    else
    {
        throw("STACK is full!"); // ����������ж����ˣ��׳��쳣
    }
}

// ��ջ��Ԫ�س�ջ����ֵ��e
STACK& STACK::operator>>(int& e)
{
    if (QUEUE::operator int())
    {
        int sizebase = QUEUE::operator int(), tmp = 0;
        for (int i = 0; i < sizebase - 1; i++)
        {
            QUEUE::operator>>(tmp); // �ӻ�������������Ԫ��
            QUEUE::operator<<(tmp); // Ȼ����ӵ��������
        }
        QUEUE::operator>>(e); // ������ջ��Ԫ�أ���ֵ��e
        return *this;
    }
    else if (q.operator int())
    {
        int sizebase = q.operator int(), tmp = 0;
        for (int i = 0; i < sizebase - 1; i++)
        {
            q.operator>>(tmp); // ����������Ϊ�գ��Ӷ���q����Ԫ��
            q.operator<<(tmp); // Ȼ����ӵ�����q
        }
        q.operator>>(e); // ������ջ��Ԫ�أ���ֵ��e
        return *this;
    }
    else
    {
        throw("STACK is empty!"); // ����������ж�Ϊ�գ��׳��쳣
    }
}

// ������ֵ����������ڸ���һ��ջ����
STACK& STACK::operator=(const STACK& s)
{
    QUEUE::operator=((QUEUE&)s); // ���û�����еĿ�����ֵ�����
    q.operator=((QUEUE&)s.q); // ���ö���q�Ŀ�����ֵ�����
    return *this;
}

// �ƶ���ֵ������������ƶ���ֵһ��ջ����
STACK& STACK::operator=(STACK&& s) noexcept
{
    QUEUE::operator=((QUEUE&&)s); // ���û�����е��ƶ���ֵ�����
    q.operator=((QUEUE&&)s.q); // ���ö���q���ƶ���ֵ�����
    return *this;
}

// ��ӡջ�����ݵ��ַ�����b
char* STACK::print(char* b) const noexcept
{
    char* s = (char*)malloc(sizeof(char) * size()); // Ϊ�ַ���������ڴ�
    s = QUEUE::print(s); // ���û�����еĴ�ӡ������������洢��s��
    b = q.print(b); // ���ö���q�Ĵ�ӡ�����������׷�ӵ��ַ�����b��
    strcat(b, s); // ��s���ӵ�b��
    free(s); // �ͷ�s���ڴ�
    return b;
}

// ��������������ջ���󣬷ֱ���û�����кͶ���q����������
STACK::~STACK() noexcept
{
}}
