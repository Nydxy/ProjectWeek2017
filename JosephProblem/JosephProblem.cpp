#include"Circlist.h"

int main()
{
	srand(time(NULL));
	const int N = 5;
	Circlist People;
	for (int i = 1; i <= N; i++)
	{
		Person *p = new Person(i, GetRandom(1, N));
		People.insertLast(p);
	}
	cout << "初始：";
	People.Print();
	People.Play(N);
	system("pause");
	return 0;
}