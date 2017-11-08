#include <iostream>  
#include <string>  
#include <cmath>
#include<stdlib.h>
#include<time.h>
#include<stack>
#include<windows.h>
#include<conio.h>
#include<iomanip>
#include <fstream>
#include<algorithm>   
#include<thread>
using namespace std;

class Work24
{
public:
	double PRECISION = 1E-6;
	int NUMBER_TO_BE_CAL = 24;
	double number[4];
	string expression[4];
	string ans;
	bool Judgement = false;          //�ж��Ƿ��н� 
	int count = 0;
	int num[100] = { 0 };
	void Search(int n);
	void Randomnum();
	bool Work();
};

void Work24::Search(int n)
{
	if (n == 1)
	{
		if (fabs(number[0] - NUMBER_TO_BE_CAL) <= PRECISION) /*���ڳ�����ҪС��С���ľ�ȷλ��*/
		{
			Judgement = true;
			ans.assign(expression[0]);
		}
	}
	for (int i = 0; i<n; i++)
	{
		for (int j = i + 1; j<n; j++)
		{
			double a, b;
			string expa, expb;
			a = number[i];
			b = number[j];
			number[j] = number[n - 1];  //�ݹ�֮��n����ǰСһλ�����Կ��Բ�ͣ��ǰ��ֵ  
			expa = expression[i];
			expb = expression[j];
			expression[j] = expression[n - 1];  //�ݹ�֮��n����ǰСһλ�����Կ��Բ�ͣ��ǰ��ֵ 
			expression[i] = '(' + expa + '+' + expb + ')';  //�ӷ�����Ҫ��˳�� 
			number[i] = a + b;
			Search(n - 1);
			expression[i] = '(' + expa + '-' + expb + ')';  //����Ӧ�÷�˳�򣬼����Լ������� 
			number[i] = a - b;
			Search(n - 1);
			expression[i] = '(' + expb + '-' + expa + ')';
			number[i] = b - a;
			Search(n - 1);
			expression[i] = '(' + expa + '*' + expb + ')';  //�˷�����Ҫ��˳�� 
			number[i] = a*b;
			Search(n - 1);
			if (b != 0)
			{
				expression[i] = '(' + expa + '/' + expb + ')';  //����Ӧ�÷�˳�򣬳����Լ������� 
				number[i] = a / b;
				Search(n - 1);
			}
			if (a != 0)
			{
				expression[i] = '(' + expb + '/' + expa + ')';
				number[i] = b / a;
				Search(n - 1);
			}
			number[i] = a;         //��4�������Ϊ�˷�ֹ������漸�ֿ��ܶ�ʧ���˵Ļ�, �Ͱ�ԭ���ĸ�ֵ������ȥ,���޸��ŵ���ȷ�Ľ��뵽��һ��forѭ�������С�
			number[j] = b;
			expression[i] = expa;
			expression[j] = expb;
		}
	}
}

void Work24::Randomnum()
{
	srand((int)time(NULL));
	for (int i = 0; i<4; i++)	num[i] = rand() % 10 + 1;
	sort(num, num + 4);
	cout << "�������ĸ������Ϊ:\n";
	for (int i = 0; i<4; i++)	cout << num[i] << " ";
	cout << endl;
	for (int i = 0; i<4; i++)
	{
		char buffer[20];
		number[i] = num[i];
		_itoa(number[i], buffer, 10);   //itoa()�����������ǰѵ�һ������(��ֵ)���͵��ڶ�������(�ַ���)��ȥ��������  
									   //����(int��)�Ǹ���ֵ���ַ�������ʲô���ƴ�š�  
		expression[i] = buffer;
	}
}

bool Work24::Work()
{
	Search(4);
	if (Judgement == true) return true;
	else 			return false;
}

class Prove
{
public:
	string infix;                       //ԭ���ı��ʽ
	string postfix;                     //�沨��ʽ������׺���ʽ
	string str;							//��ʼ������ַ��� 
	stack<char> stc;                    //���ڴ��������Ķ�ջ
	int res;							//������ռ����� 
	bool tflag = true;
	float t;
	int CharCompare(char c);            //�ַ��Ƚ�
	void InfixTopostfix();              //���ʽת��
	void Calculate();                   //������
	int Oper(int num1, char op, int num2);//����һ���ӱ��ʽ
	bool JudgeC(char c);				//�����ж��Ƿ�����Ӧ����� 
	bool JudgeN(char c);				//�����ж��Ƿ��ǹ涨������ 
	void gotoxy(int, int);
	void getxy(int &x, int &y);
	bool IsLegal();
	void Count();
	HANDLE m1;
	static DWORD WINAPI Input(LPVOID lpParam);
};


bool Prove::JudgeC(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')')	return true;
	else return false;
}

bool Prove::JudgeN(char c)
{
	if (c == 'A' || (c >= '1'&&c <= '9'))	return true;
	else return false;
}

