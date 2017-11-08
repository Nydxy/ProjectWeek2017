#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include<sstream>

//#include<afx.h>
#include"Expression.h"

typedef struct QuestionOption
{
	int MaxNumbersRange = 20;  //数字的范围
	int MaxNumbersCount = 2;  //数字的数量
	bool IncludeNegativeNumber = false;  //是否包含负数
	bool IncludeDecimal = false;  //是否包含小数
	bool IncludeBrackets = false;  //是否包含括号
	float ProbabilityOfNegative = 0.2; //出现负数的概率（小学生题，负数不要太多）
	float ProbabilityOfDecimal = 0.3;  //出现小数的概率（小学生题，小数不要太多）
	float ProbabilityOfBrackets = 0.8;  //出现括号的概率（小学生题，括号不要太多）
	QuestionOption(){}
	QuestionOption(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets)
	{
		MaxNumbersRange = maxNumbersRange;
		MaxNumbersCount = maxNumbersCount;
		IncludeNegativeNumber = includeNegativeNumber;
		IncludeDecimal = includeDecimal;
		IncludeBrackets = includeBrackets;
	}
} QuestionOption;

enum class QuestionType { Completion, Choice, TrueFalse };

//class of a question for user to answer
class Question
{
private:
	QuestionOption Options; //出题设置
	QuestionType Type; //题型
	Expression expression;  //表达式
	float AnswerValue;  //答案数值
	
	static int GetRandom(int min, int max)//生成一个[min,max]范围的随机数
	{
		return (rand() % (max - min + 1)) + min;
	}  

	static bool GetRandomBool(float probability)  //按概率生成bool型
	{
		return GetRandom(0, 10) <= probability * 10;
	}

	float GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //随机生成一个数字
	{
		float result = (float)GetRandom(min, max);
		if (allowNegative&&GetRandomBool(Options.ProbabilityOfNegative))
		{
			int rnd = GetRandom(0, 1);
			if (rnd) result = -result;
		}
		if (allowDecimal&&GetRandomBool(Options.ProbabilityOfDecimal))
		{
			float rnd = (float)GetRandom(0, 9);
			rnd = rnd / 10.0;
			result = result + rnd;
		}
		return result;
	}

	static string FloatToString(float f)//float转string(最多保留1位小数)
	{
		char str[200];
		if (f == floor(f)) sprintf(str,"%.0f",f);
		else sprintf(str,"%.1f",f);
		return str;
	}

	string CreateChoice(int &rightchoice) //生成选择题选项
	{
		rightchoice = GetRandom(1, 3);
		float a[4];
		//先生成数字
		for (int i = 1; i <= 3; i++)
		{
			//生成正确选项
			if (i == rightchoice) a[i]=AnswerValue;
			//生成错误选项
			else
			{
				bool repetition = false; //flag of repetition check
				int failureCount = 0;
				do
				{
					int rnd = GetRandom(AnswerValue * 50, AnswerValue * 200);  //change the range of choice items
					if (floor(AnswerValue) == AnswerValue) a[i] = rnd / 100; //if answer is integer, item should be integer too
					else a[i] = rnd / 100.0;
					if (fabs(AnswerValue) < 10)//if the answer is too small, the chance of repetition will be high
					{
						a[i] += GetRandom(-10, 10); 
					}
					for (int j = 1; j < i; j++) //check for repetition
					{
						if (FloatToString(a[j]) == FloatToString(a[i]))
						{
							repetition = true;
							failureCount++;
							break;
						}
					}
				} while (repetition && failureCount<3); //如果选项重复，重试不超过3次
				//if (failureCount >= 3) 	cout << "ERROR\n";
			}
		}
		//再转换成字符串
		string result("=\n");
		for (int i = 1; i <= 3; i++) 
		{
			result.push_back('A' + i - 1);
			result.append("."+FloatToString(a[i]));
			result.append("	");
		}//result.AppendFormat("%c.%s	", 'A' + i - 1, FloatToString(a[i]));
		return result;
	}

	string CreateTrueFalse(bool &judge)  //生成判断题和答案
	{
		judge = GetRandomBool(0.5);
		if (judge)
		{
			return FloatToString(AnswerValue);
		}
		else
		{
			string result;
			int rnd = GetRandom(AnswerValue * 50, AnswerValue * 200);
			if (floor(AnswerValue) == AnswerValue)//If answer is an integer
			{
				if (AnswerValue==0) result= FloatToString(GetRandom(-10, 10));  //to prevent answer=0
				else result = FloatToString(rnd / 100);  
			}
			else//If answer is a float
			{
				result = FloatToString(rnd / 100.0);
			}
			return result;
		}
	}

