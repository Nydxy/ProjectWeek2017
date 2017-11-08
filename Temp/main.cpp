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
	bool Judgement = false;          //判断是否有解 
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
		if (fabs(number[0] - NUMBER_TO_BE_CAL) <= PRECISION) /*对于除法，要小心小数的精确位数*/
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
			number[j] = number[n - 1];  //递归之后，n比以前小一位，所以可以不停向前赋值  
			expa = expression[i];
			expb = expression[j];
			expression[j] = expression[n - 1];  //递归之后，n比以前小一位，所以可以不停向前赋值 
			expression[i] = '(' + expa + '+' + expb + ')';  //加法不需要分顺序 
			number[i] = a + b;
			Search(n - 1);
			expression[i] = '(' + expa + '-' + expb + ')';  //减法应该分顺序，减数以及被减数 
			number[i] = a - b;
			Search(n - 1);
			expression[i] = '(' + expb + '-' + expa + ')';
			number[i] = b - a;
			Search(n - 1);
			expression[i] = '(' + expa + '*' + expb + ')';  //乘法不需要分顺序 
			number[i] = a*b;
			Search(n - 1);
			if (b != 0)
			{
				expression[i] = '(' + expa + '/' + expb + ')';  //除法应该分顺序，除数以及被除数 
				number[i] = a / b;
				Search(n - 1);
			}
			if (a != 0)
			{
				expression[i] = '(' + expb + '/' + expa + ')';
				number[i] = b / a;
				Search(n - 1);
			}
			number[i] = a;         //这4句语句是为了防止如果上面几种可能都失败了的话, 就把原来的赋值撤消回去,以无干扰的正确的进入到下一次for循环队列中。
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
	cout << "产生的四个随机数为:\n";
	for (int i = 0; i<4; i++)	cout << num[i] << " ";
	cout << endl;
	for (int i = 0; i<4; i++)
	{
		char buffer[20];
		number[i] = num[i];
		_itoa(number[i], buffer, 10);   //itoa()函数的作用是把第一个参数(数值)传送到第二个参数(字符串)中去，第三个  
									   //参数(int型)是该数值在字符串里以什么进制存放。  
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
	string infix;                       //原本的表达式
	string postfix;                     //逆波兰式，即后缀表达式
	string str;							//初始输入的字符串 
	stack<char> stc;                    //用于存放运算符的堆栈
	int res;							//存放最终计算结果 
	bool tflag = true;
	float t;
	int CharCompare(char c);            //字符比较
	void InfixTopostfix();              //表达式转换
	void Calculate();                   //计算结果
	int Oper(int num1, char op, int num2);//计算一个子表达式
	bool JudgeC(char c);				//用于判断是否是相应运算符 
	bool JudgeN(char c);				//用于判断是否是规定的数字 
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
			if (stc.empty() || c == '(')//堆栈为空或为（就直接入栈
				stc.push(c);
			else if (c == ')')//遇到）就一直弹栈，直到遇到（
			{
				while (stc.top() != '(')
				{
					postfix += stc.top();
					stc.pop();
				}
				stc.pop();//弹出'('，不留在堆栈中
			}
			else//遇到其它操作符先进行比较，再弹栈
			{

				while (CharCompare(stc.top()) >= CharCompare(c))//栈顶元素操作符的优先级大于当前操作符，则弹栈
				{
					postfix += stc.top();
					stc.pop();
					if (stc.empty())//防止弹栈过度
						break;
				}
				stc.push(c);//完成弹栈后插入
			}
		}
		else //数字直接插入后缀表达式中
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
	inst.push(*itstr - '0');//先压入一个数，防止堆栈为空
	++itstr;
	do
	{
		if (*itstr >= '0' && *itstr <= '9')//操作数直接压栈
		{
			inst.push(*itstr - '0');
			++itstr;
		}
		else if (*itstr == 'A')
		{
			inst.push(*itstr - '0');
			++itstr;
		}
		else//当遇到操作符时，取栈顶两个数进行运算
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
	//cout <<"计算得到的结果为："<<res<<endl;
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
	cout << "剩余时间为\n";
	//m1 = CreateThread(NULL, 0, Input, &str, 0, NULL);
	string str;
	std::thread m1([str](string s) {cin >> s;});
	int* exc = new int;
	gotoxy(0, 5);
	cout << "请输入你给出的答案(10请用A代替)" << endl;
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
		cout << "时间到！本轮结束！\n";
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
	string input;		//最初输入的答案
	char l;
	float lt;
	int score = 0;
	bool  ans;		//默认为答案为错 
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
	if (str.compare("No") == 0)	//假如输入的是No 
	{
		if (Work24::Work() == false)	ans = true;
		else cout << "本题有解，例如：" << Work24::ans << "=24" << endl;
	}
	else if (str.compare("root") == 0)		ans = true;
	else if (Prove::IsLegal() == true)
	{
		infix.assign(str);
		if (IsItem() == true)
		{
			InfixTopostfix();
			Calculate();
			cout << "根据表达式" << infix << "计算得到的结果为:" << Prove::res << endl;
			if (res == 24)				ans = true;
		}
		else
		{
			cout << "输入数字与题目预设数字不匹配\n";
			ans = false;
		}
	}
	else
	{
		cout << "输入不符合规范\n";
		ans = false;
	}
	if (ans == true)
	{
		cout << "所以答案正确\n";
		score = score + 10;
	}
	else 			cout << "所以答案有误\n";
}

void Game::Level()
{
	do
	{
		system("cls");
		cout << "请选择难度(S、A、B、C)\n";
		cout << "难度-做题时间的对应关系为：\n";
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
	cout << "您选的的难度为:" << l << ",您每次做题的时间为" << lt << "s" << endl;
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
		cout << "第" << i + 1 << "次开始\n";
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
	cout << "请输入用户名:";
	cin >> u1.name;
	Level();
	int j;
	system("cls");
	do
	{
		tflag = true;
		Start();
		system("cls");
		cout << "是否想要继续游戏？(1-继续游戏 其他键-退出游戏)\n";
		Sleep(500);
		cin.clear();
		cin >> j;
	} while (j == 1);
	u1.uscore = score;
	system("cls");
	cout << u1.name << "的最终的得分为:" << u1.uscore << endl;
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
	infile.close();             //关闭文件输入流 
}

void Menu()
{
	system("cls");
	cout << "******** 欢迎进入24点游戏 ********\n";
	cout << "******** 1. 开始游戏 ********\n";
	cout << "******** 2. 查询游戏积分 ********\n";
	cout << "******** 3. 退出游戏 ********\n";
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
