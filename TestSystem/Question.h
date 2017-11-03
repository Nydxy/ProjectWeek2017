#pragma once

typedef struct QuestionSetup
{
	int MaxNumbersRange = 100;  //数字的范围
	int MaxNumbersCount = 2;  //数字的数量
	bool IncludeNegativeNumber = false;  //是否包含负数
	bool IncludeDecimal = false;  //是否包含小数
	bool IncludeBrackets = false;  //是否包含括号
	float ProbabilityOfNegative = 0.2; //出现负数的概率（小学生题，负数不要太多）
	float ProbabilityOfDecimal = 0.3;  //出现小数的概率（小学生题，小数不要太多）
	float ProbabilityOfBrackets = 0.7;  //出现括号的概率（小学生题，括号不要太多）
	QuestionSetup(){}
	QuestionSetup(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets)
	{
		MaxNumbersRange = maxNumbersRange;
		MaxNumbersCount = maxNumbersCount;
		IncludeNegativeNumber = includeNegativeNumber;
		IncludeDecimal = includeDecimal;
		IncludeBrackets = includeBrackets;
	}
} QuestionSetup;

class Question
{
private:
	QuestionSetup Setups; //出题设置
	//CString Expression;  //表达式
	Expression expression;  //表达式
	float AnswerValue;
	CString Answer;  //答案（字符串）

	static int GetRandom(int min, int max);
	static bool GetRandomBool(float probability);
	float GetNumber(int min, int max, bool allowNegative, bool allowDecimal);
	static char GetOperator();
public:
	Question() {}
	Question(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets);
	Question(QuestionSetup questionSetup);
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

float Question::GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //随机生成一个数字
{
	float result = (float)GetRandom(min, max);
	if (allowNegative&&GetRandomBool(Setups.ProbabilityOfNegative))
	{
		int rnd = GetRandom(0, 1);
		if (rnd) result = -result;
	}
	if (allowDecimal&&GetRandomBool(Setups.ProbabilityOfDecimal))
	{
		float rnd = (float)GetRandom(0, 9);
		rnd = rnd / 10.0;
		result = result + rnd;
	}
	return result;
}

char Question::GetOperator()  //随机生成一个运算符
{
	//在这里可以设置各符号出现的概率
	int i = GetRandom(0, 10);
	if (i <= 3) return '+';
	if (i <= 6) return '-';
	if (i <= 8) return '*';
	if (i <= 10) return '/';
}

Question::Question(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets)
{
	Setups.MaxNumbersRange = maxNumbersRange;
	Setups.MaxNumbersCount = maxNumbersCount;
	Setups.IncludeNegativeNumber = includeNegativeNumber;
	Setups.IncludeDecimal = includeDecimal;
	Setups.IncludeBrackets = includeBrackets;
}

Question::Question(QuestionSetup questionSetup)
{
	Setups = questionSetup;
}

void Question::PrintQuestion(bool NewLine, bool ShowAnswer)//输出表达式
{
	cout << expression.ToString() << "=";
	if (ShowAnswer) cout << Answer;
	if (NewLine) cout << endl;
} 

void Question::Generate()//生成表达式
{
	int leftbracket = 0;
	int rightbracket = 0; 
	bool needbrackets = Setups.IncludeBrackets;  //是否应该生成括号
	if (needbrackets) needbrackets = GetRandomBool(Setups.ProbabilityOfBrackets); //如果需要括号的话，按概率生成括号
	if (Setups.MaxNumbersCount <= 2)  needbrackets = false; //如果运算数少于3个，不应该生成括号

	if (needbrackets)  //随机生成括号的位置
	{
		leftbracket = GetRandom(1, Setups.MaxNumbersCount - 1);
		if (leftbracket==1) rightbracket = GetRandom(leftbracket + 1, Setups.MaxNumbersCount-1);
		else rightbracket = GetRandom(leftbracket + 1, Setups.MaxNumbersCount);
	}

	for (int i = 1; i <= Setups.MaxNumbersCount; i++)
	{
		//添加左括号
		if (needbrackets && i == leftbracket)
			//Expression.AppendChar('(');
			expression.Append(ExpressionItem('(', true));
		//生成一个数字
		float number = GetNumber(1, Setups.MaxNumbersRange, Setups.IncludeNegativeNumber, Setups.IncludeDecimal);
		if (number < 0)
			expression.Append(ExpressionItem(number));
		else 
			expression.Append(ExpressionItem(number));
		//Expression.Append(ToString(number));
		
		//添加右括号
		if (needbrackets &&i == rightbracket) expression.Append(ExpressionItem(')', true));
		//生成一个运算符
		if (i!= Setups.MaxNumbersCount) expression.Append(ExpressionItem(GetOperator(), true));
	}

	//求出答案并保存
	AnswerValue = expression.Evaluate();
	if (AnswerValue==floor(AnswerValue)) Answer.Format("%.0f", AnswerValue);
	else Answer.Format("%.1f", AnswerValue);
}