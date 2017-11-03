#pragma once

typedef struct QuestionSetup
{
	int MaxNumbersRange = 100;  //���ֵķ�Χ
	int MaxNumbersCount = 2;  //���ֵ�����
	bool IncludeNegativeNumber = false;  //�Ƿ��������
	bool IncludeDecimal = false;  //�Ƿ����С��
	bool IncludeBrackets = false;  //�Ƿ��������
	float ProbabilityOfNegative = 0.2; //���ָ����ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ
	float ProbabilityOfDecimal = 0.3;  //����С���ĸ��ʣ�Сѧ���⣬С����Ҫ̫�ࣩ
	float ProbabilityOfBrackets = 0.7;  //�������ŵĸ��ʣ�Сѧ���⣬���Ų�Ҫ̫�ࣩ
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
	QuestionSetup Setups; //��������
	//CString Expression;  //���ʽ
	Expression expression;  //���ʽ
	float AnswerValue;
	CString Answer;  //�𰸣��ַ�����

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

int Question::GetRandom(int min, int max)  //����һ��[min,max]��Χ�������
{
	return (rand() % (max - min + 1)) + min;
}

bool Question::GetRandomBool(float probability)  //����������bool��
{
	return GetRandom(0, 10) <= probability * 10;
}

float Question::GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //�������һ������
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

char Question::GetOperator()  //�������һ�������
{
	//������������ø����ų��ֵĸ���
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

void Question::PrintQuestion(bool NewLine, bool ShowAnswer)//������ʽ
{
	cout << expression.ToString() << "=";
	if (ShowAnswer) cout << Answer;
	if (NewLine) cout << endl;
} 

void Question::Generate()//���ɱ��ʽ
{
	int leftbracket = 0;
	int rightbracket = 0; 
	bool needbrackets = Setups.IncludeBrackets;  //�Ƿ�Ӧ����������
	if (needbrackets) needbrackets = GetRandomBool(Setups.ProbabilityOfBrackets); //�����Ҫ���ŵĻ�����������������
	if (Setups.MaxNumbersCount <= 2)  needbrackets = false; //�������������3������Ӧ����������

	if (needbrackets)  //����������ŵ�λ��
	{
		leftbracket = GetRandom(1, Setups.MaxNumbersCount - 1);
		if (leftbracket==1) rightbracket = GetRandom(leftbracket + 1, Setups.MaxNumbersCount-1);
		else rightbracket = GetRandom(leftbracket + 1, Setups.MaxNumbersCount);
	}

	for (int i = 1; i <= Setups.MaxNumbersCount; i++)
	{
		//���������
		if (needbrackets && i == leftbracket)
			//Expression.AppendChar('(');
			expression.Append(ExpressionItem('(', true));
		//����һ������
		float number = GetNumber(1, Setups.MaxNumbersRange, Setups.IncludeNegativeNumber, Setups.IncludeDecimal);
		if (number < 0)
			expression.Append(ExpressionItem(number));
		else 
			expression.Append(ExpressionItem(number));
		//Expression.Append(ToString(number));
		
		//���������
		if (needbrackets &&i == rightbracket) expression.Append(ExpressionItem(')', true));
		//����һ�������
		if (i!= Setups.MaxNumbersCount) expression.Append(ExpressionItem(GetOperator(), true));
	}

	//����𰸲�����
	AnswerValue = expression.Evaluate();
	if (AnswerValue==floor(AnswerValue)) Answer.Format("%.0f", AnswerValue);
	else Answer.Format("%.1f", AnswerValue);
}