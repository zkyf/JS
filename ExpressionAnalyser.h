#pragma once
#include <string>
#include <cstdio>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <stack>
#include "JSmodule.h"
#include <map>
using namespace std;

class ExpressionAnalyser
{
private:
	stack<JSVariable> verStk;
	stack<string> optStk;
	string expInput;
	JSVariable expOutput;
	string splitter(string tt, int& st);
	vector<string> expPost;
	JSVariable stringToIdentifier(string tt);
	void calcPost();
	bool isVarible(string tt);
	bool isVar(string tt);
	bool isAssign;
	int error_Ret;
	map<string, int> mp;
public:
	ExpressionAnalyser();
	void readExpression(string tt);
	void calcExpression();
	JSVariable getResult();
	void writeExpression();
	bool isAssignment();
	int getErrorcode();
	void showErrors();
	~ExpressionAnalyser();
};

