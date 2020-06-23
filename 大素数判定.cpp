

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

const int S = 50;//DDS官方规定为至少50此，也可以修改，数值越大精度越高

integer_t mult_mod(integer_t a, integer_t b, integer_t c)  //计算 (a*b)%c. ==>[(a%c)*(b%c)]%c
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

integer_t pow_mod(integer_t a, integer_t n, integer_t c) //计算  x^n %c         
{                                                       //快速幂将11==》1011   a^11=a^1*a^2*a^8
	integer_t ret = 1, tmp = a % c;
	while (n)
	{
		if (n & 1)                         //判断n是否为奇数,因为n为奇数时，对应的二进制数最低位一定为1，n & 1的结果就是1
			ret = mult_mod(ret, tmp, c);   //查看第n个a^(2^n)是否需要加入
		tmp = mult_mod(tmp, tmp, c);       //变成a^(2^n+1)
		n = n >> 1;                        // n-->n+1
	}
	return ret;
}
//以a为基,n-1=x*2^t      a^(n-1)=1(mod n)  验证n是不是合数
//一定是合数返回true,不一定返回false

bool check(integer_t a, integer_t n, integer_t x, integer_t t)
{
	integer_t ret = pow_mod(a, x, n);      
	integer_t last = ret;
	for (int i = 1; i <= t; i++)            //将a^x变成a^(x<<t)
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
		t++;           //二进制从右起第一个不为0的位数 
	}
	srand(time(NULL));
	for (int i = 0; i < S; i++)
	{
		integer_t a = rand() % (n - 1) + 1;
		if (check(a, n, x, t))     //合数
			return false;
	}
	return true;
}

bool Prime(int i)    //数值小的情况下适用
{
	int j;
	if (i <= 1) return 0;//如果小于等于1返回0
	if (i == 2) return 1;//如果是2，返回1
	for (j = 2; j < i; j++) {//循环遍历判断是否为质数
		if (i % j == 0)return 0;
		else if (i != j + 1) continue;
		else return 1;
	}
}
void test()
{
	long i, j, k, flag1, flag2, n = 0;
	int a, b;
	cout << "下面验证1000以下的哥德巴赫猜想" << endl;
	int min = 6;
	int max = 1000;
	for (i = min; i <= max; i += 2) {            //偶数相加
		for (k = 2; k <= i / 2; k++) {           //根据鸽笼定理，将偶数分为两部分
			j = i - k;//另一部分
			flag1 = Prime(k);
			if (flag1) {//如果k为质数
				flag2 = Prime(j);
				if (flag2) {//j也为质数
					cout << i << " = " << j << "+" << k << "  ";
					n++;
					if (n % 10 == 0) cout << endl;//每5个换行
					break;
				}
			}
		}
	}
	cout << "总共形成的组数位"<<n;
	if (n == (max - min) / 2 + 1)
		cout << endl << "如上，哥德巴赫猜想正确！"<<endl;//如果个数满足要求输出猜想正确
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
				cout << "随机产生的素数是" << n << endl;
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
		cout << "请选择一个功能1:生成大素数 2:判断大素数 3:验证1000以下的哥德巴赫猜想 4:退出" << endl;
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
			cout << "请输入要测试的大数：";
			cin >> n;
			while (n < 0)
			{
				cout << "请重新输入符合要求的数字来判断" << endl;
				cin >> n;
			}
			if (Miller_Robin(n))
			{
				cout << "是素数" << endl;
			}
			else
			{
				cout << "不是素数" << endl;
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
			cout << "输入错误，请按提示再输入一次" << endl;
		}
	}
}

/*
米勒罗宾素性测试是一个相当著名的判断是否是素数的算法核心为费马小定理：
假如a是整数，p是质数，且a,p互质(即两者只有一个公约数1)，那么a的(p-1)次方除以p的余数恒等于1。
逆推一下即p的 a^(p-1)%p !=1 (0<a<p) ，它一定是合数。
如果 a^(p-1)%p ==1 (0<a<p) 则它可能是合数可能是素数。概率算法的概率就在这个 a上体现。
具体过程：
1 随机取一个 a
2 如果 它满足 a^(n-1)%n !=1
3 则n一定是 合数
4 退出
5 如果它满足 a^(n-1)%n ==1
6  则它是一个素数的概率是3/4
7 回到 1
*/