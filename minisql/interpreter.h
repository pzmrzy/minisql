/**
 * @file interpreter.h
 * @brief 解释器模块
 * @author tgmerge
 * 解释器。读取输入的SQL语句，检验输入正确性，转换输入语句为内部形式。
 * 对含有错误的语句，显示错误信息。
 */

#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#include "Minisql.h"
#include <string>

using std::string;

/**
 * classs Interpreter
 * @brief 解释器
 */
class Interpreter {

public:
	// 构造
	Interpreter();
	// 读取输入
	read_input(string& input);

private:
	// 检查CREATE
	CString create_clause();
	// 检查CREATE DATABASE databaseName
	CString create_database();
	// 检查CREATE TABLE tableName(rowName type, ..., PRIMARY KEY(primaryRowName))
	CString create_table();
	// 检查CREATE INDEX indexName
	CString create_index();
	// 检查CREATE INDEX indexName ON tableName(rowName)
	CString create_index_on();
	
	// 检查DROP
	CString drop_clause();
	// 检查DROP DATABASE databaseName
	CString drop_database();
	// 检查DROP TABLE tableName
	CString drop_table();
	// 检查DROP INDEX indexName
	CString drop_index();

	// 检查SELECT rowName FROM tableName WHERE condRow condOp condValue
	CString select_clause();

	// 检查INSERT INTO tableName VALUES(insertValueList)
	CString insert_clause();
	CString insert_into_values();
	
	// 检查DELETE
	CString delete_clause();
	// 检查DELETE FROM tableName WHERE condRow condOp condValue
	CString delete_from_where();


	CString use_clause();
	CString execfile_clause();
	CString quit_clause();

	CString get_expression();
	CString get_each();

	CString get_attribute();
	CString get_part();
}

/**
 * class SqlCommand
 * @brief 内部格式sql命令
 */
class SqlCommand {
public:
	
private:
	// 命令类型
	enum type;
	string tableName;
	string databaseName;
	string indexName;
	string rowName;
	// SELECT条件使用
	enum condition;
	string 

#endif