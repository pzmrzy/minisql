#include "interpreter.h"
#include <iostream>
#include <atltrace.h>
#include <stdlib.h>

#define TRACE ATLTRACE

using namespace std;

void main() {
	//cout << "Hello";
	
	Interpreter test;
	string testOutput;

	testOutput = test.readInput();
	
	cout << "\n\n" + testOutput;

	system("pause");
	
}
