#pragma once
#include<vector>
#include<stack>
#include<iomanip>
using namespace std;

//���ʽ����
typedef struct ExpressionItem
{
	float Value;
	char Operator;
	bool IsOperator;
	ExpressionItem(){}
	ExpressionItem(float value)
	{
		Value = value;
		IsOperator = false;
	}
	ExpressionItem(char oper, bool isOperator)
	{
		Operator = oper;
		IsOperator = true;
	}
} ExpressionItem;

//���ʽ��
class Expression
{
private:
	vector<ExpressionItem> items;  //���ʽ����

	static int Precedence(char op1, char op2);//�Ƚ�������������ȼ�������1�����ڣ�0�����ڣ�-1��С��
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix);//��׺���ʽת��׺���ʽ
	static float EvaluatePostfix(vector<ExpressionItem> postfix);//��׺���ʽ��ֵ
	template<class T>
	string to_string(const T& t); //��T����ת�����ַ���

public:
	int Length();//����
	void Append(ExpressionItem item);//���һ��
	ExpressionItem operator [](int i) { return items[i]; }
	void Parse(string str);//�ַ��������ɱ��ʽ
	Expression() { }
	Expression(const char * MyExpression) { Parse(MyExpression); }
	float Evaluate();//������ʽ
	string ToString();
	void Clear();
	void RemoveUselessBrackets();  //ȥ�����������(ʵ����)
};

int Expression::Precedence(char op1, char op2)  //�Ƚ�������������ȼ�������1�����ڣ�0�����ڣ�-1��С��
{
	if (op1 == '(') return -1;
	if (op1 == '+' || op1 == '-')
		if (op2 == '*' || op2 == '/') return -1;
		else	return 0;

		if (op1 == '*' || op1 == '/')
			if (op2 == '+' || op2 == '-') return 1;
			else return 0;
}

//�Ƚ�������������ȼ�������1�����ڣ�0�����ڣ�-1��С��

vector<ExpressionItem> Expression::InfixToPostfix(vector<ExpressionItem> infix)  //��׺���ʽת��׺���ʽ
{
	stack<ExpressionItem> optr; //�����
	vector<ExpressionItem> opnd;  //������

	for (auto i : infix)
	{
		if (!i.IsOperator)
		{
			opnd.push_back(i);  //������������ֱ�Ӽӵ�opnd��ĩβ
			continue;
		}
		//���������ţ�������ջ
		if (i.Operator == '(')
		{
			optr.push(i);
			continue;
		}
		//���������ţ�ִ�г�ջ������������ջ��Ԫ�������ֱ������ջ���������ţ������Ų����
		if (i.Operator == ')')
		{
			while (optr.top().Operator != '(')
			{
				opnd.push_back(optr.top());  //��ջ�����������뵽opnd��
				optr.pop();  //��ջ����������optr�е���
			}
			optr.pop();  //����(
			continue;
		}
		//��������������
		if (i.IsOperator)
		{
			if (optr.empty())
			{
				optr.push(i); //���ջΪ�գ�ֱ�ӽ�ջ
				continue;
			}
			//�����������ȼ����ڻ��ߵ��ڸ��������ջ��Ԫ��
			while (!optr.empty() && Precedence(i.Operator, optr.top().Operator) != 1 && optr.top().Operator != '(')
			{
				opnd.push_back(optr.top());
				optr.pop();
			}
			optr.push(i);  //ֱ��ջ�����������ȼ����ڸò����������ò�������ջ
		}
	}

	while (!optr.empty())
	{
		opnd.push_back(optr.top());
		optr.pop();
	}
	return opnd;
}

float Expression::EvaluatePostfix(vector<ExpressionItem> postfix)  //��׺���ʽ��ֵ
{
	auto Operate = [](float a, char optr, float b)
	{
		switch (optr)
		{
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a*b;
		case '/': return a / b;
		}
	};
	stack<float> result;
	int length = postfix.size();
	for (int i = 0; i < length; i++)
	{
		if (!postfix[i].IsOperator) result.push(postfix[i].Value);
		else
		{
			float a = result.top();
			result.pop();
			float b = result.top();
			result.pop();
			result.push(Operate(b, postfix[i].Operator, a));
		}
	}
	return result.top();
}

int Expression::Length() { return items.size(); }//����

void Expression::Append(ExpressionItem item) { items.push_back(item); }

void Expression::Parse(string str)  //�ַ��������ɱ��ʽ
{
	auto isNumber = [](char c) {return (c >= '0'&&c <= '9'); }; //�ж�һ���ַ��Ƿ�������
	auto length = str.length();
	auto current = 0;
	while (current<length)
	{
		char c = str[current];
		if (!isNumber(c))  //����������
		{
			if (c == '-')
			{
				//�����ǰ�Ǹ��ţ�-�����ڿ�ͷ�����߽����������ź��棩
				if (current == 0 || str[current - 1] == '(')
				{
					int q = current + 1;
					while (isNumber(str[q]) || str[q] == '.') q++;
					//qָ����һ�������
					string thisnumber = str.substr(current, q - current);
					Append(ExpressionItem(atof(thisnumber.c_str())));
					current = q;
					continue;
				}
				//�����ǰ�Ǽ���
				Append(ExpressionItem(c, true));
				current++;
			}
			else  //����������
			{
				Append(ExpressionItem(c, true));
				current++;
				continue;
			}
		}
		//���������
		else
		{
			int q = current + 1;
			while (isNumber(str[q]) || str[q] == '.') q++;
			//qָ����һ�������
			string thisnumber = str.substr(current, q - current);
			Append(ExpressionItem(atof(thisnumber.c_str())));
			current = q;
		}
	}
}

float Expression::Evaluate() //������ʽ
{
	return EvaluatePostfix(InfixToPostfix(items));
}

string Expression::ToString()
{
	string str;
	for (auto i : items)
	{
		if (i.IsOperator)
			str += i.Operator;
		else
		{
			if (i.Value<0) str += "(" + to_string(i.Value) + ")";
			else str += to_string(i.Value);
		}
	}
	return str;
}

void Expression::Clear()//��ձ��ʽ
{
	items.clear();
}

void Expression::RemoveUselessBrackets()
{
	int len = Length();
	for (int i = 0; i < len - 4; i++)
	{
		if (!items[i].IsOperator) continue;
		if (items[i].Operator != '(') continue;
		if ((i - 1 < 0 || items[i - 1].Operator == '+') && (i + 5 >= len || items[i + 5].Operator == '+' || items[i + 5].Operator == '-'))
		{
			vector<ExpressionItem>::iterator iter = items.begin() + i;
			iter = items.erase(iter);
			items.erase(iter + 3);
			break;
		}
	}
}

template<class T>
string Expression::to_string(const T & t)
{
	ostringstream oss;//����һ����
	oss << t;//��ֵ����������
	return oss.str();//��ȡת������ַ�ת������д��result
}