#ifndef _MINISQL_H_
#define _MINISQL_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

//catalog返回的信息
class catainfo{
public:
	//Ctor
	catainfo(bool f, string mes){
		succ = f;
		message = mes;
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

};

//属性信息
class attribute{
public:
	string name;	//名称
	int datatype;	//数据类型
	bool PK;		//主键
	bool UN;		//唯一
	bool NN;		//非空
};

class table{
public:
	string name;					//名称
	int attrNum;					//属性数
	int recLength;					//记录长度
	vector<attribute> attrList;		//属性列表
}

class tableNum{
public:
	tableNum(int n){
		num = n;
	}

	int getnum(){
		return num;
	}
private:
	int num;
}
#endif