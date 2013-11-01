/**
 * @file  test_interpreter.cpp
 * @brief ½âÊÍÆ÷µÄ²âÊÔ
 */

#include "interpreter.h"
#include "catalog.h"
#include <iostream>
#include <atltrace.h>
#include <stdlib.h>
#include <string>
using namespace std;
#define TRACE ATLTRACE

using namespace std;

void main() {
	string str = "test";
	catalog C;
	C.init(str);
	


	//Interpreter test;
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
	
	
	//string     readInputResult = test.readInput();
	//SqlCommand sql             = test.getExpression(readInputResult);
	
	//sql.print();
	

	system("pause");
}
