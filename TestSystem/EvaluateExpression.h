#pragma once
#include<string>

const int OPSETSIZE = 7;
char OPSET[OPSETSIZE] = { '+','-','*','/','(',')','#' };
unsigned char Prior[7][7] =
{ // ��������ȼ���   
  // '+' '-' '*' '/' '(' ')' '#'    
	/*'+'*/'>','>','<','<','<','>','>',
	/*'-'*/'>','>','<','<','<','>','>',
	/*'*'*/'>','>','>','>','<','>','>',
	/*'/'*/'>','>','>','>','<','>','>',
	/*'('*/'<','<','<','<','<','=',' ',
	/*')'*/'>','>','>','>',' ','>','>',
	/*'#'*/'<','<','<','<','<',' ','=',
};

typedef struct StackChar
{
	char c;
	struct StackChar *next;
}SC;       //StackChar���͵Ľ��SC  

typedef struct StackFloat
{
	float f;
	struct StackFloat *next;
}SF;       //StackFloat���͵Ľ��SF  

SC *Push(SC *s, char c)          //SC���͵�ָ��Push������p  
{
	SC *p = (SC*)malloc(sizeof(SC));
	p->c = c;
	p->next = s;
	return p;
}

SF *Push(SF *s, float f)        //SF���͵�ָ��Push������p  
{
	SF *p = (SF*)malloc(sizeof(SF));
	p->f = f;
	p->next = s;
	return p;
}

SC *Pop(SC *s)    //SC���͵�ָ��Pop  
{
	SC *q = s;
	s = s->next;
	free(q);
	return s;
}

SF *Pop(SF *s)      //SF���͵�ָ��Pop  
{
	SF *q = s;
	s = s->next;
	free(q);
	return s;
}

float Operate(float a, unsigned char theta, float b)      //���㺯��Operate  
{
	switch (theta)
	{
	case '+': return a + b;
	case '-': return a - b;
	case '*': return a*b;
	case '/': return a / b;
	default: return 0;
	}
}

int In(char Test, char *TestOp)
{
	int Find = false;
	for (int i = 0; i< OPSETSIZE; i++)
	{
		if (Test == TestOp[i])
			Find = true;
	}
	return Find;
}

int ReturnOpOrd(char op, char *TestOp)
{
	for (int i = 0; i< OPSETSIZE; i++)
	{
		if (op == TestOp[i])
			return i;
	}
}

char precede(char Aop, char Bop)
{
	return Prior[ReturnOpOrd(Aop, OPSET)][ReturnOpOrd(Bop, OPSET)];
}

float EvaluateExpression(const char* MyExpression)
{
	// ��������ʽ��ֵ����������㷨  
	// ��OPTR��OPND�ֱ�Ϊ�����ջ��������ջ��OPΪ���������   
	SC *OPTR = NULL;       // �����ջ���ַ�Ԫ��   
	SF *OPND = NULL;       // ������ջ��ʵ��Ԫ��   
	char TempData[20];
	float Data, a, b;
	char theta, *c, Dr[] = { '#','\0' };
	OPTR = Push(OPTR, '#');
	char p[100];
	strcpy(p, MyExpression);
	c = strcat(p, Dr);
	strcpy(TempData, "\0");//�ַ�����������   
	while (*c != '#' || OPTR->c != '#')
	{
		if (!In(*c, OPSET))
		{
			Dr[0] = *c;
			strcat(TempData, Dr);           //�ַ������Ӻ���   
			c++;
			if (In(*c, OPSET))
			{
				Data = atof(TempData);       //�ַ���ת������(double)   
				OPND = Push(OPND, Data);
				strcpy(TempData, "\0");
			}
		}
		else    // ������������ջ   
		{
			switch (precede(OPTR->c, *c))
			{
			case '<': // ջ��Ԫ�����ȼ���   
				OPTR = Push(OPTR, *c);
				c++;
				break;
			case '=': // �����Ų�������һ�ַ�   
				OPTR = Pop(OPTR);
				c++;
				break;
			case '>': // ��ջ������������ջ   
				theta = OPTR->c; OPTR = Pop(OPTR);
				b = OPND->f; OPND = Pop(OPND);
				a = OPND->f; OPND = Pop(OPND);
				OPND = Push(OPND, Operate(a, theta, b));
				break;
			} //switch  
		}
	} //while   
	return OPND->f;
} //EvaluateExpression  