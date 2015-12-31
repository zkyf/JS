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
	system("pause");
}