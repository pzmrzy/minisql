#include"minisql.h"
#include"sqlcommand.h"
#include"interpreter.h"
#include"api.h"
#include<iostream>
#include<string>
#include<fstream>
using namespace std;
string Wdbname = "";
int main () {
	
	string input;
	Interpreter cmd;
	SqlCommand sql;
	int type;
	while (true) {

		input = cmd.readInput();
		sql = cmd.getExpression(input);
		type = sql.gettype();

		if( type == SQL_EXECFILE ) {
			ifstream ef = ifstream(sql.getTableName());
			string line;
			while(!ef.eof()) {
				line = ef.getline();
				sql = cmd.getExpression(line);
				type = sql.gettype();
				api(type, sql);
			}
			ef.close();
		}

	}
	return 0;
}