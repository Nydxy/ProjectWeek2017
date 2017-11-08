#pragma once
#include<vector>
#include<stack>
#include<iomanip>

using namespace std;

typedef struct ExpressionItem//表达式的项
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

//表达式类
class Expression
{
private:
	vector<ExpressionItem> items;  //表达式的项

	static int Precedence(char op1, char op2)  //比较两个运算符优先级，返回1：大于，0：等于，-1：小于
	{
		if (op1 == '(') return -1;
		if (op1 == '+' || op1 == '-')
			if (op2 == '*' || op2 == '/') return -1;
			else	return 0;

			if (op1 == '*' || op1 == '/')
				if (op2 == '+' || op2 == '-') return 1;
				else return 0;
	}
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix)  //中缀表达式转后缀表达式
	{
		stack<ExpressionItem> optr; //运算符
		vector<ExpressionItem> opnd;  //操作数

		for (int i=0;i<infix.size();i++)
		{
			if (!infix[i].IsOperator)
			{
				opnd.push_back(infix[i]);  //遇到操作数，直接加到opnd的末尾
				continue;
			}
			//遇到左括号：将其入栈
			if (infix[i].Operator == '(')
			{
				optr.push(infix[i]);
				continue;
			}
			//遇到右括号：执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出
			if (infix[i].Operator == ')')
			{
				while (optr.top().Operator != '(')
				{
					opnd.push_back(optr.top());  //将栈顶操作符加入到opnd中
					optr.pop();  //将栈顶操作符从optr中弹出
				}
				optr.pop();  //弹出(
				continue;
			}
			//遇到其它操作符
			if (infix[i].IsOperator)
			{
				if (optr.empty())
				{
					optr.push(infix[i]); //如果栈为空，直接进栈
					continue;
				}
				//弹出所有优先级大于或者等于该运算符的栈顶元素
				while (!optr.empty() && Precedence(infix[i].Operator, optr.top().Operator) != 1 && optr.top().Operator != '(')
				{
					opnd.push_back(optr.top());
					optr.pop();
				}
				optr.push(infix[i]);  //直到栈顶操作符优先级低于该操作符，将该操作符入栈
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

	static float EvaluatePostfix(vector<ExpressionItem> postfix)  //后缀表达式求值
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
		ostringstream oss;//创建一个流
		oss << t;//把值传递如流中
		return oss.str();//获取转换后的字符转并将其写入result
	}

public:
	int Length()  //项数
	{
		return items.size();
	}
	void Append(ExpressionItem item)  //添加一项
	{
		items.push_back(item);
	}

	float Evaluate()  //计算表达式
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
	void Clear()//清空表达式
	{
		items.clear();
	}
};