bool Prove::IsLegal()
{
	bool ilegal = true;
	for (char c : str)
	{
		if (JudgeC(c) == true || JudgeN(c) == true)	continue;
		else
		{
			ilegal = false;
			break;
		}
	}
	return ilegal;
}

int Prove::CharCompare(char c)
{
	int ci;
	switch (c)
	{
		case '*':case '/': ci = 2; break;
		case '+':case '-': ci = 1; break;
		case '(':case ')': ci = 0; break;
	}
	return ci;
}

void Prove::InfixTopostfix()
{
	for (char c : infix)
	{
		if (JudgeC(c) == true)
		{
			if (stc.empty() || c == '(')//��ջΪ�ջ�Ϊ����ֱ����ջ
				stc.push(c);
			else if (c == ')')//��������һֱ��ջ��ֱ��������
			{
				while (stc.top() != '(')
				{
					postfix += stc.top();
					stc.pop();
				}
				stc.pop();//����'('�������ڶ�ջ��
			}
			else//���������������Ƚ��бȽϣ��ٵ�ջ
			{

				while (CharCompare(stc.top()) >= CharCompare(c))//ջ��Ԫ�ز����������ȼ����ڵ�ǰ����������ջ
				{
					postfix += stc.top();
					stc.pop();
					if (stc.empty())//��ֹ��ջ����
						break;
				}
				stc.push(c);//��ɵ�ջ�����
			}
		}
		else //����ֱ�Ӳ����׺���ʽ��
		{
			postfix += c;
			continue;
		}

	}
	while (!stc.empty())
	{
		postfix += stc.top();
		stc.pop();
	}
}

void Prove::Calculate()
{
	stack<int> inst;
	string::iterator itstr = postfix.begin();
	inst.push(*itstr - '0');//��ѹ��һ��������ֹ��ջΪ��
	++itstr;
	do
	{
		if (*itstr >= '0' && *itstr <= '9')//������ֱ��ѹջ
		{
			inst.push(*itstr - '0');
			++itstr;
		}
		else if (*itstr == 'A')
		{
			inst.push(*itstr - '0');
			++itstr;
		}
		else//������������ʱ��ȡջ����������������
		{
			int num1 = inst.top(); inst.pop();
			int num2 = inst.top(); inst.pop();
			if (num1 == 17)	num1 = 10;
			if (num2 == 17)	num2 = 10;
			inst.push(Oper(num1, *itstr, num2));
			++itstr;
		}
	} while (itstr != postfix.end());
	res = inst.top();
	//cout <<"����õ��Ľ��Ϊ��"<<res<<endl;
}

int Prove::Oper(int num1, char op, int num2)
{
	int res;
	switch (op)
	{
		case '*':res = num1 * num2; break;
		case '/':res = num2 / num1; break;
		case '-':res = num2 - num1; break;
		case '+':res = num1 + num2; break;
		default:
			break;
	}
	return res;
}

DWORD WINAPI Prove::Input(LPVOID lpParam)
{
	string &str = *(string*)lpParam;
	cin >> str;
	lpParam = &str;
	return 0;

}

void Prove::gotoxy(int x, int y)
{
	CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
	HANDLE    hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	csbiInfo.dwCursorPosition.X = x;
	csbiInfo.dwCursorPosition.Y = y;
	SetConsoleCursorPosition(hConsoleOut, csbiInfo.dwCursorPosition);
}
void Prove::getxy(int &x, int &y)
{
	CONSOLE_SCREEN_BUFFER_INFO    csbiInfo;
	HANDLE    hConsoleOut;
	hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	x = csbiInfo.dwCursorPosition.X;
	y = csbiInfo.dwCursorPosition.Y;
}

void Prove::Count()
{
	cout << "ʣ��ʱ��Ϊ\n";
	//m1 = CreateThread(NULL, 0, Input, &str, 0, NULL);
	string str;
	std::thread m1([str](string s) {cin >> s;});
	int* exc = new int;
	gotoxy(0, 5);
	cout << "������������Ĵ�(10����A����)" << endl;
	while (t > 0)
	{
		//GetExitCodeThread(m1, (LPDWORD)exc);
		if (!*exc) break;
		int x, y;
		getxy(x, y);
		gotoxy(0, 3);
		cout << endl << fixed << setprecision(1) << t << "\t";
		gotoxy(x, y);
		Sleep(100);
		t -= 0.1;
	}
	if (!str.empty()) {
		system("cls");
	}
	else {
		//keybd_event(VK_RETURN, 0, 0, 0);
		//keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		//TerminateThread(m1, 0);
		//CloseHandle(m1);
		system("cls");
		tflag = false;
		cout << "ʱ�䵽�����ֽ�����\n";
		Sleep(1000);
		return;
	}
}

struct User
{
	string name;
	int uscore;
};

