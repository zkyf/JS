#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

#ifndef _JSMODULE_H
#define _JSMODULE_H

#define NUMBER "Number"
#define STRING "String"
#define ARRAY  "Array"
#define UNDEFINED "Undefined"

class Data
{
	public:
	string tname; //������
	double num;   //����ֵ
	string str;   //���ַ���
};

class Variable
{
	public:
	string name;  //������
	string tname; //������
	bool defined; //�Ƿ����
	vector<Data> data; //����
	int level; //
};

typedef vector<Variable> Variables;

//��ʾ����ʵ�����
class Function
{
	public:
	string name;
	Variables variables;
	int levelcount;
	Variable returnvalue;
};

//�����������
class FunctionDef
{
	public:
	string name;
	string rtype;
	vector<string> paraname;
	int numofpara;
	string code;
};

typedef vector<Function> Functions;

vector<FunctionDef> FunctionDefs;
Functions Stack;
Variables globals;

bool isexistFunc(string name, int numofpara);
bool createFunc(string name, string rtype, vector<string>paras, char *code);
Variable callFunc(string name, Variables paras);
void setReturnval(Variable returnval);
bool endcall(); //����һ����������
bool uplevel(); //��⵽{������һ��
bool downlevel(); //��⵽}������һ��

bool isexistVar(string name);
bool createVar(Variable var, bool global);
Variable& getVariable(string name);
bool setVar(string name, Variable val);
bool unsetVar(string name);

#endif