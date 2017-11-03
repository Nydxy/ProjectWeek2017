#pragma once

typedef struct QuestionSetup
{
	int MaxNumbersRange = 100;  //数字的范围
	int MaxNumbersCount = 2;  //数字的数量
	bool IncludeNegativeNumber = false;  //是否包含负数
	bool IncludeDecimal = false;  //是否包含小数
	bool IncludeBrackets = false;  //是否包含括号
	float ProbabilityOfNegative = 0.3; //出现负数的概率（小学生题，负数不要太多）
	float ProbabilityOfDecimal = 0.5;  //出现小数的概率（小学生题，负数不要太多）
} QuestionSetup;

class Question
{
private:
	QuestionSetup Setups;
	int MaxNumbersRange = 100;  //数字的范围
	int MaxNumbersCount = 2;  //数字的数量
	bool IncludeNegativeNumber = false;  //是否包含负数
	bool IncludeDecimal = false;  //是否包含小数
	bool IncludeBrackets = false;  //是否包含括号
	float ProbabilityOfNegative = 0.3; //出现负数的概率（小学生题，负数不要太多）
	float ProbabilityOfDecimal = 0.5;  //出现小数的概率（小学生题，负数不要太多）

	CString Expression;  //表达式
	float Answer;

	static int GetRandom(int min, int max);
	static bool GetRandomBool(float probability);
	static CString ToString(float number);
	float GetNumber(int min, int max, bool allowNegative, bool allowDecimal);
	static char GetOperator();
public:
	Question() {}
	Question(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets);
	void Setup(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets);
	void PrintQuestion(bool NewLine = true, bool ShowAnswer=false);
	void Generate();
};

int Question::GetRandom(int min, int max)  //生成一个[min,max]范围的随机数
{
	return (rand() % (max - min + 1)) + min;
}
bool Question::GetRandomBool(float probability)  //按概率生成bool型
{
	return GetRandom(0, 10) <= probability * 10;
}
CString Question::ToString(float number)//数字转字符串
{
	CString str;
	str.Format("%g", number);
	return str;
}
float Question::GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //随机生成一个数字
{
	float result = GetRandom(min, max);
	if (allowNegative&&GetRandomBool(ProbabilityOfNegative))
	{
		int rnd = GetRandom(0, 1);
		if (rnd) result = -result;
	}
	if (allowDecimal&&GetRandomBool(ProbabilityOfDecimal))
	{
		float rnd = GetRandom(0, 9);
		rnd = rnd / 10.0;
		result = result + rnd;
	}
	return result;
}
char Question::GetOperator()  //随机生成一个运算符
{
	int rnd = GetRandom(1, 4);
	switch (rnd)
	{
	case 1:return '+';
	case 2:return '-';
	case 3:return '*';
	case 4:return '/';
	}
}
Question::Question(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets)
{
	MaxNumbersRange = maxNumbersRange;
	MaxNumbersCount = maxNumbersCount;
	IncludeNegativeNumber = includeNegativeNumber;
	IncludeDecimal = includeDecimal;
	IncludeBrackets = includeBrackets;
	//srand(time(0));
}
void Question::Setup(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets) //设置表达式选项
{
	MaxNumbersRange = maxNumbersRange;
	MaxNumbersCount = maxNumbersCount;
	IncludeNegativeNumber = includeNegativeNumber;
	IncludeDecimal = includeDecimal;
	IncludeBrackets = includeBrackets;
}
void Question::PrintQuestion(bool NewLine, bool ShowAnswer)//输出表达式
{
	cout << Expression << "=";
	if (ShowAnswer) cout << Answer;
	if (NewLine) cout << endl;
} 
void Question::Generate()//生成表达式
{
	for (int i = 1; i <= MaxNumbersCount; i++)
	{
		Expression+=ToString(GetNumber(1, MaxNumbersRange, IncludeNegativeNumber, IncludeDecimal));
		if (i!= MaxNumbersCount) Expression.AppendChar(GetOperator());
	}
	Answer = EvaluateExpression(Expression);
}