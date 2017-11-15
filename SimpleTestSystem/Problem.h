#pragma once
#include<iostream>
#include<cmath>
#include<string>
#include"Expression.h"

typedef struct ProblemSet
{
	int numrange = 20;  //���ֵķ�Χ
	int numcount = 2;  //���ֵ�����
	bool hasFushu = false;  //�Ƿ��������
	bool hasXiaoshu = false;  //�Ƿ����С��
	bool hasKuohao = false;  //�Ƿ��������
} ProblemSet;

enum  ProblemType { Completion, Choice, TrueFalse }; //��Ŀ����

//�������
class Problem
{
public:
	string question;  //��Ŀ
	string answer;  //���ַ���
	Problem(ProblemSet a, ProblemType b = Completion)
	{
		Options = a;
		Type = b;
	}
	void create()//������Ŀ
	{
		bool kuohao = Options.hasKuohao;  //�Ƿ�Ӧ����������
		if (kuohao)
		{
			kuohao = getBool(0.8); //�����Ҫ���ŵĻ�����������������
		}
		if (Options.numcount <= 2)
		{
			kuohao = false; //�������������3������Ӧ����������
		}
		int kuohaol = 0;
		int kuohaor = 0;
		if (kuohao)  //����������ŵ�λ��
		{
			kuohaol = getRand(1, Options.numcount - 1);
			if (kuohaol == 1) kuohaor = getRand(kuohaol + 1, Options.numcount - 1);
			else kuohaor = getRand(kuohaol + 1, Options.numcount);
		}

		for (int i = 1; i <= Options.numcount; i++)
		{
			//���������
			if (kuohao && i == kuohaol)
			{
				expression.Append(Item('(', true));
			}
			//����һ������
			float number = getNum(1, Options.numrange, Options.hasFushu, Options.hasXiaoshu);
			if (number < 0)
			{
				expression.Append(Item(number));
			}
			else
			{
				expression.Append(Item(number));
			}

			//���������
			if (kuohao &&i == kuohaor)
			{
				expression.Append(Item(')', true));
			}
			//����һ�������
			if (i != Options.numcount)
			{
				expression.Append(Item(getOperator(), true));
			}
		}

		AnswerValue = expression.Evaluate(); //����𰸲�����
		question = expression.ToString();  //�Ƚ���ĿԤ��Ϊ���ʽ�������𰸣�

		//��������������Ŀ
		switch (Type)
		{
			//�����
		case Completion:
			//������ֵ��1λС����
			answer = FloatToString(AnswerValue);
			//��Ŀ�Ǳ��ʽ
			question.push_back('=');
			break;
			//ѡ����
		case Choice:
			int choice;
			question.append(getXuanze(choice));
			answer.push_back('A' + choice - 1);
			break;
			//�ж���
		case TrueFalse:
			bool judge;
			question.push_back('=');
			question.append(getPanduan(judge));
			answer.push_back(judge ? 'T' : 'F');
			break;
		}
	}
private:
	ProblemSet Options; //��������
	ProblemType Type; //����
	Expression expression;  //���ʽ
	float AnswerValue;  //����ֵ
	
	int getRand(int min, int max)//����һ��[min,max]��Χ�������
	{
		return (rand() % (max - min + 1)) + min;
	}  

	bool getBool(float probability)  //����������bool��
	{
		return getRand(0, 10) <= probability * 10;
	}

	float getNum(int min, int max, bool allowNegative, bool allowDecimal)  //�������һ������
	{
		//����ע���ˣ���Ҷ����ö�
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
		//������������ø����ų��ֵĸ���
		int i = getRand(0, 10);
		if (i <= 3) return '+';
		if (i <= 6) return '-';
		if (i <= 9) return '*';
		if (i <= 10) return '/';
	}

	string FloatToString(float f)//floatתstring(��ౣ��1λС��)
	{
		char str[200];
		if (f == floor(f)) sprintf_s(str,"%.0f",f);
		else sprintf_s(str,"%.1f",f);
		return str;
	}

	string getXuanze(int &rightchoice) //����ѡ����ѡ��
	{
		rightchoice = getRand(1, 3);
		float a[4];
		//����������
		for (int i = 1; i <= 3; i++)
		{
			//������ȷѡ��
			if (i == rightchoice) a[i] = AnswerValue;
			//���ɴ���ѡ��
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
		//��ת�����ַ���
		string result("=\n");
		for (int i = 1; i <= 3; i++) 
		{
			result.push_back('A' + i - 1);
			result+="."+FloatToString(a[i])+"	";
		}
		return result;
	}

	string getPanduan(bool &judge)  //�����ж���ʹ�
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