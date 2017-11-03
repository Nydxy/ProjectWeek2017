#pragma once

//带头结点的循环链表类
class Circlist
{
protected:
	int len;//链表中结点个数  
	Person* Head; //指向头结点  
	
public:
	Circlist();//默认构造函数  
	~Circlist();
	void insertLast(Person * newPerson);//尾插入
	void Play(int N); //开始愉快的游戏
	void Print();
};

Circlist::Circlist() //初始化时，只有一个头结点，有head指向  
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

void Circlist::Play(int N) //开始愉快的游戏
{
	Person* CurrentPerson = Head->next;  //当前玩家指向第一个人
	Person* PreviousPerson = Head;  //用来记录上一个节点（用来删除节点）
	int m = GetRandom(1, N); //第一次报数的值
	while (CurrentPerson->next != CurrentPerson) //当玩家队伍非空的时候循环游戏
	{
		cout << "从" << CurrentPerson->index << "开始 	";
		for (int i = 0; i < m - 1; i++)//当前玩家向后推m-1人
		{
			PreviousPerson = CurrentPerson;
			CurrentPerson = CurrentPerson->next;
			if (CurrentPerson->index == 0)  //如果当前玩家指向了头结点，要向后顺移一个
			{
				PreviousPerson = CurrentPerson;
				CurrentPerson = CurrentPerson->next;
			}
		}
		cout<<"m="<<m<<"	" << CurrentPerson->index << "出队	";
		m = CurrentPerson->code; //下一次的m值
		//删除当前节点
		PreviousPerson->next = CurrentPerson->next;
		delete CurrentPerson;
		if (PreviousPerson->next == Head) PreviousPerson = Head;
		CurrentPerson = PreviousPerson->next;
		Print();
	}
};

void Circlist::Print() //打印玩家列表
{
	Person* cur = Head->next;
	while (cur != Head)
	{
		cout << cur->index << " ";
		cur = cur->next;
	}
	cout << endl;
}