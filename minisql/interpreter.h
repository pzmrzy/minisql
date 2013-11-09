/**
 * @file interpreter.h
 * @brief 解释器模块头文件
 * @author tgmerge
 * 解释器。读取输入的SQL语句，检验输入正确性，转换输入语句为内部形式。
 * 对含有错误的语句，显示错误信息。
 */

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include "sqlcommand.h"
#include <string>
#include <iostream>
#include <vector>

using namespace std;

/**
 * classs Interpreter
 * @brief 解释器
 */
class Interpreter {

public: // 命令处理
	string readInput();                     // 读取输入
	SqlCommand getExpression(string input); // 获取内部格式数据

public: // 字符串处理
	string firstWord(string& str, string split);    // 获取字符串的第一个单词（分隔符自定义）
	string delFirstWord(string& str, string split); // 删除字符串的第一个单词（分隔符自定义）
	string preProcess(string& str); // 预处理，去除多余空格等
	string& Interpreter::replaceAll(string& str,const string& old_value,const string& new_value); // 字符串替换

public: // 检查语句
	SqlCommand createDatabase(string& str);  // 检查CREATE DATABASE databaseName
	SqlCommand showDatabase(string& str);
	// TODO: createTable未完成
	SqlCommand createTable(string& str);     // 检查CREATE TABLE tableName(colName type, ..., PRIMARY KEY(primarycolName))
	SqlCommand createIndex(string& str);     // 检查CREATE INDEX indexName ON tableName(colName)
										     
	SqlCommand dropDatabase(string& str);    // 检查DROP DATABASE databaseName
	SqlCommand dropTable(string& str);       // 检查DROP TABLE tableName
	SqlCommand dropIndex(string& str);       // 检查DROP INDEX indexName
										     
	SqlCommand selectClause(string& str);    // 检查SELECT colName FROM tableName WHERE condcol condOp condValue

	SqlCommand insertIntoValues(string& str);// 检查INSERT INTO tableName VALUES(insertValueList)

	SqlCommand deleteFromWhere(string& str); // 检查DELETE FROM tableName WHERE condcol condOp condValue

	SqlCommand useClause(string& str);       // 检查USE databaseName

	SqlCommand quitClause();                 // 检查QUIT
};

#endif