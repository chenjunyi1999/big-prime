

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cmath>
#include<random>

#include <boost/multiprecision/cpp_int.hpp>

using namespace std;


using integer_t = boost::multiprecision::cpp_int;
using boost::multiprecision::pow;

const int S = 50;//DDS�ٷ��涨Ϊ����50�ˣ�Ҳ�����޸ģ���ֵԽ�󾫶�Խ��

integer_t mult_mod(integer_t a, integer_t b, integer_t c)  //���� (a*b)%c. ==>[(a%c)*(b%c)]%c
{
	a %= c;
	b %= c;
	integer_t ret = 0, tmp = a;
	while (b)
	{
		if (b & 1)               
		{
			ret += tmp;
			if (ret > c)
				ret -= c;
		}
		tmp = tmp << 1;
		if (tmp > c)
			tmp -= c;
		b >>= 1;
	}
	return ret;
}

integer_t pow_mod(integer_t a, integer_t n, integer_t c) //����  x^n %c         
{                                                       //�����ݽ�11==��1011   a^11=a^1*a^2*a^8
	integer_t ret = 1, tmp = a % c;
	while (n)
	{
		if (n & 1)                         //�ж�n�Ƿ�Ϊ����,��ΪnΪ����ʱ����Ӧ�Ķ����������λһ��Ϊ1��n & 1�Ľ������1
			ret = mult_mod(ret, tmp, c);   //�鿴��n��a^(2^n)�Ƿ���Ҫ����
		tmp = mult_mod(tmp, tmp, c);       //���a^(2^n+1)
		n = n >> 1;                        // n-->n+1
	}
	return ret;
}
//��aΪ��,n-1=x*2^t      a^(n-1)=1(mod n)  ��֤n�ǲ��Ǻ���
//һ���Ǻ�������true,��һ������false

bool check(integer_t a, integer_t n, integer_t x, integer_t t)
{
	integer_t ret = pow_mod(a, x, n);      
	integer_t last = ret;
	for (int i = 1; i <= t; i++)            //��a^x���a^(x<<t)
	{
		ret = mult_mod(ret, ret, n);         
		if (ret == 1 && last != 1 && last != n - 1)
			return true;
		last = ret;
	}
	if (ret != 1)
		return true;
	else
		return false;
}

bool Miller_Robin(integer_t n)
{

	if (n < 2)
		return false;
	if (n == 2)
		return true;
	if ((n & 1) == 0)
		return false;
	integer_t x = n - 1, t = 0;
	while ((x & 1) == 0)
	{
		x = x >> 1;
		t++;           //�����ƴ������һ����Ϊ0��λ�� 
	}
	srand(time(NULL));
	for (int i = 0; i < S; i++)
	{
		integer_t a = rand() % (n - 1) + 1;
		if (check(a, n, x, t))     //����
			return false;
	}
	return true;
}

bool Prime(int i)    //��ֵС�����������
{
	int j;
	if (i <= 1) return 0;//���С�ڵ���1����0
	if (i == 2) return 1;//�����2������1
	for (j = 2; j < i; j++) {//ѭ�������ж��Ƿ�Ϊ����
		if (i % j == 0)return 0;
		else if (i != j + 1) continue;
		else return 1;
	}
}
void test()
{
	long i, j, k, flag1, flag2, n = 0;
	int a, b;
	cout << "������֤1000���µĸ�°ͺղ���" << endl;
	int min = 6;
	int max = 1000;
	for (i = min; i <= max; i += 2) {            //ż�����
		for (k = 2; k <= i / 2; k++) {           //���ݸ���������ż����Ϊ������
			j = i - k;//��һ����
			flag1 = Prime(k);
			if (flag1) {//���kΪ����
				flag2 = Prime(j);
				if (flag2) {//jҲΪ����
					cout << i << " = " << j << "+" << k << "  ";
					n++;
					if (n % 10 == 0) cout << endl;//ÿ5������
					break;
				}
			}
		}
	}
	cout << "�ܹ��γɵ�����λ"<<n;
	if (n == (max - min) / 2 + 1)
		cout << endl << "���ϣ���°ͺղ�����ȷ��"<<endl;//�����������Ҫ�����������ȷ
}

void createprime(int i)
{
	int count = 0;
	const integer_t a = pow(integer_t(10), unsigned(25));
	const integer_t b = pow(integer_t(10), unsigned(100));
	//rand()%(b-a+1)+a
	part1:integer_t n = a;
	for ( n = a; n <= b; n++)
	{
		if (Miller_Robin(n))
		{
			count++;
			if (count == i)
			{
				cout << "���������������" << n << endl;
				break;
			}
		}
	}
	if (n > b)
	{
		i / 2;
		goto part1;
	}
	
}

int main()
{
	while (true)
	{
		int option;
		cout << "��ѡ��һ������1:���ɴ����� 2:�жϴ����� 3:��֤1000���µĸ�°ͺղ��� 4:�˳�" << endl;
		cin >> option;
		if (option == 1)
		{
			default_random_engine e(time(0));
			uniform_int_distribution<unsigned int> u(1,10);
			int i = u(e);
			createprime(i);
		}
        else if (option == 2)
		{
			integer_t n;
			cout << "������Ҫ���ԵĴ�����";
			cin >> n;
			while (n < 0)
			{
				cout << "�������������Ҫ����������ж�" << endl;
				cin >> n;
			}
			if (Miller_Robin(n))
			{
				cout << "������" << endl;
			}
			else
			{
				cout << "��������" << endl;
			}
		}
		else if (option == 3)
		{
			test();
		}
		else if (option == 4)
		{
			return 0;
		}
		else
		{
			cout << "��������밴��ʾ������һ��" << endl;
		}
	}
}

/*
�����ޱ����Բ�����һ���൱�������ж��Ƿ����������㷨����Ϊ����С����
����a��������p����������a,p����(������ֻ��һ����Լ��1)����ôa��(p-1)�η�����p�����������1��
����һ�¼�p�� a^(p-1)%p !=1 (0<a<p) ����һ���Ǻ�����
��� a^(p-1)%p ==1 (0<a<p) ���������Ǻ��������������������㷨�ĸ��ʾ������ a�����֡�
������̣�
1 ���ȡһ�� a
2 ��� ������ a^(n-1)%n !=1
3 ��nһ���� ����
4 �˳�
5 ��������� a^(n-1)%n ==1
6  ������һ�������ĸ�����3/4
7 �ص� 1
*/