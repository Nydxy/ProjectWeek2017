#pragma once

//��ͷ����ѭ��������
class Circlist
{
protected:
	int len;//�����н�����  
	Person* Head; //ָ��ͷ���  
	
public:
	Circlist();//Ĭ�Ϲ��캯��  
	~Circlist();
	void insertLast(Person * newPerson);//β����
	void Play(int N); //��ʼ������Ϸ
	void Print();
};

Circlist::Circlist() //��ʼ��ʱ��ֻ��һ��ͷ��㣬��headָ��  
{
	Head = new Person(0,0);
	Head->next = Head;
	len = 0;
}

Circlist::~Circlist()
{
	Person* current;
	Person* temp;
	if (Head != NULL)
	{
		temp = Head;
		current = Head->next;
		while (current != Head)
		{
			delete temp;
			temp = current;
			current = current->next;
		}
		delete temp;
		len = 0;
	}
}

void Circlist::insertLast(Person* newPerson)
{
	Person* current = Head;
	while (current->next != Head)	current = current->next;
	newPerson->next = current->next;
	current->next = newPerson;
	len++;
}

void Circlist::Play(int N) //��ʼ������Ϸ
{
	Person* CurrentPerson = Head->next;  //��ǰ���ָ���һ����
	Person* PreviousPerson = Head;  //������¼��һ���ڵ㣨����ɾ���ڵ㣩
	int m = GetRandom(1, N); //��һ�α�����ֵ
	while (CurrentPerson->next != CurrentPerson) //����Ҷ���ǿյ�ʱ��ѭ����Ϸ
	{
		cout << "��" << CurrentPerson->index << "��ʼ 	";
		for (int i = 0; i < m - 1; i++)//��ǰ��������m-1��
		{
			PreviousPerson = CurrentPerson;
			CurrentPerson = CurrentPerson->next;
			if (CurrentPerson->index == 0)  //�����ǰ���ָ����ͷ��㣬Ҫ���˳��һ��
			{
				PreviousPerson = CurrentPerson;
				CurrentPerson = CurrentPerson->next;
			}
		}
		cout<<"m="<<m<<"	" << CurrentPerson->index << "����	";
		m = CurrentPerson->code; //��һ�ε�mֵ
		//ɾ����ǰ�ڵ�
		PreviousPerson->next = CurrentPerson->next;
		delete CurrentPerson;
		if (PreviousPerson->next == Head) PreviousPerson = Head;
		CurrentPerson = PreviousPerson->next;
		Print();
	}
};

void Circlist::Print() //��ӡ����б�
{
	Person* cur = Head->next;
	while (cur != Head)
	{
		cout << cur->index << " ";
		cur = cur->next;
	}
	cout << endl;
}