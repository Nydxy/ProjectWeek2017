#include "JosephProblem.h"
#include"Circlist.h"

int GetRandom(int a, int b)
{
	return (rand() % (b - a + 1)) + a;
}

int main()
{
	srand(time(NULL));
	const int N = 10;
	Circlist People;
	for (int i = 1; i <= N; i++)
	{
		People.insertLast(new Person(i, GetRandom(1, N)));
	}
	cout << "初始：";
	People.Print();
	People.Play(N);
	return 0;
}