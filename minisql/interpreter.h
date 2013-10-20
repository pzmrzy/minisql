/**
 * @file interpreter.h
 * @brief 解释器模块头文件
 * @author tgmerge
 * 解释器。读取输入的SQL语句，检验输入正确性，转换输入语句为内部形式。
 * 对含有错误的语句，显示错误信息。
 */

#if !defined(_INTERPRETER_H_)
#define _INTERPRETER_H_

#define SQL_CREATE_DATABASE 10
#define SQL_CREATE_TABLE    11
#define SQL_CREATE_INDEX    12
#define SQL_DROP_DATABASE   20
#define SQL_DROP_TABLE      21
#define SQL_DROP_INDEX      22
#define SQL_SELECT          30
#define SQL_INSERT_INTO     40
#define SQL_DELETE          50
#define SQL_USE             60
#define SQL_QUIT            70
#define SQL_ERROR           90

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

	vector<int> attrListType;       // 插入/删除数据列表的 各属性类型
	vector<string> attrListName;    // 插入/删除数据列表的 各属性名
	vector<string> attrListValue;  	// 插入/删除数据列表的 各属性值
};

/**
 * classs Interpreter
 * @brief 解释器
 */
class Interpreter {

public:

public: // 命令处理
	string readInput();                     // 读取输入
	SqlCommand getExpression(string input); // 获取内部格式数据

public: // 字符串处理
	string firstWord(string& str, string split);    // 获取字符串的第一个单词（分隔符自定义）
	string delFirstWord(string& str, string split); // 删除字符串的第一个单词（分隔符自定义）

public: // 检查语句
	SqlCommand createDatabase(string& str);// 检查CREATE DATABASE databaseName
	SqlCommand createTable(string& str);   // 检查CREATE TABLE tableName(rowName type, ..., PRIMARY KEY(primaryRowName))
	SqlCommand createIndex(string& str);   // 检查CREATE INDEX indexName ON tableName(rowName)

	SqlCommand dropDatabase(string& str);  // 检查DROP DATABASE databaseName
	SqlCommand dropTable(string& str);     // 检查DROP TABLE tableName
	SqlCommand dropIndex(string& str);     // 检查DROP INDEX indexName

	SqlCommand selectClause();             // 检查SELECT rowName FROM tableName WHERE condRow condOp condValue

	SqlCommand insertIntoValues();         // 检查INSERT INTO tableName VALUES(insertValueList)

	SqlCommand deleteFromWhere();          // 检查DELETE FROM tableName WHERE condRow condOp condValue

	SqlCommand useClause(string& str);     // 检查USE databaseName

	SqlCommand quitClause();               // 检查QUIT
};

#endif