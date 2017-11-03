#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
const int N = 15;  //玩家数量

//循环链表中的结点
class Node
{
public:
	int index;//序号
	int code;//密码
	Node *next; //单链表链表下一项的地址
	Node(int i, int m) {
		index = i;
		code = m;
	}
};

//循环链表类
class Circlist
{
protected:
	int len;//链表中结点个数  
	Node* Head; //指向头结点  

public:

	//构造函数
	Circlist()  {
		Head = new Node(0, 0);
		Head->next = Head;
		len = 0;
		srand(time(0));
	}

	//析构函数
	~Circlist() {
		Node* current;
		Node* tmp;
		if (Head != NULL) {
			tmp = Head;
			current = Head->next;
			while (current != Head) {
				delete tmp;
				tmp = current;
				current = current->next;
			}
			delete tmp;
			len = 0;
		}
	}

	//在链表最后添加结点
	void append(Node * p) {
		Node* current = Head;
		while (current->next != Head) {
			current = current->next;
		}
		p->next = current->next;
		current->next = p;
		len++;
	}
	
	//开始
	void Begin(); 
};

void Circlist::Begin() {
	Node* p;  //当前结点（第一个人）
	Node* last;  //上一个节点
	int m = (rand() % len) + 1; //随便取一个m开始
	p = Head->next;
	last = Head;
	//剩余的人不能为0，链表不能空
	while (p->next != p) {
		for (int i = 0; i < m - 1; i++) {
			last = p;
			p = p->next;
			if (p->index == 0) {
				last = p;
				p = p->next;
			}
		}
		cout << p->index << ", "; //输出出列的人的编号
		m = p->code; //产生下一次的m值
		//删除当前节点
		last->next = p->next;
		delete p;
		if (last->next == Head) {
			last = Head;
		}
		p = last->next;
	}
};

void main() {
	printf("一共有%d个人，编号1-%d\n", N, N);
	Circlist list;
	int i = 1;
	//先初始化这个循环链表
	while (i <= N) {
		int rnd = 2 * ((rand() % N) + 1);
		Node* a = new Node(i, rnd);
		list.append(a);
		i = i + 1;
	}
	printf("出列的顺序是：");
	list.Begin();  //开始
	printf("\n");
	system("pause");
	return;
}