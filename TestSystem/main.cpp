#include "TestSystem.h"
#include"Expression.h"
#include"Question.h"

int main()
{
	MenuMain();
	srand(time(0));
	QuestionSetup set = QuestionSetup(20, 10, true, true, true);
	set.ProbabilityOfBrackets = 1;  //恐怖小学算术题
	for (int i = 0; i < 10; i++)
	{
		Question a=Question(set);
		a.Generate();
		a.PrintQuestion(true, true);
	}

	//Expression exp;
	//exp.Parse("(1+2)*3+4/5+(-20.5)");
	//cout << exp.ToString()<<"=";
	//cout<<exp.Evaluate()<<endl;
	//exp.Append(ExpressionItem('*',true));
	//cout << exp[0].Value << endl;
	system("pause");
	return 0;
}
void MenuMain()
{

}