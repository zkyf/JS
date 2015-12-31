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

class JSData
{
	public:
	string tname; //������
	double num;   //����ֵ
	string str;   //���ַ���
};

class JSVariable
{
	public:
	string name;  //������
	string tname; //������
	bool defined; //�Ƿ����
	vector<JSData> data; //Array����
	JSData _data; //Number/Sring Data
	int level; //
};

typedef vector<JSVariable> JSVariables;

//��ʾ����ʵ�����
class JSFunction
{
	public:
	string name;
	JSVariables variables;
	int levelcount;
	JSVariable returnvalue;
};

//�����������
class JSFunctionDef
{
	public:
	string name;
	string rtype;
	vector<string> paraname;
	int numofpara;
	string code;
};

typedef vector<JSFunction> JSFunctions;

static vector<JSFunctionDef> FunctionDefs;
static JSFunctions Stack;
static JSVariables globals;

bool isexistFunc(string name, int numofpara);
bool createFunc(string name, string rtype, vector<string>paras, char *code);
JSVariable callFunc(string name, JSVariables paras);
void setReturnval(JSVariable returnval);
bool endcall(); //����һ����������
bool uplevel(); //��⵽{������һ��
bool downlevel(); //��⵽}������һ��

bool isexistVar(string name);
bool createVar(string var, bool isGlobal);
JSVariable& getVariable(string name);
bool setVar(string name, JSVariable val);
bool unsetVar(string name);

#endif