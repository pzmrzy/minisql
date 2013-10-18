#include "interpreter.h"
#include <iostream>
#include <atltrace.h>
#include <stdlib.h>

#define TRACE ATLTRACE

using namespace std;

void main() {

	Interpreter test;
	/*
	string testout1("drop database stu;");
	string testout2("drop table stu;");

	SqlCommand sql = test.dropDatabase(testout1);
	sql.print();
	sql = test.dropTable(testout2);
	sql.print();
	*/
	//cout << test.firstWord(testOutput, " )(") << "|" << endl;
	//cout << test.delFirstWord(testOutput, " ()") << "|" << endl;
	
	
	string     readInputResult = test.readInput();
	SqlCommand sql             = test.getExpression(readInputResult);
	
	sql.print();
	

	system("pause");
}
