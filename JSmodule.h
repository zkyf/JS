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
	string tname; //类型名
	double num;   //存数值
	string str;   //存字符串
};

class Variable
{
	public:
	string name;  //变量名
	string tname; //类型名
	bool defined; //是否定义过
	vector<Data> data; //数据
	int level; //
};

typedef vector<Variable> Variables;

//表示函数实体的类
class Function
{
	public:
	string name;
	Variables variables;
	int levelcount;
	Variable returnvalue;
};

//函数定义的类
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
bool endcall(); //结束一个函数调用
bool uplevel(); //检测到{，提升一层
bool downlevel(); //检测到}，降低一层

bool isexistVar(string name);
bool createVar(Variable var, bool global);
Variable& getVariable(string name);
bool setVar(string name, Variable val);
bool unsetVar(string name);

#endif