class Game : public Work24, public Prove
{
public:
	string input;		//�������Ĵ�
	char l;
	float lt;
	int score = 0;
	bool  ans;		//Ĭ��Ϊ��Ϊ�� 
	User u1;
	int a[100];
	void Getans();
	void Level();
	void Run();
	void Save();
	void Start();
	void Read();
	bool IsItem();
};

bool Game::IsItem()
{
	int i = 0;
	int a[100] = { 0 };
	bool nflag = true;
	for (char c : infix)
	{
		if (JudgeN(c) == true)
		{
			if (c == 'A')	a[i] = 10;
			else	a[i] = c - 48;
			i++;
		}
	}
	int j;
	for (int i = 0; i<100; i++)
	{
		if (a[i] == 0)
		{
			j = i - 1;
			break;
		}
	}
	sort(a, a + j + 1);
	for (int i = 0; i<j; i++)
	{
		if (a[i] != Work24::num[i])
		{
			nflag = false;
			break;
		}
	}
	return nflag;
}

void Game::Getans()
{
	if (str.compare("No") == 0)	//�����������No 
	{
		if (Work24::Work() == false)	ans = true;
		else cout << "�����н⣬���磺" << Work24::ans << "=24" << endl;
	}
	else if (str.compare("root") == 0)		ans = true;
	else if (Prove::IsLegal() == true)
	{
		infix.assign(str);
		if (IsItem() == true)
		{
			InfixTopostfix();
			Calculate();
			cout << "���ݱ��ʽ" << infix << "����õ��Ľ��Ϊ:" << Prove::res << endl;
			if (res == 24)				ans = true;
		}
		else
		{
			cout << "������������ĿԤ�����ֲ�ƥ��\n";
			ans = false;
		}
	}
	else
	{
		cout << "���벻���Ϲ淶\n";
		ans = false;
	}
	if (ans == true)
	{
		cout << "���Դ���ȷ\n";
		score = score + 10;
	}
	else 			cout << "���Դ�����\n";
}

void Game::Level()
{
	do
	{
		system("cls");
		cout << "��ѡ���Ѷ�(S��A��B��C)\n";
		cout << "�Ѷ�-����ʱ��Ķ�Ӧ��ϵΪ��\n";
		cout << "S-20; A-30; B-45; C-60\n";
		cin >> l;
		switch (l)
		{
			case 'S':	lt = 20;	break;
			case 'A':	lt = 30;	break;
			case 'B':	lt = 45;	break;
			case 'C':	lt = 60;	break;
			default:	lt = 0;
		}
	} while (lt == 0);
	cout << "��ѡ�ĵ��Ѷ�Ϊ:" << l << ",��ÿ�������ʱ��Ϊ" << lt << "s" << endl;
	Sleep(2000);
}

void Game::Start()
{
	system("cls");
	for (int i = 0; i<3; i++)
	{
		ans = false;
		Prove::str = "";
		t = lt;
		cout << "��" << i + 1 << "�ο�ʼ\n";
		Randomnum();
		Count();
		if (tflag == true)
		{

			Getans();
			_getche();
			system("cls");
		}
		else
		{
			char c;
			c = cin.get();
			break;
		}
	}
}

void Game::Save()
{
	ofstream ofile;
	ofile.open("file.txt", ios::app);
	ofile << u1.name << "\t" << u1.uscore << endl;
	cout << endl;
	ofile.close();
}


void Game::Run()
{
	cout << "�������û���:";
	cin >> u1.name;
	Level();
	int j;
	system("cls");
	do
	{
		tflag = true;
		Start();
		system("cls");
		cout << "�Ƿ���Ҫ������Ϸ��(1-������Ϸ ������-�˳���Ϸ)\n";
		Sleep(500);
		cin.clear();
		cin >> j;
	} while (j == 1);
	u1.uscore = score;
	system("cls");
	cout << u1.name << "�����յĵ÷�Ϊ:" << u1.uscore << endl;
	Save();
}

void Game::Read()
{
	ifstream infile;
	infile.open("file.txt");
	string s;
	while (getline(infile, s))
	{
		cout << s << endl;
	}
	infile.close();             //�ر��ļ������� 
}

void Menu()
{
	system("cls");
	cout << "******** ��ӭ����24����Ϸ ********\n";
	cout << "******** 1. ��ʼ��Ϸ ********\n";
	cout << "******** 2. ��ѯ��Ϸ���� ********\n";
	cout << "******** 3. �˳���Ϸ ********\n";
}

int main()
{
	Game g1;
	int a;
	do
	{
		Menu();
		cin >> a;
		Sleep(500);
		system("cls");
		switch (a)
		{
			case 1:
			{
				g1.Run();
				Sleep(1500);
				Menu();
				break;
			}
			case 2:
			{
				g1.Read();
				Sleep(1500);
				Menu();
				break;
			}
		}
	} while (a != 3);
	return 0;
}
