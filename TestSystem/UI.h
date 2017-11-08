#pragma once
#include"Question.h"
#include<string>
#include<windows.h>
#include<algorithm>
#include<sstream>
#include<fstream>
#include<map>

class UI
{
private:
	QuestionOption qset; //题目设置
	QuestionType qtype= QuestionType::Completion;//题型
	int qnum=10; //题量
	string UserName = "user";  //用户名
	map<string, int> users;  //用户列表

	int Input(int min, int max);// 带校验的输入
	
public:
	UI() { srand(time(0)); }
	void Run();//运行入口
	void MenuMain();//主菜单
	void MenuDifficultySetup();//设置菜单
	void MenuAdvancedSetup();//高级难度设置
	void MenuTypeSetup();//题型设置
	void MenuAmountSetup();//题量设置
	void Begin();//开始做题
	int LoadUsers(string name);//载入用户列表，返回指定用户的积分
	void SaveUsers();//保存用户数据到文件
};

int UI::Input(int min, int max)// 带校验的输入
{
	int n;
	cin >> n;
	while (cin.fail() || n<min || n>max)
	{
		cout << "您的输入有误，请重新输入:";
		cin.clear();
		cin.ignore(1024, '\n'); //清除缓冲区当前行
		cin >> n;
	}
	return n;
}

void UI::Run()//运行入口
{
	MenuMain();
	SaveUsers();
}

void UI::MenuMain()//主菜单
{
	cout << "请输入您的用户名：";
	cin >> UserName;
	int userscore = LoadUsers(UserName); //载入用户列表，获取用户积分
	system("cls");

	while (1)
	{
		cout << "*******************欢迎来到小学生智商测试系统*********************" << endl;
		cout << "您好，" << UserName << "，您的积分是" << userscore << endl << endl
			<< "[1]难度设置\n"
			<< "[2]题型设置\n"
			<< "[3]数量设置\n"
			<< "[4]【开始答题】!\n"
			<< "[0]退出\n\n"
			<< "注意：如果结果是小数请保留一位，字母不分大小写，判断题请输入T/F\n\n";
		cout << "请选择您的操作:";
		switch (Input(0, 5))
		{
		case 0:return;
		case 1:
			MenuDifficultySetup();
			continue;
		case 2:
			MenuTypeSetup();
			continue;
		case 3:
			MenuAmountSetup();
			continue;
		case 4:
			Begin();
			break;
		}
	}
}

void UI::MenuDifficultySetup() //设置菜单
{
	system("cls");
	cout << "***************难度设置*******************\n"
		<< "[1]运算数范围(默认20)\n"
		<< "[2]运算数数量(默认2)\n"
		<< "[3]是否包含负数(默认0)\n"
		<< "[4]是否包含小数(默认0)\n"
		<< "[5]是否包含括号(默认0)\n"
		<< "[6]高级设置\n"
		<< "[0]返回上一层菜单\n";
	while (1)
	{
		cout << "\n请选择您的操作:";
		switch (Input(0, 6))
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "运算数范围(默认100) | 10-最小 10000-最大:";
			qset.MaxNumbersRange = Input(10, 10000);
			break;
		case 2:
			cout << "运算数数量(默认3) | 2-最小 10-最大:";
			qset.MaxNumbersCount = Input(2, 10);
			break;
		case 3:
			cout << "是否包含负数(默认0) | 0-不包含 1-包含:";
			qset.IncludeNegativeNumber = Input(0, 1) == 1;
			break;
		case 4:
			cout << "是否包含小数(默认0) | 0-不包含 1-包含:";
			qset.IncludeDecimal = Input(0, 1) == 1;
			break;
		case 5:
			cout << "是否包含括号(默认0) | 0-不包含 1-包含:";
			qset.IncludeBrackets = Input(0, 1) == 1;
			break;
		case 6:
			MenuAdvancedSetup();
			cout << "***************难度设置*******************\n"
				<< "[1]运算数范围(默认20)\n"
				<< "[2]运算数数量(默认2)\n"
				<< "[3]是否包含负数(默认0)\n"
				<< "[4]是否包含小数(默认0)\n"
				<< "[5]是否包含括号(默认0)\n"
				<< "[6]高级设置\n"
				<< "[0]返回上一层菜单\n";
			break;
		}
	}
}

void UI::MenuAdvancedSetup() //高级难度设置
{
	system("cls");
	cout << "***************高级设置*******************\n"
		<< "[1]出现括号的概率(默认0.8)\n"
		<< "[2]出现小数的概率(默认0.3)\n"
		<< "[3]出现负数的概率(默认0.3)\n"
		<< "[0]返回上一层菜单\n";
	while (1)
	{
		cout << "\n请选择您的操作:";
		switch (Input(0, 3))
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "出现括号的概率(默认0.8): ";
			cin >> qset.ProbabilityOfBrackets;
			break;
		case 2:
			cout << "出现小数的概率(默认0.3): ";
			cin >> qset.ProbabilityOfDecimal;
			break;
		case 3:
			cout << "出现负数的概率(默认0.3): ";
			cin >> qset.ProbabilityOfNegative;
			break;
		}
	}
}

