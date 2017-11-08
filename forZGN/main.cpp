#include"Question.h"
#include<windows.h>
#include<algorithm>
#include<fstream>

#pragma region Global
QuestionOption qset; //题目设置
QuestionType qtype = Completion;//题型
int qnum = 10; //题量
string UserName = "user";//用户名
#pragma endregion

#pragma region UI
int Input(int min, int max)  // 带校验的输入函数
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

void MenuDifficultySetup() //设置菜单
{
	system("cls");
	cout << "***************题目设置*******************\n"
		<< "[1]运算数范围(默认20)\n"
		<< "[2]运算数数量(默认2)\n"
		<< "[3]是否包含负数(默认0)\n"
		<< "[4]是否包含小数(默认0)\n"
		<< "[5]是否包含括号(默认0)\n"
		<< "[0]返回上一层菜单\n";
	while (1)
	{
		cout << "\n请选择您的操作:";
		switch (Input(0, 5))
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
		}
	}
}

void MenuTypeSetup()  //题型设置
{
	system("cls");
	cout << "***************题型设置*******************\n"
		<< "[1]填空题(默认)\n"
		<< "[2]选择题\n"
		<< "[3]判断题\n"
		<< "[0]返回上一层菜单\n";

	cout << "\n请选择您的操作:";
	switch (Input(0, 3))
	{
	case 0:
		system("cls");
		return;
	case 1:
		qtype = Completion;
		break;
	case 2:
		qtype = Choice;
		break;
	case 3:
		qtype = TrueFalse;
		break;
	}
	system("cls");
}

void MenuAmountSetup()//题量设置
{
	system("cls");
	cout << "***************题量设置*******************\n"
		<< "请输入您需要训练的题目数量(默认10)\n";
	int n = Input(0, 10000);
	if (n == 0)
	{
		return;
	}
	else
	{
		qnum = n;
	}
	system("cls");
}

void Begin() //开始做题
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
			cout << "正确" << endl;
			output << "正确" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "错误，答案是"<< a.AnswerString << endl;
			output << "错误，答案是" << a.AnswerString << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "测试结束，您答对了：" << rightcount << "/" << qnum << " , 得分： " << score << endl;
	output << "测试结束，您答对了：" << rightcount << "/" << qnum << " , 得分： " << score << endl;

	cout <<"测试题目、答案及结果已保存到"<< UserName + ".txt" << "\n\n****************按Enter键返回主菜单********************\n";
	output << "测试题目、答案及结果已保存到" << UserName + ".txt" << "\n\n****************按Enter键返回主菜单********************\n";

	ofstream file(UserName + ".txt");
	file << output.str();
	file.close();

	cin.get(); //按Enter键返回主菜单
	cin.get();

	system("cls"); //清屏
}

void MenuMain()//主菜单
{
	while (1)
	{
		cout << "*******************小学生四则运算自测系统*********************" << endl;
		cout << "[1]设置您的用户名\n"
			<< "[2]难度设置\n"
			<< "[3]题型设置\n"
			<< "[4]数量设置\n"
			<< "[5]【开始答题】!\n"
			<< "[0]退出\n\n"
			<< "注意：如果结果是小数请保留一位，字母不分大小写，判断题请输入T/F\n\n";
		cout << "请选择您的操作:";
		switch (Input(0, 5))
		{
		case 0:return;
		case 1:
			system("cls"); //清屏
			cout << "请输入您的用户名:";
			cin >> UserName;
			system("cls"); //清屏
			continue;
		case 2:
			MenuDifficultySetup();
			continue;
		case 3:
			MenuTypeSetup();
			continue;
		case 4:
			MenuAmountSetup();
			continue;
		case 5:
			Begin();
			break;
		}
	}
}
#pragma endregion

int main()
{
	srand(time(0));
	MenuMain();
	return 0;
}