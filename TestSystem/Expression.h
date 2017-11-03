#pragma once
#include<vector>
#include<stack>
//表达式的项
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

//表达式类
class Expression
{
private:
	vector<ExpressionItem> items;  //表达式的项
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
public:
	int Length()  //项数
	{
		return items.size();
	}
	void Append(ExpressionItem item)  //添加一项
	{
		items.push_back(item);
	}
	ExpressionItem operator [](int i)
	{
		return items[i];
	}
	void Parse(CString str)  //字符串解析成表达式
	{
		int length = str.GetLength();
		int current = 0;
		while (current<length)
		{
			char c = str[current];
			if (!isNumber(c))  //如果是运算符
			{
				if (c == '-')
				{
					//如果当前是减号
					if (current == 0 || isNumber(str[current - 1]))//前一个字符是数字，表示当前是减号
					{
						Append(ExpressionItem(c, true));
						current++;
						continue;
					}
					//如果当前是负号
					int q = current + 1;
					while (isNumber(str[q]) || str[q] == '.') q++;
					//q指向下一个运算符
					CString thisnumber = str.Mid(current, q - current);  //提取出这个数字
					Append(ExpressionItem(atof(thisnumber)));
					current = q;
				}
				else  //是其他符号
				{
					Append(ExpressionItem(c, true));
					current++;
					continue;
				}
			}
			//如果是数字
			else
			{
				int q = current + 1;
				while (isNumber(str[q])||str[q]=='.') q++;
				//q指向下一个运算符
				CString thisnumber = str.Mid(current, q - current);  //提取出这个数字
				Append(ExpressionItem(atof(thisnumber)));
				current = q;
			}
		}
	}
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix)  //中缀表达式转后缀表达式
	{
		stack<ExpressionItem> optr; //运算符
		vector<ExpressionItem> opnd; //操作数和结果
		int ExpressionLength = infix.size();
		for (int i = 0; i < ExpressionLength; i++)
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
				while (!optr.empty() && Precedence(infix[i].Operator, optr.top().Operator) != 1 && optr.top().Operator !='(')
				{
					opnd.push_back(optr.top());
					optr.pop();
				}
				optr.push(infix[i]);  //直到栈顶操作符优先级低于该操作符，将该操作符入栈
			}
		}
		//最终将栈中的元素依次出栈，输出
		while (!optr.empty())
		{
			opnd.push_back(optr.top());
			optr.pop();
		}
		return opnd;
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
	Expression() { }
	Expression(const char * MyExpression)
	{
		Parse(MyExpression);
	}
	float Evaluate()  //计算表达式
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