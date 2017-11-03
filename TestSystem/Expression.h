#pragma once
#include<vector>
#include<stack>
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
	static bool isNumber(char c)
	{
		return (c >= '0'&&c <= '9');
	}
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
public:
	int Length()  //����
	{
		return items.size();
	}
	void Append(ExpressionItem item)  //���һ��
	{
		items.push_back(item);
	}
	ExpressionItem operator [](int i)
	{
		return items[i];
	}
	void Parse(CString str)  //�ַ��������ɱ��ʽ
	{
		int length = str.GetLength();
		int current = 0;
		while (current<length)
		{
			char c = str[current];
			if (!isNumber(c))  //����������
			{
				if (c == '-')
				{
					//�����ǰ�Ǽ���
					if (current == 0 || isNumber(str[current - 1]))//ǰһ���ַ������֣���ʾ��ǰ�Ǽ���
					{
						Append(ExpressionItem(c, true));
						current++;
						continue;
					}
					//�����ǰ�Ǹ���
					int q = current + 1;
					while (isNumber(str[q]) || str[q] == '.') q++;
					//qָ����һ�������
					CString thisnumber = str.Mid(current, q - current);  //��ȡ���������
					Append(ExpressionItem(atof(thisnumber)));
					current = q;
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
				while (isNumber(str[q])||str[q]=='.') q++;
				//qָ����һ�������
				CString thisnumber = str.Mid(current, q - current);  //��ȡ���������
				Append(ExpressionItem(atof(thisnumber)));
				current = q;
			}
		}
	}
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix)  //��׺���ʽת��׺���ʽ
	{
		stack<ExpressionItem> optr; //�����
		vector<ExpressionItem> opnd; //�������ͽ��
		int ExpressionLength = infix.size();
		for (int i = 0; i < ExpressionLength; i++)
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
				while (!optr.empty() && Precedence(infix[i].Operator, optr.top().Operator) != 1 && optr.top().Operator !='(')
				{
					opnd.push_back(optr.top());
					optr.pop();
				}
				optr.push(infix[i]);  //ֱ��ջ�����������ȼ����ڸò����������ò�������ջ
			}
		}
		//���ս�ջ�е�Ԫ�����γ�ջ�����
		while (!optr.empty())
		{
			opnd.push_back(optr.top());
			optr.pop();
		}
		return opnd;
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
	Expression() { }
	Expression(const char * MyExpression)
	{
		Parse(MyExpression);
	}
	float Evaluate()  //������ʽ
	{
		return EvaluatePostfix(InfixToPostfix(items));
	}
	CString ToString()
	{
		CString str;
		for (int i = 0; i < Length(); i++)
		{
			if (items[i].IsOperator) str.AppendChar(items[i].Operator);
			else 
			{
				if (items[i].Value<0) str.AppendFormat("(%g)", items[i].Value);
				else str.AppendFormat("%g", items[i].Value);
			}
		}
		return str;
	}
};