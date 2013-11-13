#ifndef _MINISQL_H_
#define _MINISQL_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include"sqlcommand.h"

using namespace std;

#define MAX_FILENAME_LENGTH 256     /* 最长文件名长度 */
#define MAX_CMD_LENGTH      256     /* 最长命令长度 */
#define MAX_ATTRNAME_LENGTH 256     /* 最长字段长度 */
#define MAX_RECORD_LENGTH   256     /* 最长记录长度 */
#define MAX_ATTR_NUM        32      /* 一个表的最长字段数目 */
#define MAX_CHAR_LENGTH 32
#define FILE_PAGESIZE 4096          /* 虚拟页大小 */
#define MEM_MAXPAGE 1000            /* 内存页最大数目 */
#define FILENAME_MAXLEN 256         /* 文件名最大长度 */
#define MAX_CHAR_LENGTH     32      /* 最大 CHAR 类型字段长度 */
#define IDXHEAD_SIZE_IN_FILE (ATTR_SIZE_IN_FILE + sizeof(int))/* 一个索引头在文件中所占的大小 */
#define IDXNODE_SIZE_IN_FILE (sizeof(unsigned) + MAX_CHAR_LENGTH * sizeof(char)) /* 一个索引节点在文件中所占的大小 */
#define ATTR_SIZE_IN_FILE (sizeof(bool)*4 + sizeof(int)*2 + MAX_CHAR_LENGTH * sizeof(char))/* 一个字段在文件中所占的大小 */
#define TABLEHEAD_SIZE_IN_FILE (sizeof(int))/* 一个表信息头在文件中所占的大小 */
#define TABLENODE_SIZE_IN_FILE (2 * sizeof(int) + MAX_CHAR_LENGTH * sizeof(char)*2 + MAX_ATTR_NUM * ATTR_SIZE_IN_FILE)/* 一个表
信息项在文件中所占的大小 */
#define _TYPE_STRING 1
#define _TYPE_INT 0
#define _TYPE_FLOAT -1
//求类型大小
#define TYPE_SIZE \
int typeSize(int type)\
{\
	switch(type) \
	{\
	case _TYPE_FLOAT:return sizeof(float);break;\
	case _TYPE_INT:return sizeof(int);break;\
	case _TYPE_STRING:return sizeof(char[255]);break;\
	}\
}//TODO:是255么？

//catalog返回的信息
//属性信息
class attribute{
public:
	attribute():PK(false), UN(false), NN(false), ID(false){
	}
	string name;	//名称
	int datatype;	//数据类型
	int length;		//数据长度
	bool PK;		//主键
	bool UN;		//唯一
	bool NN;		//非空
	bool ID;		//是否有索引
	string typeName(){
		if (datatype == 0)
			return "Integer";
		if (datatype == -1)
			return "Float";
		stringstream ss;
		ss<<datatype;
		return "Char(" + ss.str() + ")";
	}
};
class table{
public:
	table (const table& T){
		name = T.name;
		dbname = T.dbname;
		attrNum = T.attrNum;
		recLength = T.recLength;
		attrList = T.attrList;
		recNum = T.recNum;
	}
	table (){}
	string name;					//名称
	string dbname;					//属于哪个数据库
	int attrNum;					//属性数
	int recLength;					//记录长度
	int recNum;						//记录条数
	int size;						//表长度
	vector<attribute> attrList;		//属性列表
};
class catainfo{
public:
	//Ctor
	catainfo(bool f, string mes, table tab){
		succ = f;
		message = mes;
		T = tab;
	}
	catainfo(){}
	//获取指令是否成功
	bool getsucc(){
		return succ;	
	}
	//输出错误信息
	void print(){
		cout<<message<<endl;
	}
	table gettable(){
		return T;
	}
private:
	bool succ;
	string message;
	table T;

};





class tableNum{
public:
	tableNum(){}
	tableNum(int n){
		num = n;
	}
	int num;
};
//record返回信息
//结果信息，由行列组成，每一行由多列vector<string> col组成
class Row
{
public:
	vector<string> col;//clolumns
};
class Results
{
public:
	vector<Row> row;//rows
};
//record返回的信息
class recoinfo
{
public:
	recoinfo(bool f, string mes,Results res,long num ){
		succ = f;
		message = mes;
		results=res;
		number=num;
	}
	recoinfo(){};
	//is execution succeed
	bool getsucc(){
		return succ;
	}
	//get the error message
	string errormes(){
		return message;
	}
	//get the reusult
	Results resultsmes(){
		return results;

	}
	//gei the number info of records
	long nummes(){
		return number;
	}
	void print (){
		cout<<message<<endl;
	}
private:
	bool succ;
	string message;
	Results results;
	long number;
};
class indexInfo{
	vector<int> result;
};
#endif