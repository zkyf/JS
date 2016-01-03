
#include <iostream>
#include <string>
#include <fstream>
#include "ExpressionAnalyser.h"
using namespace std;
ExpressionAnalyser woot;
int main()
{
	string tem;
	getline(cin, tem);
	createVar("_x", true);
	JSVariable& x = getVariable("_x");
	x.tname = NUMBER;
	x._data.num = 10;

	woot.readExpression(tem);
	woot.writeExpression();
	woot.calcExpression();
	JSVariable hehe = woot.getResult();
	if (woot.getErrorcode() == 0)
	{
		if (hehe.tname == STRING)
			cout << hehe._data.str << endl;
		else if (hehe.tname == NUMBER)
			cout << hehe._data.num << endl;
		cout << woot.isAssignment() << endl;
	}
	else
	{
		woot.showErrors();
	}
	return 0;
}