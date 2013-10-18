/**
 * @file  interpreter.cpp
 * @brief 解释器的实现
 *
   大概流程
   cin:    "drop table    tableA;"
   |  readInput();  单词等格式整理
   v
   string: "drop table tableA ;"
   |  getExpression();  获取SqlCommand，中间会调用dropTable();
   v
   SqlCommand 包含各种信息，发给API
   
 */
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

SqlCommand Interpreter::dropDatabase(string& str) {
	SqlCommand sql;
	string name;
	sql.setType(SQL_DROP_DATABASE);
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");
	name = firstWord(str, " ;");
	sql.setDatabaseName(name);
	return sql;
}

SqlCommand Interpreter::dropTable(string& str) {
	SqlCommand sql;
	string name;
	sql.setType(SQL_DROP_TABLE);
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");
	name = firstWord(str, " ;"); // ←readInput()那里应该已经处理过";"让它变成" ;"了 by tgmerge
	sql.setTableName(name);
	return sql;
}

/**
 * @brief  检验drop index语句合法性
 * @author tgmerge
 * @param  string&    SQL指令
 * @return SqlCommand 类型为SQL_DROP_INDEX的内部命令
 */
SqlCommand Interpreter::dropIndex(string& str)

/**
 * @brief  检验use语句合法性
 * @author tgmerge
 * @param  string&    SQL指令
 * @return SqlCommand 类型为SQL_USE的内部命令
 */
SqlCommand Interpreter::useClause(string& str){
	// 现在的str示例  use database1 ;
	SqlCommand sql;
	string name;

	//删除"use"
	str = delFirstWord(str, " ");
	
	//取数据库名称
	name = firstWord(str, " ");

	// TODO: 检验数据库名称存在性
	sql.setType(SQL_USE);
	sql.setDatabaseName(name);

	return sql;
}

SqlCommand Interpreter::

/**
 * @brief  解析命令，调用各检测方法，返回一个SqlCommand对象供API使用
 * @author tgmerge
 * @param  string     要解析的字符串
 * @return SqlCommand 解析完成的命令对象
 */
SqlCommand Interpreter::getExpression(string input) {

	// 要返回的对象
	SqlCommand sql;
	
	// 取出的第一个单词
	string firstStr = firstWord(input, " ");
	
	// 根据第一个单词判断
	// 第一个是use
	if( firstStr == "use" ) {
		sql = useClause(input);
	}
	// 第一个是drop
	else if( firstStr == "drop" ) {
		// 根据第二个单词判断
		string secondStr = delFirstWord(input, " ");
		       secondStr = firstWord(secondStr, " ");
		if( secondStr == "database" ) {
			sql = dropDatabase(input);
		}
		else if( secondStr == "table" ) {
			sql = dropTable(input);
		}
		// 无法匹配
		else {
			sql.setType(SQL_ERROR);
		}
	}
	else {
		sql.setType(SQL_ERROR);
	}

	return sql;
}

