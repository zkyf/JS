#include "JSmodule.h"

JSData::JSData() :
tname(UNDEFINED)
{}

JSVariable::JSVariable() :
name(""), tname(UNDEFINED), defined(false)
{}

JSVariable::JSVariable(string _name) :
name(_name), tname(UNDEFINED), defined(false)
{}

JSVariable::JSVariable(string _name, string _tname) :
name(_name), tname(_tname), defined(false)
{}

void JSVariable::operator=(JSVariable& b)
{
	tname = b.tname;
	defined = b.defined;
	_data = b._data;
	data.clear();
	for (vector<JSData>::iterator i = b.data.begin();
			 i != b.data.end(); i++)
	{
		data.push_back(*i);
	}
}

JSData JSVariable::value()
{
	JSData _val;
	if(tname == NUMBER)
	{
		_val.tname = NUMBER;
		_val.num = _data.num;
	}
	else if (tname == STRING)
	{
		_val.tname = STRING;
		_val.str = _data.str;
	}
	return _val;
}

JSData JSVariable::at(int index)
{
	JSData _val;
	if (tname == ARRAY && index < data.size())
	{
		_val = data[index];
	}
	return _val;
}

bool isexistFunc(string name, int numofpara)
{
	for (vector<JSFunctionDef>::iterator i = FunctionDefs.begin();
			 i != FunctionDefs.end(); i++)
	{
		if (i->name == name && i->numofpara == numofpara)
		{
			return true;
		}
	}
	return false;
}

bool createFunc(string name, string rtype, vector<string>paras, string code)
{
	if (code == "")
	{
		return false;
	}
	int numofpara = 0;
	JSFunctionDef newfunc;
	for (vector<string>::iterator i = paras.begin();
			 i != paras.end(); i++)
	{
		numofpara++;
		newfunc.paraname.push_back(*i);
	}
	if (isexistFunc(name, numofpara))
	{
		return false;
	}
	newfunc.name = name;
	newfunc.rtype = rtype;
	newfunc.numofpara = numofpara;
	newfunc.code = code;
	FunctionDefs.push_back(newfunc);
	return true;
}

JSVariable callFunc(string name, JSVariables paras)
{
	JSVariable nullresult;
	nullresult.defined = false;
	int numofpara = 0;
	for (JSVariables::iterator i = paras.begin();
			 i != paras.end(); i++)
	{
		numofpara++;
	}
	if (!isexistFunc(name, numofpara))
	{
		return nullresult;
	}
	JSFunction torun;
	torun.levelcount = 0;
	torun.name = name;
	for (JSVariables::iterator i = paras.begin();
			 i != paras.end(); i++)
	{
		torun.variables.push_back(*i);
	}
	Stack.push_back(torun);

	//run the function

	return (Stack.end() - 1)->returnvalue;

}

void setReturnval(JSVariable returnval)
{
	(Stack.end() - 1)->returnvalue = returnval;
}

bool endcall()
{
	if (Stack.empty())
	{
		return false;
	}
	Stack.erase(Stack.end() - 1);
	return true;
}

bool uplevel()
{
	if (Stack.empty())
	{
		return false;
	}
	(Stack.end() - 1)->levelcount++;
	return true;
}

bool downlevel()
{
	if (Stack.empty())
	{
		return true;
	}
	JSFunctions::iterator nowfunc = Stack.end() - 1;
	for (JSVariables::iterator i = nowfunc->variables.begin();
			 i != nowfunc->variables.end(); )
	{
		if (i->level == nowfunc->levelcount)
		{
			i = nowfunc->variables.erase(i);
		}
		else
		{
			i++;
		}
	}
	nowfunc->levelcount--;
}

bool isexistVar(string name)
{
	if (!Stack.empty())
	{
		for (JSVariables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				return true;
			}
		}
	}
	for (JSVariables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			return true;
		}
	}
	return false;
}

bool createVar(string name, bool isGlobal)
{
	if (isexistVar(name))
	{
		return false;
	}
	JSVariable newvar;
	newvar.tname = UNDEFINED;
	newvar.name = name;
	newvar.defined = false;
	if (isGlobal)
	{
		globals.push_back(newvar);
	}
	else
	{
		if (Stack.empty())
		{
			return false;
		}
		newvar.level = (Stack.end() - 1)->levelcount;
		(Stack.end() - 1)->variables.push_back(newvar);
	}
	return true;
}

JSVariable& getVariable(string name)
{
	if (!Stack.empty())
	{
		for (JSVariables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				return *i;
			}
		}
	}
	for (JSVariables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			return *i;
		}
	}
}

bool setVar(string name, JSVariable val)
{
	if (!isexistVar(name))
	{
		return false;
	}
	JSVariable& ref = getVariable(name);
	ref = val;
	return true;
}

bool setVar(string name, double number)
{
	JSVariable _val;
	_val.defined = true;
	_val.tname = NUMBER;
	_val._data.num = number;
	return setVar(name, _val);
}

bool setVar(string name, string str)
{
	JSVariable _val;
	_val.defined = true;
	_val.tname = STRING;
	_val._data.str = str;
	return setVar(name, _val);
}

bool setVar(string name, vector<JSData> data)
{
	JSVariable _val;
	_val.defined = true;
	_val.tname = ARRAY;
	for (vector<JSData>::iterator i = data.begin();
			 i != data.end(); i++)
	{
		_val.data.push_back(*i);
	}
	return setVar(name, _val);
}

bool unsetVar(string name)
{
	if (!isexistVar(name))
	{
		return true;
	}
	if (!Stack.empty())
	{
		for (JSVariables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				(Stack.end() - 1)->variables.erase(i);
				return true;
			}
		}
	}
	for (JSVariables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			globals.erase(i);
			return true;
		}
	}
}