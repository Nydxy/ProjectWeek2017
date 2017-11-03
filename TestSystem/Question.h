#pragma once

typedef struct QuestionSetup
{
	int MaxNumbersRange = 100;  //���ֵķ�Χ
	int MaxNumbersCount = 2;  //���ֵ�����
	bool IncludeNegativeNumber = false;  //�Ƿ��������
	bool IncludeDecimal = false;  //�Ƿ����С��
	bool IncludeBrackets = false;  //�Ƿ��������
	float ProbabilityOfNegative = 0.3; //���ָ����ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ
	float ProbabilityOfDecimal = 0.5;  //����С���ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ
} QuestionSetup;

class Question
{
private:
	QuestionSetup Setups;
	int MaxNumbersRange = 100;  //���ֵķ�Χ
	int MaxNumbersCount = 2;  //���ֵ�����
	bool IncludeNegativeNumber = false;  //�Ƿ��������
	bool IncludeDecimal = false;  //�Ƿ����С��
	bool IncludeBrackets = false;  //�Ƿ��������
	float ProbabilityOfNegative = 0.3; //���ָ����ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ
	float ProbabilityOfDecimal = 0.5;  //����С���ĸ��ʣ�Сѧ���⣬������Ҫ̫�ࣩ

	CString Expression;  //���ʽ
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

int Question::GetRandom(int min, int max)  //����һ��[min,max]��Χ�������
{
	return (rand() % (max - min + 1)) + min;
}
bool Question::GetRandomBool(float probability)  //����������bool��
{
	return GetRandom(0, 10) <= probability * 10;
}
CString Question::ToString(float number)//����ת�ַ���
{
	CString str;
	str.Format("%g", number);
	return str;
}
float Question::GetNumber(int min, int max, bool allowNegative, bool allowDecimal)  //�������һ������
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
char Question::GetOperator()  //�������һ�������
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
void Question::Setup(int maxNumbersRange, int maxNumbersCount, bool includeNegativeNumber, bool includeDecimal, bool includeBrackets) //���ñ��ʽѡ��
{
	MaxNumbersRange = maxNumbersRange;
	MaxNumbersCount = maxNumbersCount;
	IncludeNegativeNumber = includeNegativeNumber;
	IncludeDecimal = includeDecimal;
	IncludeBrackets = includeBrackets;
}
void Question::PrintQuestion(bool NewLine, bool ShowAnswer)//������ʽ
{
	cout << Expression << "=";
	if (ShowAnswer) cout << Answer;
	if (NewLine) cout << endl;
} 
void Question::Generate()//���ɱ��ʽ
{
	for (int i = 1; i <= MaxNumbersCount; i++)
	{
		Expression+=ToString(GetNumber(1, MaxNumbersRange, IncludeNegativeNumber, IncludeDecimal));
		if (i!= MaxNumbersCount) Expression.AppendChar(GetOperator());
	}
	Answer = EvaluateExpression(Expression);
}