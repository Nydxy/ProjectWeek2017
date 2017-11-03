#include<iostream>
#include<stdlib.h>
#include<time.h>
using namespace std;
const int N = 15;  //�������

//ѭ�������еĽ��
class Node
{
public:
	int index;//���
	int code;//����
	Node *next; //������������һ��ĵ�ַ
	Node(int i, int m) {
		index = i;
		code = m;
	}
};

//ѭ��������
class Circlist
{
protected:
	int len;//�����н�����  
	Node* Head; //ָ��ͷ���  

public:

	//���캯��
	Circlist()  {
		Head = new Node(0, 0);
		Head->next = Head;
		len = 0;
		srand(time(0));
	}

	//��������
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

	//�����������ӽ��
	void append(Node * p) {
		Node* current = Head;
		while (current->next != Head) {
			current = current->next;
		}
		p->next = current->next;
		current->next = p;
		len++;
	}
	
	//��ʼ
	void Begin(); 
};

void Circlist::Begin() {
	Node* p;  //��ǰ��㣨��һ���ˣ�
	Node* last;  //��һ���ڵ�
	int m = (rand() % len) + 1; //���ȡһ��m��ʼ
	p = Head->next;
	last = Head;
	//ʣ����˲���Ϊ0�������ܿ�
	while (p->next != p) {
		for (int i = 0; i < m - 1; i++) {
			last = p;
			p = p->next;
			if (p->index == 0) {
				last = p;
				p = p->next;
			}
		}
		cout << p->index << ", "; //������е��˵ı��
		m = p->code; //������һ�ε�mֵ
		//ɾ����ǰ�ڵ�
		last->next = p->next;
		delete p;
		if (last->next == Head) {
			last = Head;
		}
		p = last->next;
	}
};

void main() {
	printf("һ����%d���ˣ����1-%d\n", N, N);
	Circlist list;
	int i = 1;
	//�ȳ�ʼ�����ѭ������
	while (i <= N) {
		int rnd = 2 * ((rand() % N) + 1);
		Node* a = new Node(i, rnd);
		list.append(a);
		i = i + 1;
	}
	printf("���е�˳���ǣ�");
	list.Begin();  //��ʼ
	printf("\n");
	system("pause");
	return;
}