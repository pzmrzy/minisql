#ifndef _MINISQL_H_
#define _MINISQL_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
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
#define ATTR_SIZE_IN_FILE (sizeof(bool) + sizeof(int) + sizeof(int) + MAX_CHAR_LENGTH * sizeof(char))/* 一个字段在文件中所占的大小 */
#define TABLEHEAD_SIZE_IN_FILE (sizeof(int))/* 一个表信息头在文件中所占的大小 */
#define TABLENODE_SIZE_IN_FILE (2 * sizeof(int) + MAX_CHAR_LENGTH * sizeof(char) + MAX_ATTR_NUM * ATTR_SIZE_IN_FILE)/* 一个表信息项在文件中所占的大小 */
//catalog返回的信息
//属性信息
class attribute{
public:
	string name;	//名称
	int datatype;	//数据类型
	int length;		//数据长度
	bool PK;		//主键
	bool UN;		//唯一
	bool NN;		//非空
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
		attrNum = T.attrNum;
		recLength = T.recLength;
		attrList = T.attrList;
	}
	table (){}
	string name;					//名称
	int attrNum;					//属性数
	int recLength;					//记录长度
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
	//获取指令是否成功
	bool getsucc(){
		return succ;	
	}
	//输出错误信息
	void print(){
		cout<<message<<endl;
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
#endif