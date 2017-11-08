#pragma once
#include"Question.h"
#include<string>
#include<windows.h>
#include<algorithm>
#include<sstream>
#include<fstream>
//void PrintTime()//��ȡϵͳ��ǰʱ��
//{
//	while (1)
//	{
//		CTime t = CTime::GetCurrentTime();
//		cout << t.Format("%Y-%m-%d %H:%M:%S");
//		Sleep(1000);
//		cout << "\33[2K\r";
//	}
//}
class UI
{
private:
	QuestionOption qset; //��Ŀ����
	QuestionType qtype= QuestionType::Completion;//����
	int qnum=10; //����
	string UserName = "user";

	int Input(int min, int max)  // ��У������뺯��
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
	
public:
	UI() { srand(time(0)); }

	void Run()//�������
	{
		MenuMain();
	}

	void MenuMain()//���˵�
	{
		while (1)
		{
			cout << "*******************��ӭ����Сѧ�����̲���ϵͳ*********************" << endl;
			cout << "[1]���������û���\n"
				<< "[2]�Ѷ�����\n"
				<< "[3]��������\n"
				<< "[4]��������\n"
				<< "[5]����ʼ���⡿!\n"
				<< "[9]DEBUGģʽ\n"
				<< "[0]�˳�\n\n"
				<< "ע�⣺��������С���뱣��һλ����ĸ���ִ�Сд���ж���������T/F\n\n";
			cout << "��ѡ�����Ĳ���:";
			switch (Input(0, 9))
			{
			case 0:return;
			case 1:
				system("cls"); //����
				cout << "�����������û���:";
				cin >> UserName;
				system("cls"); //����
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
			case 9:
				Debug();
				break;
			}
		}
	}

	void MenuDifficultySetup() //���ò˵�
	{
		system("cls");
		cout <<"***************��Ŀ����*******************\n"
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
					qset.IncludeNegativeNumber = Input(0, 1)==1;
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
					cout << "�ݲ�֧��"<<endl;
					break;
			}
		}
	}

	void MenuTypeSetup()  //��������
	{
		system("cls");
		cout << "***************��������*******************\n"
			<< "[1]�����(Ĭ��)\n"
			<< "[2]ѡ����\n"
			<< "[3]�ж���\n"
			<< "[4]���ģʽ\n"
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

	void MenuAmountSetup()//��������
	{
		system("cls");
		cout << "***************��������*******************\n"
			<< "����������Ҫѵ������Ŀ����(Ĭ��10)�����س�ȷ��\n";
		auto n = Input(0, 10000);
		if (n == 0) return;
		else
		{
			qnum = n;
			cout << "����������Ϊ" << n << "�������������˵�" <<endl;
			Sleep(1500);
			system("cls");
		}
	}

	void Begin() //��ʼ����
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
				cout << "�������Բ��� �����ˣ� -1s" << endl;
				output << "�������Բ��� �����ˣ� -1s" << endl;
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

		ofstream file(UserName + ".txt");
		file << output.str();
		file.close();

		cin.get(); //��Enter���������˵�
		cin.get();

		system("cls"); //����
	}

	void Debug()
	{
		//for (int i = 0; i < 10; i++)
		//{
		//	qset.IncludeBrackets = true;
		//	qset.MaxNumbersCount = 3;
		//	qset.MaxNumbersRange = 100;
		//	Question a = Question(qset, qtype);
		//	a.Generate();
		//	auto b = a.GetExpression();
		//	
		//	cout << b.ToString() << endl;
		//	b.RemoveUselessBrackets();
		//	cout << b.ToString() << endl;
		//	cout << endl;
		//}
		cout << setprecision(1) << 10 << endl;
	}
};