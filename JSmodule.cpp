#include "JSmodule.h"

bool isseixtFunc(string name, int numofpara)
{
	for (vector<FunctionDef>::iterator i = FunctionDefs.begin();
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
	FunctionDef newfunc;
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

Variable callFunc(string name, Variables paras)
{
	Variable nullresult;
	nullresult.defined = false;
	int numofpara = 0;
	for (Variables::iterator i = paras.begin();
			 i != paras.end(); i++)
	{
		numofpara++;
	}
	if (!isexistFunc(name, numofpara))
	{
		return nullresult;
	}
	Function torun;
	torun.levelcount = 0;
	torun.name = name;
	for (Variables::iterator i = paras.begin();
			 i != paras.end(); i++)
	{
		torun.variables.push_back(*i);
	}
	Stack.push_back(torun);

	//run the function

	return (Stack.end() - 1)->returnvalue;

}

void setReturnval(Variable returnval)
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
	(Stack.end() - 1)->levelcount--;
}

bool isexistVar(string name)
{
	if (!Stack.empty())
	{
		for (Variables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				return true;
			}
		}
	}
	for (Variables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			return true;
		}
	}
	return false;
}

bool createVar(string name, bool global)
{
	if (isexistVar(name))
	{
		return false;
	}
	Variable newvar;
	newvar.tname = UNDEFINED;
	newvar.name = name;
	newvar.defined = false;
	if (global)
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

Variable& getVariable(string name)
{
	if (!Stack.empty())
	{
		for (Variables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				return *i;
			}
		}
	}
	for (Variables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			return *i;
		}
	}
}

bool setVar(string name, Variable val)
{
	if (!isexistVar(name))
	{
		return false;
	}
	Variable& ref = getVariable(name);
	ref = val;
	return true;
}

bool unsetVar(string name)
{
	if (!isexistVar(name))
	{
		return true;
	}
	if (!Stack.empty())
	{
		for (Variables::iterator i = (Stack.end() - 1)->variables.begin();
				 i != (Stack.end() - 1)->variables.end(); i++)
		{
			if (i->name == name)
			{
				(Stack.end() - 1)->variables.erase(i);
				return true;
			}
		}
	}
	for (Variables::iterator i = globals.begin();
			 i != globals.end(); i++)
	{
		if (i->name == name)
		{
			globals.erase(i);
			return true;
		}
	}
}