#pragma once
#include<iostream>
#include<stdlib.h>
#include<time.h>

using namespace std;

int GetRandom(int a, int b);

//ÈË-Àà
class Person
{
public:
	int index;//ĞòºÅ
	int code;//ÃÜÂë
	Person *next;
	Person() {}
	Person(int i, int m)
	{
		index = i;
		code = m;
	}
};