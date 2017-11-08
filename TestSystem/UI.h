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
	QuestionOption qset; //��Ŀ����
	QuestionType qtype= QuestionType::Completion;//����
	int qnum=10; //����
	string UserName = "user";  //�û���
	map<string, int> users;  //�û��б�

	int Input(int min, int max);// ��У�������
	
public:
	UI() { srand(time(0)); }
	void Run();//�������
	void MenuMain();//���˵�
	void MenuDifficultySetup();//���ò˵�
	void MenuAdvancedSetup();//�߼��Ѷ�����
	void MenuTypeSetup();//��������
	void MenuAmountSetup();//��������
	void Begin();//��ʼ����
	int LoadUsers(string name);//�����û��б�����ָ���û��Ļ���
	void SaveUsers();//�����û����ݵ��ļ�
};

int UI::Input(int min, int max)// ��У�������
{
	int n;
	cin >> n;
	while (cin.fail() || n<min || n>max)
	{
		cout << "����������������������:";
		cin.clear();
		cin.ignore(1024, '\n'); //�����������ǰ��
		cin >> n;
	}
	return n;
}

void UI::Run()//�������
{
	MenuMain();
	SaveUsers();
}

void UI::MenuMain()//���˵�
{
	cout << "�����������û�����";
	cin >> UserName;
	int userscore = LoadUsers(UserName); //�����û��б���ȡ�û�����
	system("cls");

	while (1)
	{
		cout << "*******************��ӭ����Сѧ�����̲���ϵͳ*********************" << endl;
		cout << "���ã�" << UserName << "�����Ļ�����" << userscore << endl << endl
			<< "[1]�Ѷ�����\n"
			<< "[2]��������\n"
			<< "[3]��������\n"
			<< "[4]����ʼ���⡿!\n"
			<< "[0]�˳�\n\n"
			<< "ע�⣺��������С���뱣��һλ����ĸ���ִ�Сд���ж���������T/F\n\n";
		cout << "��ѡ�����Ĳ���:";
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

void UI::MenuDifficultySetup() //���ò˵�
{
	system("cls");
	cout << "***************�Ѷ�����*******************\n"
		<< "[1]��������Χ(Ĭ��20)\n"
		<< "[2]����������(Ĭ��2)\n"
		<< "[3]�Ƿ��������(Ĭ��0)\n"
		<< "[4]�Ƿ����С��(Ĭ��0)\n"
		<< "[5]�Ƿ��������(Ĭ��0)\n"
		<< "[6]�߼�����\n"
		<< "[0]������һ��˵�\n";
	while (1)
	{
		cout << "\n��ѡ�����Ĳ���:";
		switch (Input(0, 6))
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "��������Χ(Ĭ��100) | 10-��С 10000-���:";
			qset.MaxNumbersRange = Input(10, 10000);
			break;
		case 2:
			cout << "����������(Ĭ��3) | 2-��С 10-���:";
			qset.MaxNumbersCount = Input(2, 10);
			break;
		case 3:
			cout << "�Ƿ��������(Ĭ��0) | 0-������ 1-����:";
			qset.IncludeNegativeNumber = Input(0, 1) == 1;
			break;
		case 4:
			cout << "�Ƿ����С��(Ĭ��0) | 0-������ 1-����:";
			qset.IncludeDecimal = Input(0, 1) == 1;
			break;
		case 5:
			cout << "�Ƿ��������(Ĭ��0) | 0-������ 1-����:";
			qset.IncludeBrackets = Input(0, 1) == 1;
			break;
		case 6:
			MenuAdvancedSetup();
			cout << "***************�Ѷ�����*******************\n"
				<< "[1]��������Χ(Ĭ��20)\n"
				<< "[2]����������(Ĭ��2)\n"
				<< "[3]�Ƿ��������(Ĭ��0)\n"
				<< "[4]�Ƿ����С��(Ĭ��0)\n"
				<< "[5]�Ƿ��������(Ĭ��0)\n"
				<< "[6]�߼�����\n"
				<< "[0]������һ��˵�\n";
			break;
		}
	}
}

void UI::MenuAdvancedSetup() //�߼��Ѷ�����
{
	system("cls");
	cout << "***************�߼�����*******************\n"
		<< "[1]�������ŵĸ���(Ĭ��0.8)\n"
		<< "[2]����С���ĸ���(Ĭ��0.3)\n"
		<< "[3]���ָ����ĸ���(Ĭ��0.3)\n"
		<< "[0]������һ��˵�\n";
	while (1)
	{
		cout << "\n��ѡ�����Ĳ���:";
		switch (Input(0, 3))
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "�������ŵĸ���(Ĭ��0.8): ";
			cin >> qset.ProbabilityOfBrackets;
			break;
		case 2:
			cout << "����С���ĸ���(Ĭ��0.3): ";
			cin >> qset.ProbabilityOfDecimal;
			break;
		case 3:
			cout << "���ָ����ĸ���(Ĭ��0.3): ";
			cin >> qset.ProbabilityOfNegative;
			break;
		}
	}
}

