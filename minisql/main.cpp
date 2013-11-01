#include"minisql.h"
#include"sqlcommand.h"
#include"interpreter.h"
#include"api.h"
#include<iostream>
#include<string>
using namespace std;
int main () {
	string input;
	Interpreter cmd;
	SqlCommand sql;
	int type;
	while (true) {
		getline(cin, input);
		if (cin == "Exit")
			break;
		input = cmd.readInput();
		sql = cmd.getExpression(input);
		type = sql.gettype();
		api(type, sql);
	}
	return 0;
}