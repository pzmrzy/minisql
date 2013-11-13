/**
 * @file  sqlcommand.h
 * @brief 内部命令类头文件
 * TODO: 决定内部命令的细节
 */

#ifndef _SQLCOMMAND_H_
#define _SQLCOMMAND_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

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
#define SQL_SHOW_DATABASE   100
#define SQL_EXECFILE		110

#define SQL_TYPE_INT		0
#define SQL_TYPE_FLOAT		-1

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
	//获取数据库名
	string getDatabaseName(){
		return databaseName;
	}
	//设置表名
	void setTableName(string& str){
		tableName = str;
	}
	//获取表名
	string getTableName(){
		return tableName;
	}
	//设置索引名
	void setIndexName(string& str){
		indexName = str;
	}
	//获取索引名
	string getIndexName(){
		return indexName;
	}
	//设置列名
	void setcolName(string& str){
		colName = str;
	}
	string getcolName(){
		return colName;
	}
	//push列名(select使用)
	void pushColNameVector(string& t){
		colNameVector.push_back(t);
	}
	vector<string> getcolNameVector(){
		return colNameVector;
	}
	//push列值(insert使用)
	void pushColValueVector(string& str){
		if(str.at(0)=='\'' && str.at(str.length()-1)=='\'')
			str = str.substr(1, str.length()-2);
		colValueVector.push_back(str);
	}
	vector<string> getcolValueVector(){
		return colValueVector;
	}
	//push条件左值(select,delete)
	void pushCondLeftVector(string& str){
		condLeftVector.push_back(str);
	}
	vector<string> getCondLeftVector(){
		return condLeftVector;
	}
	//push条件操作符(select,delete)
	void pushCondOpVector(string& str){
		condOpVector.push_back(str);
	}
	vector<string> getCondOpVector(){
		return condOpVector;
	}
	//push条件右值(select,delete)
	void pushCondRightVector(string& str){
		condRightVector.push_back(str);
	}
	vector<string> getCondRightVevtor(){
		return condRightVector;
	}
	int gettype(){
		return type;
	}
	//输出命令信息
	void print(){

		cout<<"type: "<<type<<endl;
		cout<<"tableName: "<<tableName<<endl;
		cout<<"databaseName: "<<databaseName<<endl;
		cout<<"indexName: "<<indexName<<endl;
		cout<<"colName: "<<colName<<endl;

		cout<<"colNameVector:  ";
		for (int i=0; i<colNameVector.size(); i++)
			cout<<colNameVector[i]<<" ";
		cout<<endl;
		cout<<"colValueVector:  ";
		for (int i=0; i<colValueVector.size(); i++)
			cout<<colValueVector[i]<<" ";
		cout<<endl;
		cout<<"condLeftVector: ";
		for (int i=0; i<condLeftVector.size(); i++)
			cout<<condLeftVector[i]<<" ";
		cout<<endl;
		cout<<"condOpVector: ";
		for (int i=0; i<condOpVector.size(); i++)
			cout<<condOpVector[i]<<" ";
		cout<<endl;
		cout<<"condRightVector: ";
		for (int i=0; i<condRightVector.size(); i++)
			cout<<condRightVector[i]<<" ";
		cout<<endl;
	}
	//写日志
	void writelog(){
		std::ofstream fout("db.log", ios::ate);
		time_t ltime;
		time(&ltime);
		fout << ctime(&ltime);
		if (databaseName!="")
			fout<<" "<<databaseName;
		else
			fout<<" -";
		if (tableName!="")
			fout<<" "<<tableName;
		else
			fout<<" -";
		if (databaseName!="")
			fout<<" "<<indexName;
		else
			fout<<" -";
		if (databaseName!="")
			fout<<" "<<colName;
		else
			fout<<" -";
		fout<<" "<<type;

		fout<<" "<<colNameVector.size();
		for (int i=0; i<colNameVector.size(); i++)
			fout<<" "<<colNameVector[i];

		fout<<" "<<colValueVector.size();
		for (int i=0; i<colValueVector.size(); i++)
			fout<<" "<<colValueVector[i];

		fout<<" "<<condLeftVector.size();
		for (int i=0; i<condLeftVector.size(); i++)
			fout<<" "<<condLeftVector[i];

		fout<<" "<<condOpVector.size();
		for (int i=0; i<condOpVector.size(); i++)
			fout<<" "<<condOpVector[i];

		fout<<" "<<condRightVector.size();
		for (int i=0; i<condRightVector.size(); i++)
			fout<<" "<<condRightVector[i];

		fout<<" "<<colSpecialVector.size();
		for (int i=0; i<colSpecialVector.size(); i++)
			fout<<" "<<colSpecialVector[i];

		fout<<" "<<colType.size();
		for (int i=0; i<colType.size(); i++)
			fout<<" "<<colType[i];
		fout<<endl;
	}
private:
	// 命令类型
	int type;

	// 命令数据
	string tableName;
	string databaseName;
	string indexName;
	string colName;
public:
	vector<string> colNameVector;	// where/insert/CREATE TABLE条件中的属性名
	vector<string> colValueVector;  // where/insert条件中的属性值
	vector<string> condLeftVector;	// where条件中的属性名 e.g. colA
	vector<string> condOpVector;	// where条件中的符号   e.g. >=
	vector<string> condRightVector;	// where条件中的值     e.g. 10

public:
	vector<string> colSpecialVector;// create table中各列是否为主键('primary')或unique('unique')
	vector<int> colType; // 0=整形, -1=浮点, 1-15为char长度
};

#endif