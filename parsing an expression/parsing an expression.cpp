#include<iostream>
#include<cstring>
#include<stdlib.h>
#include<typeinfo>
using namespace std;

const int len1 = 40;
class Token//Абстрактный базовый класс
{
public:
	virtual void getNumber() = 0;//чистая виртуальная
	//функция
	virtual void getOperator() = 0;
	virtual void get() {};
};
class Operator :public Token
{
private:
	char oper;//Операторы+,-,*,/
public:
	//Operator(){};
	Operator(char ap) { oper = ap; }//конструктор устанавливает значение
	//~Operator(){cout<<"удалим Operator"<<endl;}
	void getOperator() { cout << oper << " "; }//получить значение
	void getNumber() {}//просто некая функция
	void get() { cout << oper; }
};
class Number :public Token
{
private:
	float fnum;//число
public:
	//Number(){};
	Number(float ap) { fnum = ap; }//конструктор устанавливает значение
	//~Number(){cout<<"удалим Number"<<endl;}
	void getNumber() { cout << fnum << ""; }//получить значение
	void getOperator() {}//просто некая функция
	void get() { cout << fnum; }
};
class Stack
{
private:
	int topToken;
	Token* atoken[20];//содержит типы Operator и Number*
public:
	Stack() { topToken = 0; }
	//~Stack(){cout<<"удалимStack";}
	void addOperator(char a) {
		(atoken[topToken++]) = new Operator(a);
		//*(atoken[++topToken])=a;
	}
	void addNumber(float b) {
		(atoken[topToken++]) = new Number(b);
		//*(atoken[++topToken])=b;
	}
	int gettok() { return topToken; }
	Token* pokaz(int h) { return*(atoken + h); }
};
class Express
{
private:
	Stack s;
	float nst[10];
	char ost[10] = " ";
	char* ptr;
	int nstT = 0;
	int ostT = 0;
	int len;
	int top = 0;
	float lastn[10];//стек цифр
	int lasnT = 0;//счет цифр
	int lasoT = 0;//счет знаков
	char lasto[10];//стек знаков
public:
	void addlastn(float a) { lastn[++lasnT] = a; }
	float getlastn() { return lastn[lasnT--]; }
	void addlasto(char b) { lasto[++lasoT] = b; }
	char getlasto() { return lasto[lasoT--]; }

	Express() {};
	Express(char* var) { len = strlen(var); ptr = new char(len + 1); strcpy_s(ptr,len+1, var); }
	//~Express(){cout<<"удалим Express"<<endl;}
	void addn(float a) { nst[nstT++] = a; }//НЕ ЗАБЫТЬ ПРО ++
	void addo(char b) { ost[ostT++] = b; }
	void razbros();
	void parse();
	//void solve();
	void show()
	{
		for (int h = 0; h <= s.gettok() - 1; h++)
		{
			s.pokaz(h)->get();
		}
	}
};
int main()
{
	setlocale(LC_ALL, "Russian");
	cout << "введите выражение" << endl;
	char arr[len1];
	cin >> arr;
	arr[strlen(arr) + 1] = 0;
	cout << endl;
	Express* ptrs = new Express(arr);
	ptrs->razbros();
	ptrs->parse();
	//ptrs->solve();
	//ptrs->show();//выводим из токена опер и намб, с динамическим связыванием
}
void Express::razbros()
{
	char v[20];
	char last = '+';
	int x1 = 0;
	for (int x = 0; x <= len; x++)
	{
		char ch = *(ptr + x);
		if (ch == '+' || ch == '-' || ch == '/' || ch == '*')//если сейчас ch равно знаку
		{
			if (last != '+' && last != '-' && last != '/' && last != '*')//а до этого была цифра
			{
				v[x1] = 0;
				double c = atof(v);
				x1 = 0;
				float c1 = static_cast<float>(c);
				s.addNumber(c1);
				s.addOperator(ch); last = ch;
				addn(c1);
				addo(ch);
			}
		}
		else
		{
			v[x1] = ch;
			last = ch;
			x1++;
			if (x == len)
			{
				v[x1] = 0;
				double c = atof(v);
				x1 = 0;
				float c1 = static_cast<float>(c);
				s.addNumber(c1);
				addn(c1);
			}
		}
	}
}
void Express::parse()
{
	float nowno;
	char nowop;
	char lastop = '1';
	float lastno;
	for (int x = 0; x < nstT; x++)
	{
		nowno = nst[x];
		nowop = ost[x];
		if (lastop == '1')
		{
			addlastn(nowno); addlasto(nowop); lastop = nowop;
		}
		else
		{
			lastno = getlastn();
			lastop = getlasto();
			if ((nowop == '*' || nowop == '/') && (lastop == '+' || lastop == '-'))
			{
				addlastn(lastno);
				addlasto(lastop);
				addlastn(nowno);
				addlasto(nowop);
			}
			else
			{
				switch (lastop)
				{
				case '*':addlastn(lastno * nowno); break;
				case '/': addlastn(lastno / nowno); break;
				case '+':addlastn(lastno + nowno); break;
				case '-': addlastn(lastno - nowno); break;
				default: break;
				}
				addlasto(nowop);
			}
		}
	}
	//2*  3+  4   :::  6+  4 x????
	//2+4*3+2
	// for(int z=0; z<=lasnT+1; z++)
	// {cout<<getlastn();}
	getlasto();
	float rez;
	int c = lasnT;
	while (c > 1)
	{
		float temp = getlastn();
		char zn = getlasto();
		float per = getlastn();
		switch (zn)
		{
		case '*': addlastn(per * temp); break;
		case '/': addlastn(per / temp); break;
		case '+':addlastn(per + temp); break;
		case '-': addlastn(per - temp); break;
		default: break;
		}
		c--;
	}
	rez = getlastn();
	cout << rez;
}