#pragma once
#include<vector>
#include<stack>
#include<sstream>

using namespace std;

typedef struct Item//���ʽ����
{
	float Value;
	char Operator;
	bool IsOperator;
	Item(){}
	Item(float value)
	{
		Value = value;
		IsOperator = false;
	}
	Item(char oper, bool isOperator)
	{
		Operator = oper;
		IsOperator = true;
	}
} Item;

//���ʽ��
class Expression
{
private:
	vector<Item> items;  //���ʽ����

	int Precedence(char op1, char op2)  //�Ƚ�������������ȼ�������1�����ڣ�0�����ڣ�-1��С��
	{
		if (op1 == '(') return -1;
		if (op1 == '+' || op1 == '-')
			if (op2 == '*' || op2 == '/') return -1;
			else	return 0;

			if (op1 == '*' || op1 == '/')
				if (op2 == '+' || op2 == '-') return 1;
				else return 0;
	}

	vector<Item> InfixToPostfix(vector<Item> infix)  //��׺���ʽת��׺���ʽ
	{
		//����ǳ���
		stack<Item> operators; //�����
		vector<Item> numbers;  //������

		for (int i=0;i<infix.size();i++)
		{
			if (!infix[i].IsOperator)
			{
				numbers.push_back(infix[i]);  //������������ֱ�Ӽӵ�numbers��ĩβ
				continue;
			}
			//���������ţ�������ջ
			if (infix[i].Operator == '(')
			{
				operators.push(infix[i]);
				continue;
			}
			//���������ţ�ִ�г�ջ������������ջ��Ԫ�������ֱ������ջ���������ţ������Ų����
			if (infix[i].Operator == ')')
			{
				while (operators.top().Operator != '(')
				{
					numbers.push_back(operators.top());  //��ջ�����������뵽numbers��
					operators.pop();  //��ջ����������operators�е���
				}
				operators.pop();  //����(
				continue;
			}
			//��������������
			if (infix[i].IsOperator)
			{
				if (operators.empty())
				{
					operators.push(infix[i]); //���ջΪ�գ�ֱ�ӽ�ջ
					continue;
				}
				//�����������ȼ����ڻ��ߵ��ڸ��������ջ��Ԫ��
				while (!operators.empty() && Precedence(infix[i].Operator, operators.top().Operator) != 1 && operators.top().Operator != '(')
				{
					numbers.push_back(operators.top());
					operators.pop();
				}
				operators.push(infix[i]);  //ֱ��ջ�����������ȼ����ڸò����������ò�������ջ
			}
		}

		while (!operators.empty())
		{
			numbers.push_back(operators.top());
			operators.pop();
		}
		return numbers;
	}

	float Operate(float a, char operators, float b)
	{
		//Ҳûʲô��˵�ģ�����
		switch (operators)
		{
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a*b;
		case '/': return a / b;
		}
	}

	float EvaluatePostfix(vector<Item> postfix)  //��׺���ʽ��ֵ
	{
		//ûʲô��˵�ģ�easy
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

	string to_string(const float& t)
	{
		ostringstream oss;//����һ����
		oss << t;//��ֵ����������
		return oss.str();//��ȡת������ַ�ת������д��result
	}

public:
	void Append(Item item)  //���һ��
	{
		items.push_back(item);
	}

	float Evaluate()  //������ʽ
	{
		return EvaluatePostfix(InfixToPostfix(items));
	}
	string ToString()
	{
		string str;
		for (int i=0;i<items.size();i++)
		{
			if (items[i].IsOperator) 
				str+=items[i].Operator;
			else
			{
				if (items[i].Value<0) str += "(" + to_string(items[i].Value) + ")";
				else str +=  to_string(items[i].Value);
			}
		}
		return str;
	}
	void Clear()//��ձ��ʽ
	{
		items.clear();
	}
};
