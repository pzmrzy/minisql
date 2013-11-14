#include"minisql.h"
#include"sqlcommand.h"
#include"interpreter.h"
#include"buffermanager.h"
#include"block.h"
#include"record.h"
#include"api.h"
#include"catalog.h"
#include "stdlib.h"
#include<iostream>
#include<string>
using namespace std;

string Wdbname = "";
BufferManager *bfm;

int main () {
	system("a.bat");
	string input;
	Interpreter cmd;
	SqlCommand sql;
	int type;
	while (true) {
		//getline(cin, input);
		//if (cin == "Exit")
			//break;
		input = cmd.readInput();
		sql = cmd.getExpression(input);
		type = sql.gettype();
		api(type, sql);
	}

BufferManager bb("testa");
Block b;
b = bb.newBlock("t");

	return 0;
}