void UI::MenuTypeSetup()  //题型设置
{
	system("cls");
	cout << "***************题型设置*******************\n"
		<< "[1]填空题(默认)\n"
		<< "[2]选择题\n"
		<< "[3]判断题\n"
		//<< "[4]混合模式\n"
		<< "[0]返回上一层菜单\n";

	cout << "\n请选择您的操作:";
	switch (Input(0, 4))
	{
	case 0:
		system("cls");
		return;
	case 1:
		qtype = QuestionType::Completion;
		cout << "题型已设置为 填空题，即将返回主菜单" << endl;
		break;
	case 2:
		qtype = QuestionType::Choice;
		cout << "题型已设置为 选择题，即将返回主菜单" << endl;
		break;
	case 3:
		qtype = QuestionType::TrueFalse;
		cout << "题型已设置为 判断题，即将返回主菜单" << endl;
		break;
	case 4:
		//TODO: 混合出题模式
		break;
	}
	Sleep(1500);
	system("cls");
}

void UI::MenuAmountSetup()//题量设置
{
	system("cls");
	cout << "***************题量设置*******************\n"
		<< "请输入您需要训练的题目数量(默认10)，按回车确认\n";
	auto n = Input(0, 10000);
	if (n == 0) return;
	else
	{
		qnum = n;
		cout << "题量已设置为" << n << "，即将返回主菜单" << endl;
		Sleep(1500);
		system("cls");
	}
}

void UI::Begin() //开始做题
{
	system("cls");
	stringstream output; //使用stringstream来保存输入输出

	cout << "*********************开始答题************************\n";
	output << "*********************开始答题************************\n";
	float score = 0; //得分
	float eachscore = 100.0 / qnum; //每题分值
	int rightcount = 0; //答对计数

	for (int i = 0; i < qnum; i++)
	{
		Question a = Question(qset, qtype);
		a.Generate();
		cout << "[" << i + 1 << "/" << qnum << "] ";
		output << "[" << i + 1 << "/" << qnum << "] ";
		cout << a.QuestionContent << endl;
		output << a.QuestionContent << endl;
		cout << "你的答案是：";
		output << "你的答案是：";
		string str;
		cin >> str;
		output << str << endl;
		transform(str.begin(), str.end(), str.begin(), ::toupper); //将输入转换成大写
		if (a.AnswerString == str)
		{
			cout << "【√】恭喜你 答对了！ +1s" << endl;
			output << "【√】恭喜你 答对了！ +1s" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "【×】对不起 你答错了，答案是" << a.AnswerString << endl;
			output << "【×】对不起 你答错了，答案是" << a.AnswerString << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "测试结束，您的得分是：" << score << endl;
	output << "测试结束，您的得分是：" << score << endl;
	cout << "答对 " << rightcount << "/" << qnum << " , 正确率 " << score << "%\n\n";
	output << "答对 " << rightcount << "/" << qnum << " , 正确率 " << score << "%\n\n";

	if (score < 60)
	{
		cout << "不及格哦！您的智商可能需要充值！\n";
		output << "不及格哦！您的智商可能需要充值！\n";
	}
	if (score == 100)
	{
		cout << "哇，全对，您的智商在小学生中无敌！\n";
		output << "哇，全对，您的智商在小学生中无敌！\n";
	}

	cout << "\n****************按Enter键返回主菜单********************\n";
	output << "\n****************按Enter键返回主菜单********************\n";

	//输出本次测试内容
	ofstream file(UserName + ".txt");
	file << output.str();
	file.close();

	users.find(UserName)->second += rightcount;

	cin.get(); //按Enter键返回主菜单
	cin.get();

	system("cls"); //清屏
}

int UI::LoadUsers(string name) //载入用户列表，返回指定用户的积分
{
	ifstream file("save.data");

	string str;
	stringstream line;
	string username;
	int score;
	int result = 0;
	bool userexist = false;

	while (getline(file, str))
	{
		if (str.length() < 2) continue;
		line << str;
		line >> username >> score;
		users.insert(pair<string, int>(username, score));
		if (username == name)
		{
			result = score;
			userexist = true;
		}
		line.clear();
	}
	if (!userexist) //如果用户不存在，新建一个用户
		users.insert(pair<string, int>(name, 0));

	file.close();
	return result;
}

void UI::SaveUsers() //保存用户数据到文件
{
	ofstream file("save.data");
	for each (auto u in users)
	{
		file << u.first << " " << u.second << endl;
	}
	file.close();
}