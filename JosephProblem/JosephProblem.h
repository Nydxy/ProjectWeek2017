#pragma once
#include<iostream>
#include<stdlib.h>
#include<time.h>

using namespace std;

int GetRandom(int a, int b);

//��-��
class Person
{
public:
	int index;//���
	int code;//����
	Person *next;
	Person() {}
	Person(int i, int m)
	{
		index = i;
		code = m;
	}
};