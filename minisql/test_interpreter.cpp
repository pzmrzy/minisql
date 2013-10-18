#include "interpreter.h"
#include <iostream>
#include <atltrace.h>
#include <stdlib.h>

#define TRACE ATLTRACE

using namespace std;

void main() {
	//cout << "Hello";
	
	Interpreter test;
	string testOutput(" This() is me");

	cout << test.firstWord(testOutput, " )(") << "|" << endl;
	cout << test.delFirstWord(testOutput, " ()") << "|" << endl;

	system("pause");
}
