#include "JSmodule.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

using namespace std;

int main()
{
	createVar("x", true);
	if (isexistVar("x"))
	{
		cout << "x found!\n";
	}
	else
	{
		cout << "x not found\n";
	}

	JSVariable& x = getVariable("x");
	x.tname = NUMBER;
	x._data.num = 10;
	cout << x._data.num << endl;

	setVar("x", "hahaha");
	cout << x._data.str << endl;

	system("pause");
}