/**
 * @file interpreter.h
 * @brief 解释器模块头文件
 * @author tgmerge
 * 解释器。读取输入的SQL语句，检验输入正确性，转换输入语句为内部形式。
 * 对含有错误的语句，显示错误信息。
 */

#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

//#include "Minisql.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

/**
 * class SqlCommand
 * @brief 内部格式sql命令
 */

class SqlCommand {
public:

private:
	// 命令类型
	int type;

	// 命令数据
	string tableName;
	string databaseName;
	string indexName;
	string rowName;

	// 插入/删除数据列表的 各属性类型
	vector<int> attrListType;
	// 插入/删除数据列表的 各属性名
	vector<string> attrListName;
	// 插入/删除数据列表的 各属性值
	vector<string> attrListValue;
};

/**
 * classs Interpreter
 * @brief 解释器
 */
class Interpreter {

public:
	// 构造
	Interpreter();
	// 读取输入
	string readInput();
	// 获取内部格式数据
	SqlCommand getExpression(string input);
	// 获取字符串的第一个单词（分隔符自定义）
	string firstWord(string& str, string split);
	// 删除字符串的第一个单词（分隔符自定义）
	string delFirstWord(string& str, string split);



private:
	// 获取字符串的第一个单词（分隔符自定义）
	string firstWord(string& str, string& split);
	// 删除字符串的第一个单词（分隔符自定义）
	string delFirstWord(string& str, string& split);

	// 检查CREATE
	SqlCommand createClause();
	// 检查CREATE DATABASE databaseName
	SqlCommand createDatabase();
	// 检查CREATE TABLE tableName(rowName type, ..., PRIMARY KEY(primaryRowName))
	SqlCommand createTable();
	// 检查CREATE INDEX indexName
	SqlCommand createIndex();
	// 检查CREATE INDEX indexName ON tableName(rowName)
	SqlCommand createIndexOn();
	
	// 检查DROP
	SqlCommand dropClause();
	// 检查DROP DATABASE databaseName
	SqlCommand dropDatabase();
	// 检查DROP TABLE tableName
	SqlCommand dropTable();
	// 检查DROP INDEX indexName
	SqlCommand dropIndex();

	// 检查SELECT rowName FROM tableName WHERE condRow condOp condValue
	SqlCommand selectClause();

	// 检查INSERT INTO tableName VALUES(insertValueList)
	SqlCommand insertClause();
	SqlCommand insertIntoValues();
	
	// 检查DELETE
	SqlCommand deleteClause();
	// 检查DELETE FROM tableName WHERE condRow condOp condValue
	SqlCommand deleteFromWhere();

	// 检查USE databaseName
	SqlCommand useClause();
	//
	SqlCommand execfileClause();

	// 检查QUIT
	SqlCommand quitClause();
};

#endif