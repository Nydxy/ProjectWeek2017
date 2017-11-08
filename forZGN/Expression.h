#pragma once
#include<vector>
#include<stack>
#include<iomanip>

using namespace std;

typedef struct ExpressionItem//���ʽ����
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

	static int Precedence(char op1, char op2)  //�Ƚ�������������ȼ�������1�����ڣ�0�����ڣ�-1��С��
	{
		if (op1 == '(') return -1;
		if (op1 == '+' || op1 == '-')
			if (op2 == '*' || op2 == '/') return -1;
			else	return 0;

			if (op1 == '*' || op1 == '/')
				if (op2 == '+' || op2 == '-') return 1;
				else return 0;
	}
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix)  //��׺���ʽת��׺���ʽ
	{
		stack<ExpressionItem> optr; //�����
		vector<ExpressionItem> opnd;  //������

		for (int i=0;i<infix.size();i++)
		{
			if (!infix[i].IsOperator)
			{
				opnd.push_back(infix[i]);  //������������ֱ�Ӽӵ�opnd��ĩβ
				continue;
			}
			//���������ţ�������ջ
			if (infix[i].Operator == '(')
			{
				optr.push(infix[i]);
				continue;
			}
			//���������ţ�ִ�г�ջ������������ջ��Ԫ�������ֱ������ջ���������ţ������Ų����
			if (infix[i].Operator == ')')
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
			if (infix[i].IsOperator)
			{
				if (optr.empty())
				{
					optr.push(infix[i]); //���ջΪ�գ�ֱ�ӽ�ջ
					continue;
				}
				//�����������ȼ����ڻ��ߵ��ڸ��������ջ��Ԫ��
				while (!optr.empty() && Precedence(infix[i].Operator, optr.top().Operator) != 1 && optr.top().Operator != '(')
				{
					opnd.push_back(optr.top());
					optr.pop();
				}
				optr.push(infix[i]);  //ֱ��ջ�����������ȼ����ڸò����������ò�������ջ
			}
		}

		while (!optr.empty())
		{
			opnd.push_back(optr.top());
			optr.pop();
		}
		return opnd;
	}

	static float Operate(float a, char optr, float b)
	{
		switch (optr)
		{
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a*b;
		case '/': return a / b;
		}
	}

	static float EvaluatePostfix(vector<ExpressionItem> postfix)  //��׺���ʽ��ֵ
	{
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
	int Length()  //����
	{
		return items.size();
	}
	void Append(ExpressionItem item)  //���һ��
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
