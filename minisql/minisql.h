#ifndef _MINISQL_H_
#define _MINISQL_H_

#include <string>
#include <iostream>
#include <vector>

using namespace std;

//catalog返回的信息
class catainfo{
public:
	bool succ;
	string message;
};

//属性信息
class attribute{
	string attr;	//名称
	int datatype;	//数据类型
	bool PK;		//主键
	bool UNIQUE;	//唯一
	bool NN;		//非空
};

#endif