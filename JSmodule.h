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
	string tname; //类型名
	double num;   //存数值
	string str;   //存字符串
};

class JSVariable
{
	public:
	string name;  //变量名
	string tname; //类型名
	bool defined; //是否定义过
	vector<JSData> data; //Array数据
	JSData _data; //Number/Sring Data
	int level; //
};

typedef vector<JSVariable> JSVariables;

//表示函数实体的类
class JSFunction
{
	public:
	string name;
	JSVariables variables;
	int levelcount;
	JSVariable returnvalue;
};

//函数定义的类
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
bool endcall(); //结束一个函数调用
bool uplevel(); //检测到{，提升一层
bool downlevel(); //检测到}，降低一层

bool isexistVar(string name);
bool createVar(string var, bool isGlobal);
JSVariable& getVariable(string name);
bool setVar(string name, JSVariable val);
bool unsetVar(string name);

#endif