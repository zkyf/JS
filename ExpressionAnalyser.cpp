#include "ExpressionAnalyser.h"


ExpressionAnalyser::ExpressionAnalyser()
{
	mp.clear();
	mp["#"] = -10086;
	mp["=="] = 0;
	mp["!="] = 0;
	mp[">"] = 0;
	mp["<"] = 0;
	mp["<="] = 0;
	mp[">="] = 0;
	mp["+"] = 1;
	mp["-"] = 1;
	mp["*"] = 2;
	mp["/"] = 2;
	mp["("] = 10086;
	mp[")"] = 10086;
	mp["!"] = 3;
	mp["&&"] = -1;
	mp["||"] = -1;
	mp["="] = -2;
}
bool ExpressionAnalyser::isAssignment()
{
	return isAssign;
}
void ExpressionAnalyser::readExpression(string tt)
{
	expInput = tt;
}
JSVariable ExpressionAnalyser::getResult()
{
	return expOutput;
}
void ExpressionAnalyser::writeExpression()
{
}
string ExpressionAnalyser::splitter(string tt, int& st)
{
	string ret = "";
	for (int i = st; i < tt.length(); i++)
	{
		if (tt[i] == ' ' || tt[i] == ';'||tt[i]=='\n')
		{
			st = i + 1;
			return ret;
		}
		else
		{
			ret = ret + tt[i];
			st = st + 1;
		}
	}
	return ret;
}
bool isNumber(string ss)
{
	int num = 0;
	for (int i = 0; i < ss.length();i++)
	{
		if (ss[i] == '-')
		{
			if (i!=0)
				return false;
		}
		else if (ss[i] == '.')
			num++;
		else if (!(ss[i] >= '0'&&ss[i] <= '9'))
			return false;
	}
	if (num>1)
		return false;
	return true;
}
bool ExpressionAnalyser::isVarible(string ss)
{
	if (!(ss[0] == '_'||((ss[0] >= 'A') && (ss[0] <= 'Z')) ||((ss[0] >= 'a') && (ss[0] <= 'z'))))
		return false;
	for (int i = 0; i < ss.length(); i++)
	{
		if (ss[i] == '.' || ss[i] == '(' || ss[i] == ')')
			return false;
	}
	return true;
}
JSVariable ExpressionAnalyser::stringToIdentifier(string ss)
{
	JSVariable ret;
	ret.defined = false;
	if (isNumber(ss))
	{
		double retf = atof(ss.c_str());
		ret.defined = true;
		ret._data.num = retf;
		ret.tname = NUMBER;
		ret._data.tname = NUMBER;
	}
	else if (ss[0] == '"')
	{
		ret.defined = true;
		ret._data.str = ss.substr(1, ss.length() - 2);
		ret._data.tname = STRING;
		ret.tname = STRING;
	}
	else if (isVarible(ss))
	{
		if (isexistVar(ss))
			ret = getVariable(ss);
		else
			ret.defined = false, error_Ret |= 16;
	}
	else
	{
		ret.defined = false;
		error_Ret |= 32;
	}
	return ret;
}
bool ExpressionAnalyser::isVar(string tt)
{
	//in the map
	if (mp.find(tt)!=mp.end())
		return false;
	return true;
}
int ExpressionAnalyser::getErrorcode()
{
	return error_Ret;
}
void ExpressionAnalyser::calcPost()
{
	isAssign = false;
	while (!verStk.empty())
		verStk.pop();
	for (int i = 0; i < expPost.size(); i++)
	{
		if (isVar(expPost[i]))
			verStk.push(stringToIdentifier(expPost[i]));
		else
		{
			JSVariable op1, op2;
			if (expPost[i] == "!")
			{
				if (verStk.empty())
					error_Ret |= 1;
				else
					op2 = verStk.top(), verStk.pop();

				if (op2.tname == NUMBER)
				{
					if (abs(op2._data.num) < 1e-5)
						op2._data.num = 1.0;
					else
						op2._data.num = 0.0;
				}
				else
					error_Ret |= 2, op2.defined = false;

				verStk.push(op2);
				continue;
			}
			if (verStk.empty())
				error_Ret|=1;
			else
				op2 = verStk.top(), verStk.pop();
			if (verStk.empty())
				error_Ret|=1;
			else
				op1 = verStk.top(), verStk.pop();

			if (expPost[i] == "+")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = op1._data.num + op2._data.num;
				else if (op1.tname == STRING&&op2.tname == STRING)
					op1._data.str = op1._data.str + op2._data.str;
				else
					error_Ret|=2, op1.defined=false;
				verStk.push(op1);
			}
			if (expPost[i] == "-")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = op1._data.num - op2._data.num;
				else
					error_Ret|=2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "*")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = op1._data.num * op2._data.num;
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "/")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = op1._data.num / op2._data.num;
				else
					error_Ret|=2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "==")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num == op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str == op2._data.str)
						op1._data.num= 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;			
				}
				else
					error_Ret|=2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "!=")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num != op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str != op2._data.str)
						op1._data.num = 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;
				}
				else
					error_Ret|=2, op1.defined = false;
				verStk.push(op1);
			}			// work on two 
			if (expPost[i] == ">")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num > op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str > op2._data.str)
						op1._data.num = 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == ">=")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num >= op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str >= op2._data.str)
						op1._data.num = 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "<=")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num <= op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str <= op2._data.str)
						op1._data.num = 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "<")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
					op1._data.num = (op1._data.num < op2._data.num);
				else if (op1.tname == STRING&&op2.tname == STRING)
				{
					if (op1._data.str < op2._data.str)
						op1._data.num = 1;
					else
						op1._data.num = 0;
					op1.tname = NUMBER;
					op1._data.tname = NUMBER;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "&&")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
				{
					if ((abs(op1._data.num) > 1e-5) && (abs(op2._data.num) > 1e-5))
						op1._data.num = 1.0;
					else
						op1._data.num = 0.0;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "||")
			{
				if (op1.tname == NUMBER&&op2.tname == NUMBER)
				{
					if ((abs(op1._data.num) > 1e-5) || (abs(op2._data.num) > 1e-5))
						op1._data.num = 1.0;
					else
						op1._data.num = 0.0;
				}
				else
					error_Ret |= 2, op1.defined = false;
				verStk.push(op1);
			}
			if (expPost[i] == "=")
			{
				if (isexistVar(op1.name))
				{
					JSVariable& x = getVariable(op1.name);
					x.data = op2.data;
					x.defined = op2.defined;
					x.tname = op2.tname;
					x._data.num = op2._data.num;
					x._data.str = op2._data.str;
					x._data.tname = op2._data.tname;
					isAssign = true;
//					cout << x._data.tname << endl;
				}
				else
					op1.defined = false, expOutput.defined = false, isAssign = true,error_Ret|=8;
				verStk.push(op1);
			}
		}
	}
	expOutput = verStk.top();
	verStk.pop();
	/*
	expOutput.Tname = verStk.top().Tname;
	expOutput.Defined = verStk.top().Defined;
	expOutput.Level = verStk.top().Level;
	expOutput.Data.clear();
	for (int i = 0; i < verStk.top().Data.size(); i++)
		expOutput.Data.push_back(verStk.top().Data[i]);
	verStk.pop();
	*/
}
void ExpressionAnalyser::showErrors()
{
	if ((error_Ret & 0x01) != 0)
		cout << "污♂特报警: Missing varibles" << endl;
	if ((error_Ret & 0x02) != 0)
		cout << "污♂特报警: the varibles types mismatch,or the operation between the varibles not available" << endl;
	if ((error_Ret & 0x04) != 0)
		cout << "污♂特报警: the ( and ) mismatched" << endl;
	if ((error_Ret & 0x08) != 0)
		cout << "污♂特报警: such operater is not available" << endl;
	if ((error_Ret & 0x10) != 0)
		cout << "污♂特报警: no such varible " << endl;
	if ((error_Ret & 0x20) != 0)
		cout << "污♂特报警: illegal varible" << endl;
}
void ExpressionAnalyser::calcExpression()
{
	int st = 0;
	error_Ret = 0;
	while (!verStk.empty())	verStk.pop();
	while (!optStk.empty())	optStk.pop();
	expPost.clear();
	optStk.push("#");
	if (expInput == "")
	{
		expOutput.defined = false;
	}
	else
	{
		// first we check out the 
		while (true)
		{
			if (st >= expInput.length())	
				break;
			string stem1 = splitter(expInput,st);
			//firt trans to post
//			cout << stem1<<" "<<st<<" "<<expInput.length()<< endl;
			if (isVar(stem1))	//is digit
				expPost.push_back(stem1);
			else
			{
				if (stem1 == ")")
				{
					while (optStk.top() != "(")
					{
						if (optStk.empty())
							error_Ret |= 4;
						expPost.push_back(optStk.top());
						optStk.pop();
					}
					optStk.pop();
				}
				else if (mp[stem1]>mp[optStk.top()])
				{
					optStk.push(stem1);
				}
				else
				{
					do 
					{
						if (optStk.top() == "(")
							break;
						expPost.push_back(optStk.top());
						optStk.pop();
					} while (mp[optStk.top()] >= mp[stem1]);
					optStk.push(stem1);
				}
			}

		}
		while (optStk.top()!="#")
		{
			if (optStk.top() != "(")
				expPost.push_back(optStk.top());
			optStk.pop();
		}
	}
//	cout << "#######################" << endl;
//	for (int i = 0; i < expPost.size(); i++)
//		cout << expPost[i] << endl;
	calcPost();
	
}

ExpressionAnalyser::~ExpressionAnalyser()
{
}
