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
	//设置命令类型
	void setType(int t){
		type = t;
	}
	//设置数据库名
	void setDatabaseName(string& str){
		databaseName = str;
	}
	//设置表名
	void setTableName(string& str){
		tableName = str;
	}
	//设置索引名
	void setIndexName(string& str){
		indexName = str;
	}
	//设置列名
	void setRowName(string& str){
		rowName = str;
	}

	//vector<int> attrListType;
	//vector<string> attrListName;
	//vector<string> attrListValue;
	//push记录类型
	void pushType(int t){
		attrListType.push_back(t);
	}
	//push记录名
	void pushName(string& str){
		attrListName.push_back(str);
	}
	//push记录值
	void pushValue(string& str){
		attrListValue.push_back(str);
	}
	//输出命令信息
	void print(){
		cout<<"type: "<<type<<endl;
		cout<<"tableName: "<<tableName<<endl;
		cout<<"databaseName: "<<databaseName<<endl;
		cout<<"indexName: "<<indexName<<endl;
		cout<<"rowName: "<<rowName<<endl;

		cout<<"attrListType:  ";
		for (int i=0; i<attrListType.size(); i++)
			cout<<attrListType[i]<<" ";
		cout<<endl;
		cout<<"attrListName:  ";
		for (int i=0; i<attrListName.size(); i++)
			cout<<attrListName[i]<<" ";
		cout<<endl;
		cout<<"attrListValue: ";
		for (int i=0; i<attrListValue.size(); i++)
			cout<<attrListValue[i]<<" ";
		cout<<endl;
	}
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



public:
	// 获取字符串的第一个单词（分隔符自定义）
	string firstWord(string& str, string& split);
	// 删除字符串的第一个单词（分隔符自定义）
	string delFirstWord(string& str, string& split);

	// 检查CREATE	type = 1
	SqlCommand createClause();
	// 检查CREATE DATABASE databaseName	type = 2
	SqlCommand createDatabase();
	// 检查CREATE TABLE tableName(rowName type, ..., PRIMARY KEY(primaryRowName))	type = 3
	SqlCommand createTable();
	// 检查CREATE INDEX indexName	type = 4
	SqlCommand createIndex();
	// 检查CREATE INDEX indexName ON tableName(rowName)	type = 5
	SqlCommand createIndexOn();
	
	// 检查DROP	type = 6
	SqlCommand dropClause();
	// 检查DROP DATABASE databaseName	type = 7
	SqlCommand dropDatabase(string& str);
	// 检查DROP TABLE tableName	type = 8
	SqlCommand dropTable(string& str);
	// 检查DROP INDEX indexName	type = 9
	SqlCommand dropIndex();

	// 检查SELECT rowName FROM tableName WHERE condRow condOp condValue	type = 10
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