#include "interpreter.h"
#include <cstring>
using namespace std;

string delFirstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	    start   = str.find_first_of(split, start);
		start   = str.find_first_not_of(split, start);
	return str.substr(start);
}
string firstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	int end     = str.find_first_of(split, start);
	return str.substr(start, end-start);
}
/**
 * @brief  取字符串的第一个单词
 * @author tgmerge
 * @param  string& 要处理的串
 *         string  分隔符
 * @return string  第一个单词
 */
string Interpreter::firstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	int end     = str.find_first_of(split, start);
	return str.substr(start, end-start);
}

/**
 * @brief  删除字符串的第一个单词
 * @author tgmerge
 * @param  string& 要处理的串
 *         string  分隔符
 * @return string  剩下的字符串
 */
string Interpreter::delFirstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	    start   = str.find_first_of(split, start);
		start   = str.find_first_not_of(split, start);
	return str.substr(start);
}

Interpreter::Interpreter() {
}

/**
 * @brief  从用户读取输入，删除多余空格
 * @author tgmerge
 * @return 整理后的字符串
 */
string Interpreter::readInput() {
	string sql("");
	string temp("");
	bool fin = false;

	while( !fin ) {
		cin >> temp;
		sql += temp + " ";
		if( sql.at(sql.length()-2) == ';' ) {
			sql = sql.substr(0, sql.length()-2);
			sql += " ;";
			fin = true;
		}
	}

	return sql;
}

/**
 * @brief  解析命令，返回一个SqlCommand对象供API使用
 * @author tgmerge
 * @param  string     要解析的字符串
 * @return SqlCommand 解析完成的命令对象
 */
SqlCommand Interpreter::getExpression(string input) {
	// 要返回的内部命令对象
	SqlCommand command;

	return command;
}

SqlCommand Interpreter::dropDatabase(string& str){
	SqlCommand sql;
	string name;
	sql.setType(7);
	str = ::delFirstWord(str, " ");
	str = ::delFirstWord(str, " ");
	name = ::firstWord(str, ";");
	//cout<<str;
	sql.setDatabaseName(name);
	return sql;
}

SqlCommand Interpreter::dropTable(string& str){
	SqlCommand sql;
	string name;
	sql.setType(8);
	str = ::delFirstWord(str, " ");
	str = ::delFirstWord(str, " ");
	name = ::firstWord(str, ";");
	sql.setTableName(name);
	return sql;
}