void UI::MenuTypeSetup()  //��������
{
	system("cls");
	cout << "***************��������*******************\n"
		<< "[1]�����(Ĭ��)\n"
		<< "[2]ѡ����\n"
		<< "[3]�ж���\n"
		//<< "[4]���ģʽ\n"
		<< "[0]������һ��˵�\n";

	cout << "\n��ѡ�����Ĳ���:";
	switch (Input(0, 4))
	{
	case 0:
		system("cls");
		return;
	case 1:
		qtype = QuestionType::Completion;
		cout << "����������Ϊ ����⣬�����������˵�" << endl;
		break;
	case 2:
		qtype = QuestionType::Choice;
		cout << "����������Ϊ ѡ���⣬�����������˵�" << endl;
		break;
	case 3:
		qtype = QuestionType::TrueFalse;
		cout << "����������Ϊ �ж��⣬�����������˵�" << endl;
		break;
	case 4:
		//TODO: ��ϳ���ģʽ
		break;
	}
	Sleep(1500);
	system("cls");
}

void UI::MenuAmountSetup()//��������
{
	system("cls");
	cout << "***************��������*******************\n"
		<< "����������Ҫѵ������Ŀ����(Ĭ��10)�����س�ȷ��\n";
	auto n = Input(0, 10000);
	if (n == 0) return;
	else
	{
		qnum = n;
		cout << "����������Ϊ" << n << "�������������˵�" << endl;
		Sleep(1500);
		system("cls");
	}
}

void UI::Begin() //��ʼ����
{
	system("cls");
	stringstream output; //ʹ��stringstream�������������

	cout << "*********************��ʼ����************************\n";
	output << "*********************��ʼ����************************\n";
	float score = 0; //�÷�
	float eachscore = 100.0 / qnum; //ÿ���ֵ
	int rightcount = 0; //��Լ���

	for (int i = 0; i < qnum; i++)
	{
		Question a = Question(qset, qtype);
		a.Generate();
		cout << "[" << i + 1 << "/" << qnum << "] ";
		output << "[" << i + 1 << "/" << qnum << "] ";
		cout << a.QuestionContent << endl;
		output << a.QuestionContent << endl;
		cout << "��Ĵ��ǣ�";
		output << "��Ĵ��ǣ�";
		string str;
		cin >> str;
		output << str << endl;
		transform(str.begin(), str.end(), str.begin(), ::toupper); //������ת���ɴ�д
		if (a.AnswerString == str)
		{
			cout << "���̡���ϲ�� ����ˣ� +1s" << endl;
			output << "���̡���ϲ�� ����ˣ� +1s" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "�������Բ��� �����ˣ�����" << a.AnswerString << endl;
			output << "�������Բ��� �����ˣ�����" << a.AnswerString << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "���Խ��������ĵ÷��ǣ�" << score << endl;
	output << "���Խ��������ĵ÷��ǣ�" << score << endl;
	cout << "��� " << rightcount << "/" << qnum << " , ��ȷ�� " << score << "%\n\n";
	output << "��� " << rightcount << "/" << qnum << " , ��ȷ�� " << score << "%\n\n";

	if (score < 60)
	{
		cout << "������Ŷ���������̿�����Ҫ��ֵ��\n";
		output << "������Ŷ���������̿�����Ҫ��ֵ��\n";
	}
	if (score == 100)
	{
		cout << "�ۣ�ȫ�ԣ�����������Сѧ�����޵У�\n";
		output << "�ۣ�ȫ�ԣ�����������Сѧ�����޵У�\n";
	}

	cout << "\n****************��Enter���������˵�********************\n";
	output << "\n****************��Enter���������˵�********************\n";

	//������β�������
	ofstream file(UserName + ".txt");
	file << output.str();
	file.close();

	users.find(UserName)->second += rightcount;

	cin.get(); //��Enter���������˵�
	cin.get();

	system("cls"); //����
}

int UI::LoadUsers(string name) //�����û��б�����ָ���û��Ļ���
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
	if (!userexist) //����û������ڣ��½�һ���û�
		users.insert(pair<string, int>(name, 0));

	file.close();
	return result;
}

void UI::SaveUsers() //�����û����ݵ��ļ�
{
	ofstream file("save.data");
	for each (auto u in users)
	{
		file << u.first << " " << u.second << endl;
	}
	file.close();
}