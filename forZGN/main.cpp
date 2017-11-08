#include"Question.h"
#include<windows.h>
#include<algorithm>
#include<fstream>

#pragma region Global
QuestionOption qset; //��Ŀ����
QuestionType qtype = Completion;//����
int qnum = 10; //����
string UserName = "user";//�û���
#pragma endregion

#pragma region UI
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

void MenuDifficultySetup() //���ò˵�
{
	system("cls");
	cout << "***************��Ŀ����*******************\n"
		<< "[1]��������Χ(Ĭ��20)\n"
		<< "[2]����������(Ĭ��2)\n"
		<< "[3]�Ƿ��������(Ĭ��0)\n"
		<< "[4]�Ƿ����С��(Ĭ��0)\n"
		<< "[5]�Ƿ��������(Ĭ��0)\n"
		<< "[0]������һ��˵�\n";
	while (1)
	{
		cout << "\n��ѡ�����Ĳ���:";
		switch (Input(0, 5))
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
		<< "[0]������һ��˵�\n";

	cout << "\n��ѡ�����Ĳ���:";
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

void MenuAmountSetup()//��������
{
	system("cls");
	cout << "***************��������*******************\n"
		<< "����������Ҫѵ������Ŀ����(Ĭ��10)\n";
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
			cout << "��ȷ" << endl;
			output << "��ȷ" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "���󣬴���"<< a.AnswerString << endl;
			output << "���󣬴���" << a.AnswerString << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "���Խ�����������ˣ�" << rightcount << "/" << qnum << " , �÷֣� " << score << endl;
	output << "���Խ�����������ˣ�" << rightcount << "/" << qnum << " , �÷֣� " << score << endl;

	cout <<"������Ŀ���𰸼�����ѱ��浽"<< UserName + ".txt" << "\n\n****************��Enter���������˵�********************\n";
	output << "������Ŀ���𰸼�����ѱ��浽" << UserName + ".txt" << "\n\n****************��Enter���������˵�********************\n";

	ofstream file(UserName + ".txt");
	file << output.str();
	file.close();

	cin.get(); //��Enter���������˵�
	cin.get();

	system("cls"); //����
}

void MenuMain()//���˵�
{
	while (1)
	{
		cout << "*******************Сѧ�����������Բ�ϵͳ*********************" << endl;
		cout << "[1]���������û���\n"
			<< "[2]�Ѷ�����\n"
			<< "[3]��������\n"
			<< "[4]��������\n"
			<< "[5]����ʼ���⡿!\n"
			<< "[0]�˳�\n\n"
			<< "ע�⣺��������С���뱣��һλ����ĸ���ִ�Сд���ж���������T/F\n\n";
		cout << "��ѡ�����Ĳ���:";
		switch (Input(0, 5))
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