#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include"Expression.h"

typedef struct ProblemSet
{
	int numrange = 20;  //数字的范围
	int numcount = 2;  //数字的数量
	bool hasFushu = false;  //是否包含负数
	bool hasXiaoshu = false;  //是否包含小数
	bool hasKuohao = false;  //是否包含括号
} ProblemSet;

enum  ProblemType { Completion, Choice, TrueFalse }; //题目类型

//问题的类
class Problem
{
public:
	string question;  //题目
	string answer;  //答案字符串
	Problem(ProblemSet a, ProblemType b = Completion)
	{
		Options = a;
		Type = b;
	}
	void create()//生成题目
	{
		bool kuohao = Options.hasKuohao;  //是否应该生成括号
		if (kuohao)
		{
			kuohao = getBool(0.8); //如果需要括号的话，按概率生成括号
		}
		if (Options.numcount <= 2)
		{
			kuohao = false; //如果运算数少于3个，不应该生成括号
		}
		int kuohaol = 0;
		int kuohaor = 0;
		if (kuohao)  //随机生成括号的位置
		{
			kuohaol = getRand(1, Options.numcount - 1);
			if (kuohaol == 1) kuohaor = getRand(kuohaol + 1, Options.numcount - 1);
			else kuohaor = getRand(kuohaol + 1, Options.numcount);
		}

		for (int i = 1; i <= Options.numcount; i++)
		{
			//添加左括号
			if (kuohao && i == kuohaol)
			{
				expression.Append(Item('(', true));
			}
			//生成一个数字
			float number = getNum(1, Options.numrange, Options.hasFushu, Options.hasXiaoshu);
			if (number < 0)
			{
				expression.Append(Item(number));
			}
			else
			{
				expression.Append(Item(number));
			}

			//添加右括号
			if (kuohao &&i == kuohaor)
			{
				expression.Append(Item(')', true));
			}
			//生成一个运算符
			if (i != Options.numcount)
			{
				expression.Append(Item(getOperator(), true));
			}
		}

		AnswerValue = expression.Evaluate(); //求出答案并保存
		question = expression.ToString();  //先将题目预置为表达式（不含答案）

		//根据题型生成题目
		switch (Type)
		{
			//填空题
		case Completion:
			//答案是数值（1位小数）
			answer = FloatToString(AnswerValue);
			//题目是表达式
			question.push_back('=');
			break;
			//选择题
		case Choice:
			int choice;
			question.append(getXuanze(choice));
			answer.push_back('A' + choice - 1);
			break;
			//判断题
		case TrueFalse:
			bool judge;
			question.push_back('=');
			question.append(getPanduan(judge));
			answer.push_back(judge ? 'T' : 'F');
			break;
		}
	}
private:
	ProblemSet Options; //出题设置
	ProblemType Type; //题型
	Expression expression;  //表达式
	float AnswerValue;  //答案数值
	
	int getRand(int min, int max)//生成一个[min,max]范围的随机数
	{
		return (rand() % (max - min + 1)) + min;
	}  

	bool getBool(float probability)  //按概率生成bool型
	{
		return getRand(0, 10) <= probability * 10;
	}

	float getNum(int min, int max, bool allowNegative, bool allowDecimal)  //随机生成一个数字
	{
		//不用注释了，大家都看得懂
		float result = (float)getRand(min, max);
		if (allowNegative&&getBool(0.2))
		{
			int rnd = getRand(0, 1);
			if (rnd) result = -result;
		}
		if (allowDecimal&&getBool(0.3))
		{
			float rnd = (float)getRand(0, 9);
			rnd = rnd / 10.0;
			result = result + rnd;
		}
		return result;
	}

	char getOperator()
	{
		//在这里可以设置各符号出现的概率
		int i = getRand(0, 10);
		if (i <= 3) return '+';
		if (i <= 6) return '-';
		if (i <= 9) return '*';
		if (i <= 10) return '/';
	}

	string FloatToString(float f)//float转string(最多保留1位小数)
	{
		char str[200];
		if (f == floor(f)) sprintf_s(str,"%.0f",f);
		else sprintf_s(str,"%.1f",f);
		return str;
	}

	string getXuanze(int &rightchoice) //生成选择题选项
	{
		rightchoice = getRand(1, 3);
		float a[4];
		//先生成数字
		for (int i = 1; i <= 3; i++)
		{
			//生成正确选项
			if (i == rightchoice) a[i] = AnswerValue;
			//生成错误选项
			else
			{
				int rnd = getRand(AnswerValue * 30, AnswerValue * 300);  //change the range of choice items
				if (floor(AnswerValue) == AnswerValue)
				{
					a[i] = rnd / 100; //if answer is integer, item should be integer too
				}
				else
				{
					a[i] = rnd / 100.0;
				}
				if (fabs(AnswerValue) < 10)//if the answer is too small, the chance of repetition will be high
				{
					a[i] += getRand(-10, 10);
				}
			}
		}
		//再转换成字符串
		string result("=\n");
		for (int i = 1; i <= 3; i++) 
		{
			result.push_back('A' + i - 1);
			result+="."+FloatToString(a[i])+"	";
		}
		return result;
	}

	string getPanduan(bool &judge)  //生成判断题和答案
	{
		judge = getBool(0.5);
		if (judge)
		{
			return FloatToString(AnswerValue);
		}
		else
		{
			string result;
			int rnd = getRand(AnswerValue * 30, AnswerValue * 300);
			if (floor(AnswerValue) == AnswerValue)//If answer is an integer
			{
				if (AnswerValue==0) result= FloatToString(getRand(-10, 10));  //to prevent answer=0
				else result = FloatToString(rnd / 100);  
			}
			else//If answer is a float
			{
				result = FloatToString(rnd / 100.0);
			}
			return result;
		}
	}
};