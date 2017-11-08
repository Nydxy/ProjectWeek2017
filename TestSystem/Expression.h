#pragma once
#include<vector>
#include<stack>
#include<iomanip>
using namespace std;

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

	static int Precedence(char op1, char op2);//比较两个运算符优先级，返回1：大于，0：等于，-1：小于
	static vector<ExpressionItem> InfixToPostfix(vector<ExpressionItem> infix);//中缀表达式转后缀表达式
	static float EvaluatePostfix(vector<ExpressionItem> postfix);//后缀表达式求值
	template<class T>
	string to_string(const T& t); //将T类型转换成字符串

public:
	int Length();//项数
	void Append(ExpressionItem item);//添加一项
	ExpressionItem operator [](int i) { return items[i]; }
	void Parse(string str);//字符串解析成表达式
	Expression() { }
	Expression(const char * MyExpression) { Parse(MyExpression); }
	float Evaluate();//计算表达式
	string ToString();
	void Clear();
	void RemoveUselessBrackets();  //去除多余的括号(实验性)
};

int Expression::Precedence(char op1, char op2)  //比较两个运算符优先级，返回1：大于，0：等于，-1：小于
{
	if (op1 == '(') return -1;
	if (op1 == '+' || op1 == '-')
		if (op2 == '*' || op2 == '/') return -1;
		else	return 0;

		if (op1 == '*' || op1 == '/')
			if (op2 == '+' || op2 == '-') return 1;
			else return 0;
}

//比较两个运算符优先级，返回1：大于，0：等于，-1：小于

vector<ExpressionItem> Expression::InfixToPostfix(vector<ExpressionItem> infix)  //中缀表达式转后缀表达式
{
	stack<ExpressionItem> optr; //运算符
	vector<ExpressionItem> opnd;  //操作数

	for (auto i : infix)
	{
		if (!i.IsOperator)
		{
			opnd.push_back(i);  //遇到操作数，直接加到opnd的末尾
			continue;
		}
		//遇到左括号：将其入栈
		if (i.Operator == '(')
		{
			optr.push(i);
			continue;
		}
		//遇到右括号：执行出栈操作，并将出栈的元素输出，直到弹出栈的是左括号，左括号不输出
		if (i.Operator == ')')
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
		if (i.IsOperator)
		{
			if (optr.empty())
			{
				optr.push(i); //如果栈为空，直接进栈
				continue;
			}
			//弹出所有优先级大于或者等于该运算符的栈顶元素
			while (!optr.empty() && Precedence(i.Operator, optr.top().Operator) != 1 && optr.top().Operator != '(')
			{
				opnd.push_back(optr.top());
				optr.pop();
			}
			optr.push(i);  //直到栈顶操作符优先级低于该操作符，将该操作符入栈
		}
	}

	while (!optr.empty())
	{
		opnd.push_back(optr.top());
		optr.pop();
	}
	return opnd;
}

float Expression::EvaluatePostfix(vector<ExpressionItem> postfix)  //后缀表达式求值
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

int Expression::Length() { return items.size(); }//项数

void Expression::Append(ExpressionItem item) { items.push_back(item); }

void Expression::Parse(string str)  //字符串解析成表达式
{
	auto isNumber = [](char c) {return (c >= '0'&&c <= '9'); }; //判断一个字符是否是数字
	auto length = str.length();
	auto current = 0;
	while (current<length)
	{
		char c = str[current];
		if (!isNumber(c))  //如果是运算符
		{
			if (c == '-')
			{
				//如果当前是负号（-出现在开头，或者紧跟在左括号后面）
				if (current == 0 || str[current - 1] == '(')
				{
					int q = current + 1;
					while (isNumber(str[q]) || str[q] == '.') q++;
					//q指向下一个运算符
					string thisnumber = str.substr(current, q - current);
					Append(ExpressionItem(atof(thisnumber.c_str())));
					current = q;
					continue;
				}
				//如果当前是减号
				Append(ExpressionItem(c, true));
				current++;
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
			while (isNumber(str[q]) || str[q] == '.') q++;
			//q指向下一个运算符
			string thisnumber = str.substr(current, q - current);
			Append(ExpressionItem(atof(thisnumber.c_str())));
			current = q;
		}
	}
}

float Expression::Evaluate() //计算表达式
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

void Expression::Clear()//清空表达式
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
	ostringstream oss;//创建一个流
	oss << t;//把值传递如流中
	return oss.str();//获取转换后的字符转并将其写入result
}