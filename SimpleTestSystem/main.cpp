#include"Problem.h"
#include<time.h>
#include<fstream>

ProblemSet pset; //题目设置
ProblemType ptype = Completion;//题型
int pnum = 10; //题量
string UserName = "user";//用户名

void nandu() //设置菜单
{
	system("cls");
	int n;
	cout << "***************题目设置*******************\n"
		<< "1.运算数范围(默认20)\n"
		<< "2.运算数数量(默认2)\n"
		<< "3.是否包含负数(默认0)\n"
		<< "4.是否包含小数(默认0)\n"
		<< "5.是否包含括号(默认0)\n"
		<< "0.返回上一层菜单\n";
	while (1)
	{
		cout << "\n请选择您的操作:";
		cin >> n;
		switch (n)
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "运算数范围(默认100) | 10-最小 10000-最大:";
			cin>>pset.numrange;
			break;
		case 2:
			cout << "运算数数量(默认3) | 2-最小 10-最大:";
			cin>>pset.numcount;
			break;
		case 3:
			cout << "是否包含负数(默认0) | 0-不包含 1-包含:";
			int negative;
			cin >> negative;
			pset.hasFushu = negative == 1;
			break;
		case 4:
			cout << "是否包含小数(默认0) | 0-不包含 1-包含:";
			int xiaoshu;
			cin >> xiaoshu;
			pset.hasXiaoshu = xiaoshu == 1;
			break;
		case 5:
			cout << "是否包含括号(默认0) | 0-不包含 1-包含:";
			int kuohao;
			cin >> kuohao;
			pset.hasKuohao = kuohao == 1;
			break;
		}
	}
}

void tixing()  //题型设置
{
	system("cls");
	cout << "***************题型设置*******************\n"
		<< "1.填空题(默认)\n"
		<< "2.选择题\n"
		<< "3.判断题\n"
		<< "0.返回上一层菜单\n";

	cout << "\n请选择您的操作:";
	int n;
	cin >> n;
	switch (n)
	{
	case 0:
		system("cls");
		return;
	case 1:
		ptype = Completion;
		break;
	case 2:
		ptype = Choice;
		break;
	case 3:
		ptype = TrueFalse;
		break;
	}
	system("cls");
}

void shuliang()//题量设置
{
	system("cls");
	cout << "***************题量设置*******************\n"
		<< "请输入您需要训练的题目数量(默认10)\n";
	int n;
	cin >> n;
	if (n == 0)
	{
		return;
	}
	else
	{
		pnum = n;
	}
	system("cls");
}

void Begin() //开始做题
{
	system("cls");
	ofstream output(UserName + ".txt");

	cout << "*********************开始答题************************\n";
	output << "*********************开始答题************************\n";
	float score = 0; //得分
	float eachscore = 100.0 / pnum; //每题分值
	int rightcount = 0; //答对计数

	for (int i = 0; i < pnum; i++)
	{
		Problem a = Problem(pset, ptype);
		a.create();
		cout << "[" << i + 1 << "/" << pnum << "] ";
		output << "[" << i + 1 << "/" << pnum << "] ";
		cout << a.question << endl;
		output << a.question << endl;
		cout << "你的答案是：";
		output << "你的答案是：";
		string str;
		cin >> str;
		output << str << endl;
		if (a.answer == str)
		{
			cout << "正确" << endl;
			output << "正确" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "错误，答案是"<< a.answer << endl;
			output << "错误，答案是" << a.answer << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "测试结束，您答对了：" << rightcount << "/" << pnum << " , 得分： " << score << endl;
	output << "测试结束，您答对了：" << rightcount << "/" << pnum << " , 得分： " << score << endl;

	cout <<"测试题目、答案及结果已保存到"<< UserName + ".txt" << "\n\n****************按Enter键返回主菜单********************\n";
	output << "测试题目、答案及结果已保存到" << UserName + ".txt" << "\n\n****************按Enter键返回主菜单********************\n";

	output.close();

	cin.get(); //按Enter键返回主菜单
	cin.get();

	system("cls"); //清屏
}

int main()
{
	srand(time(0));
	while (1)
	{
		cout << "*******************四则运算自测系统*********************" << endl;
		cout << "1.设置您的用户名\n"
			<< "2.难度设置\n"
			<< "3.题型设置\n"
			<< "4.数量设置\n"
			<< "5.开始\n"
			<< "0.退出\n\n";
		cout << "请选择您的操作:";
		int n;
		cin >> n;
		switch (n)
		{
		case 0:return 0;
		case 1:
			system("cls"); //清屏
			cout << "请输入您的用户名:";
			cin >> UserName;
			system("cls"); //清屏
			continue;
		case 2:
			nandu();
			continue;
		case 3:
			tixing();
			continue;
		case 4:
			shuliang();
			continue;
		case 5:
			Begin();
			break;
		}
	}
	return 0;
}