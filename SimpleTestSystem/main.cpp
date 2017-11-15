#include"Problem.h"
#include<time.h>
#include<fstream>

ProblemSet pset; //��Ŀ����
ProblemType ptype = Completion;//����
int pnum = 10; //����
string UserName = "user";//�û���

void nandu() //���ò˵�
{
	system("cls");
	int n;
	cout << "***************��Ŀ����*******************\n"
		<< "1.��������Χ(Ĭ��20)\n"
		<< "2.����������(Ĭ��2)\n"
		<< "3.�Ƿ��������(Ĭ��0)\n"
		<< "4.�Ƿ����С��(Ĭ��0)\n"
		<< "5.�Ƿ��������(Ĭ��0)\n"
		<< "0.������һ��˵�\n";
	while (1)
	{
		cout << "\n��ѡ�����Ĳ���:";
		cin >> n;
		switch (n)
		{
		case 0:
			system("cls");
			return;
		case 1:
			cout << "��������Χ(Ĭ��100) | 10-��С 10000-���:";
			cin>>pset.numrange;
			break;
		case 2:
			cout << "����������(Ĭ��3) | 2-��С 10-���:";
			cin>>pset.numcount;
			break;
		case 3:
			cout << "�Ƿ��������(Ĭ��0) | 0-������ 1-����:";
			int negative;
			cin >> negative;
			pset.hasFushu = negative == 1;
			break;
		case 4:
			cout << "�Ƿ����С��(Ĭ��0) | 0-������ 1-����:";
			int xiaoshu;
			cin >> xiaoshu;
			pset.hasXiaoshu = xiaoshu == 1;
			break;
		case 5:
			cout << "�Ƿ��������(Ĭ��0) | 0-������ 1-����:";
			int kuohao;
			cin >> kuohao;
			pset.hasKuohao = kuohao == 1;
			break;
		}
	}
}

void tixing()  //��������
{
	system("cls");
	cout << "***************��������*******************\n"
		<< "1.�����(Ĭ��)\n"
		<< "2.ѡ����\n"
		<< "3.�ж���\n"
		<< "0.������һ��˵�\n";

	cout << "\n��ѡ�����Ĳ���:";
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

void shuliang()//��������
{
	system("cls");
	cout << "***************��������*******************\n"
		<< "����������Ҫѵ������Ŀ����(Ĭ��10)\n";
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

void Begin() //��ʼ����
{
	system("cls");
	ofstream output(UserName + ".txt");

	cout << "*********************��ʼ����************************\n";
	output << "*********************��ʼ����************************\n";
	float score = 0; //�÷�
	float eachscore = 100.0 / pnum; //ÿ���ֵ
	int rightcount = 0; //��Լ���

	for (int i = 0; i < pnum; i++)
	{
		Problem a = Problem(pset, ptype);
		a.create();
		cout << "[" << i + 1 << "/" << pnum << "] ";
		output << "[" << i + 1 << "/" << pnum << "] ";
		cout << a.question << endl;
		output << a.question << endl;
		cout << "��Ĵ��ǣ�";
		output << "��Ĵ��ǣ�";
		string str;
		cin >> str;
		output << str << endl;
		if (a.answer == str)
		{
			cout << "��ȷ" << endl;
			output << "��ȷ" << endl;
			score += eachscore;
			rightcount++;
		}
		else
		{
			cout << "���󣬴���"<< a.answer << endl;
			output << "���󣬴���" << a.answer << endl;
		}
		cout << endl;
		output << endl;
	}

	cout << "********************************\n\n";
	output << "********************************\n\n";
	cout << "���Խ�����������ˣ�" << rightcount << "/" << pnum << " , �÷֣� " << score << endl;
	output << "���Խ�����������ˣ�" << rightcount << "/" << pnum << " , �÷֣� " << score << endl;

	cout <<"������Ŀ���𰸼�����ѱ��浽"<< UserName + ".txt" << "\n\n****************��Enter���������˵�********************\n";
	output << "������Ŀ���𰸼�����ѱ��浽" << UserName + ".txt" << "\n\n****************��Enter���������˵�********************\n";

	output.close();

	cin.get(); //��Enter���������˵�
	cin.get();

	system("cls"); //����
}

int main()
{
	srand(time(0));
	while (1)
	{
		cout << "*******************���������Բ�ϵͳ*********************" << endl;
		cout << "1.���������û���\n"
			<< "2.�Ѷ�����\n"
			<< "3.��������\n"
			<< "4.��������\n"
			<< "5.��ʼ\n"
			<< "0.�˳�\n\n";
		cout << "��ѡ�����Ĳ���:";
		int n;
		cin >> n;
		switch (n)
		{
		case 0:return 0;
		case 1:
			system("cls"); //����
			cout << "�����������û���:";
			cin >> UserName;
			system("cls"); //����
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