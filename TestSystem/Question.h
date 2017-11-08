#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include<sstream>

//#include<afx.h>
#include"Expression.h"

typedef struct QuestionOption
{
	int MaxNumbersRange = 20;  //���ֵķ�Χ
	int MaxNumbersCount = 2;  //���ֵ�����
	bool IncludeNegativeNumber = false;  //�Ƿ��������
	bool IncludeDecimal = false;  //�Ƿ����С��
	bool IncludeBrackets = false;  //�Ƿ��������
	float ProbabilityOfNegative = 0.2; //���ָ����ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ
	float ProbabilityOfDecimal = 0.3;  //����С���ĸ��ʣ�Сѧ���⣬С����Ҫ̫�ࣩ
	float ProbabilityOfBrackets = 0.8;  //�������ŵĸ��ʣ�Сѧ���⣬���Ų�Ҫ̫�ࣩ
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
	QuestionOption Options; //��������
	QuestionType Type; //����
	Expression expression;  //���ʽ
	float AnswerValue;  //����ֵ
	
	static int GetRandom(int min, int max)//����һ��[min,max]��Χ�������
	{
		return (rand() % (max - min + 1)) + min;
	}  

	static bool GetRandomBool(float probability)  //����������bool��
	{
		return GetRandom(0, 10) <= probability * 10;
	}

	float GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //�������һ������
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

	static string FloatToString(float f)//floatתstring(��ౣ��1λС��)
	{
		char str[200];
		if (f == floor(f)) sprintf(str,"%.0f",f);
		else sprintf(str,"%.1f",f);
		return str;
	}

	string CreateChoice(int &rightchoice) //����ѡ����ѡ��
	{
		rightchoice = GetRandom(1, 3);
		float a[4];
		//����������
		for (int i = 1; i <= 3; i++)
		{
			//������ȷѡ��
			if (i == rightchoice) a[i]=AnswerValue;
			//���ɴ���ѡ��
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
				} while (repetition && failureCount<3); //���ѡ���ظ������Բ�����3��
				//if (failureCount >= 3) 	cout << "ERROR\n";
			}
		}
		//��ת�����ַ���
		string result("=\n");
		for (int i = 1; i <= 3; i++) 
		{
			result.push_back('A' + i - 1);
			result.append("."+FloatToString(a[i]));
			result.append("	");
		}//result.AppendFormat("%c.%s	", 'A' + i - 1, FloatToString(a[i]));
		return result;
	}

	string CreateTrueFalse(bool &judge)  //�����ж���ʹ�
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

	void GenerateExpression()//���ɱ��ʽ
	{
		auto GetOperator = []
		{
			//������������ø����ų��ֵĸ���
			int i = GetRandom(0, 10);
			if (i <= 3) return '+';
			if (i <= 6) return '-';
			if (i <= 9) return '*';
			if (i <= 10) return '/';
		};

		int leftbracket = 0;
		int rightbracket = 0;
		bool needbrackets = Options.IncludeBrackets;  //�Ƿ�Ӧ����������
		if (needbrackets) needbrackets = GetRandomBool(Options.ProbabilityOfBrackets); //�����Ҫ���ŵĻ�����������������
		if (Options.MaxNumbersCount <= 2)  needbrackets = false; //�������������3������Ӧ����������

		if (needbrackets)  //����������ŵ�λ��
		{
			leftbracket = GetRandom(1, Options.MaxNumbersCount - 1);
			if (leftbracket == 1) rightbracket = GetRandom(leftbracket + 1, Options.MaxNumbersCount - 1);
			else rightbracket = GetRandom(leftbracket + 1, Options.MaxNumbersCount);
		}

		for (int i = 1; i <= Options.MaxNumbersCount; i++)
		{
			//���������
			if (needbrackets && i == leftbracket) expression.Append(ExpressionItem('(', true));
			//����һ������
			float number = GetNumber(1, Options.MaxNumbersRange, Options.IncludeNegativeNumber, Options.IncludeDecimal);
			if (number < 0)
				expression.Append(ExpressionItem(number));
			else
				expression.Append(ExpressionItem(number));

			//���������
			if (needbrackets &&i == rightbracket) expression.Append(ExpressionItem(')', true));
			//����һ�������
			if (i != Options.MaxNumbersCount) expression.Append(ExpressionItem(GetOperator(), true));
		}

		AnswerValue = expression.Evaluate(); //����𰸲�����
		//�ڴ˴�����𰸣������жϴ��Ƿ����Ҫ��
	}

	void GenerateQA()  //���ݱ��ʽ������Ŀ�ʹ�
	{
		QuestionContent = expression.ToString();  //�Ƚ���ĿԤ��Ϊ���ʽ�������𰸣�

		switch (Type)
		{
			//�����
			case QuestionType::Completion:
				//������ֵ��1λС����
				AnswerString = FloatToString(AnswerValue);
				//��Ŀ�Ǳ��ʽ
				QuestionContent.push_back('=');
				break;
			//ѡ����
			case QuestionType::Choice:
				int choice;
				QuestionContent.append(CreateChoice(choice));
				AnswerString.push_back('A' + choice - 1);
				break;
			//�ж���
			case QuestionType::TrueFalse:
				bool judge;
				QuestionContent.push_back('=');
				QuestionContent.append(CreateTrueFalse(judge));
				AnswerString.push_back(judge ? 'T' : 'F');
				break;
		}
	}

public:
	string QuestionContent;  //��Ŀ
	string AnswerString;  //���ַ���
	Question() {}
	Question(QuestionOption questionOption, QuestionType questionType = QuestionType::Completion)
	{
		Options = questionOption;
		Type = questionType;
	}
	void PrintQuestion(bool NewLine = true, bool ShowAnswer = false)//������ʽ
	{
		cout << QuestionContent;
		if (ShowAnswer) cout <<"	"<< AnswerString;
		if (NewLine) cout << endl;
	}
	void Generate()//���ɱ��ʽ
	{
		GenerateExpression();
		auto CheckAnswerValidity = [this](float a) //�жϴ��Ƿ�����Ѷ�Ҫ��
		{
			if (fabs(a) > Options.MaxNumbersRange*2) return false;  //ֵ������Χ
			if (!Options.IncludeNegativeNumber && a < 0) return false; //��ֵ����
			if (!Options.IncludeDecimal && floor(a) != a) return false;  //С������
			return true;
		};
		//������ɵı��ʽ�𰸲���Ҫ�󣬱�����������
		while (!CheckAnswerValidity(AnswerValue))
		{
			expression.Clear();
			GenerateExpression();
		}
		expression.RemoveUselessBrackets();
		GenerateQA(); //���ݱ��ʽ������Ŀ�ʹ�
	}
	Expression GetExpression()
	{
		return expression;
	}
};