	void GenerateExpression()//生成表达式
	{
		auto GetOperator = []
		{
			//在这里可以设置各符号出现的概率
			int i = GetRandom(0, 10);
			if (i <= 3) return '+';
			if (i <= 6) return '-';
			if (i <= 9) return '*';
			if (i <= 10) return '/';
		};

		int leftbracket = 0;
		int rightbracket = 0;
		bool needbrackets = Options.IncludeBrackets;  //是否应该生成括号
		if (needbrackets) needbrackets = GetRandomBool(Options.ProbabilityOfBrackets); //如果需要括号的话，按概率生成括号
		if (Options.MaxNumbersCount <= 2)  needbrackets = false; //如果运算数少于3个，不应该生成括号

		if (needbrackets)  //随机生成括号的位置
		{
			leftbracket = GetRandom(1, Options.MaxNumbersCount - 1);
			if (leftbracket == 1) rightbracket = GetRandom(leftbracket + 1, Options.MaxNumbersCount - 1);
			else rightbracket = GetRandom(leftbracket + 1, Options.MaxNumbersCount);
		}

		for (int i = 1; i <= Options.MaxNumbersCount; i++)
		{
			//添加左括号
			if (needbrackets && i == leftbracket) expression.Append(ExpressionItem('(', true));
			//生成一个数字
			float number = GetNumber(1, Options.MaxNumbersRange, Options.IncludeNegativeNumber, Options.IncludeDecimal);
			if (number < 0)
				expression.Append(ExpressionItem(number));
			else
				expression.Append(ExpressionItem(number));

			//添加右括号
			if (needbrackets &&i == rightbracket) expression.Append(ExpressionItem(')', true));
			//生成一个运算符
			if (i != Options.MaxNumbersCount) expression.Append(ExpressionItem(GetOperator(), true));
		}

		AnswerValue = expression.Evaluate(); //求出答案并保存
		//在此处求出答案，方便判断答案是否符合要求
	}

	void GenerateQA()  //根据表达式生成题目和答案
	{
		QuestionContent = expression.ToString();  //先将题目预置为表达式（不含答案）

		switch (Type)
		{
			//填空题
			case QuestionType::Completion:
				//答案是数值（1位小数）
				AnswerString = FloatToString(AnswerValue);
				//题目是表达式
				QuestionContent.push_back('=');
				break;
			//选择题
			case QuestionType::Choice:
				int choice;
				QuestionContent.append(CreateChoice(choice));
				AnswerString.push_back('A' + choice - 1);
				break;
			//判断题
			case QuestionType::TrueFalse:
				bool judge;
				QuestionContent.push_back('=');
				QuestionContent.append(CreateTrueFalse(judge));
				AnswerString.push_back(judge ? 'T' : 'F');
				break;
		}
	}

public:
	string QuestionContent;  //题目
	string AnswerString;  //答案字符串
	Question() {}
	Question(QuestionOption questionOption, QuestionType questionType = QuestionType::Completion)
	{
		Options = questionOption;
		Type = questionType;
	}
	void PrintQuestion(bool NewLine = true, bool ShowAnswer = false)//输出表达式
	{
		cout << QuestionContent;
		if (ShowAnswer) cout <<"	"<< AnswerString;
		if (NewLine) cout << endl;
	}
	void Generate()//生成表达式
	{
		GenerateExpression();
		auto CheckAnswerValidity = [this](float a) //判断答案是否符合难度要求
		{
			if (fabs(a) > Options.MaxNumbersRange*2) return false;  //值超出范围
			if (!Options.IncludeNegativeNumber && a < 0) return false; //负值错误
			if (!Options.IncludeDecimal && floor(a) != a) return false;  //小数错误
			return true;
		};
		//如果生成的表达式答案不合要求，必须重新生成
		while (!CheckAnswerValidity(AnswerValue))
		{
			expression.Clear();
			GenerateExpression();
		}
		expression.RemoveUselessBrackets();
		GenerateQA(); //根据表达式生成题目和答案
	}
	Expression GetExpression()
	{
		return expression;
	}
};
