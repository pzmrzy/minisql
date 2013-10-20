#ifndef _MINISQL_H_
#define _MINISQL_H_
#include "catalog.h"
#include "interpreter.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#define SQL_CREATE_DATABASE 10
#define SQL_CREATE_TABLE    11
#define SQL_CREATE_INDEX    12
#define SQL_CREATE_INDEX_ON 13
#define SQL_DROP_DATABASE   20
#define SQL_DROP_TABLE      21
#define SQL_DROP_INDEX      22
#define SQL_SELECT          30
#define SQL_INSERT_INTO     40
#define SQL_DELETE          50
#define SQL_USE             60
#define SQL_QUIT            70
#define SQL_ERROR           90
//catalog返回的信息
class catainfo{
public:
	bool succ;
	string message;
};

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
//属性信息
class attribute{
	string attr;	//名称
	int datatype;	//数据类型
	bool PK;		//主键
	bool UNIQUE;	//唯一
	bool NN;		//非空
}
#endif