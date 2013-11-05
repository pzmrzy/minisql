/**
 * @file  test_interpreter.cpp
 * @brief ½âÊÍÆ÷µÄ²âÊÔ
 */
/*
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
	
	string db = "tes";
	string tb1 = "stu";
	string tb2 = "cou";
	catalog C;
	C.init(db);
	SqlCommand cmd;
	catainfo CI;
	cmd.setDatabaseName(db);
	cmd.setTableName(tb1);
	//create table
	cmd.colNameVector.push_back("id");
	cmd.colType.push_back(11);
	cmd.colSpecialVector.push_back("primary");

	cmd.colNameVector.push_back("age");
	cmd.colType.push_back(0);
	cmd.colSpecialVector.push_back("not null");
	
	cmd.colNameVector.push_back("score");
	cmd.colType.push_back(-1);
	cmd.colSpecialVector.push_back("unique");
	CI = C.creat_Table(cmd);
	cmd.setTableName(tb2);
	cmd.colNameVector.push_back("idc");
	cmd.colType.push_back(11);
	cmd.colSpecialVector.push_back("primary");

	cmd.colNameVector.push_back("name");
	cmd.colType.push_back(11);
	cmd.colSpecialVector.push_back("not null");
	
	cmd.colNameVector.push_back("tea");
	cmd.colType.push_back(11);
	cmd.colSpecialVector.push_back("unique");
	CI = C.creat_Table(cmd);

	//insert/delete rec
	
	cmd.colValueVector.push_back("1234567890");
	cmd.colValueVector.push_back("18.1");
	cmd.colValueVector.push_back("87.6");
	
	CI = C.insert_Rec(cmd);
	
	cmd.condLeftVector.push_back("sco");
	cmd.condOpVector.push_back("=");
	cmd.condRightVector.push_back("1");

	CI = C.delete_Rec(cmd);
	CI = C.select_Rec(cmd);
	int a = 0;
	cmd.setTableName(tb1);
	//CI = C.drop_Table(cmd);
	string t = "age";
	cmd.setcolName(t);
	CI = C.creat_Index(cmd);
	CI = C.drop_Index(cmd);
	CI = C.drop_Database(cmd);
	a=1;
	
	//system("pause");
}
*/