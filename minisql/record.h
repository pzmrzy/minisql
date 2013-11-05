#ifndef _RECORD_H_
#define _RECORD_H_

#include "minisql.h"
#include "sqlcommand.h"
#include "BufferManager.h"
#include "block.h"
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <strstream>
#include <sstream>
#include "minisql.h"
using namespace std;

#define BLOCK_LEN        4096
#define Used 0x01
#define Unused 0x00
///////////////////////////////////////////////////
class record
{
public:
	record(void){}
	virtual ~record(void){}
	//查找操作
	recoinfo Select_Rec(SqlCommand& sql,table &Table);
	//删除操作
	recoinfo Delete_Rec(SqlCommand& sql,table &Table);
	//插入操作
	recoinfo Insert_Rec(SqlCommand& sql,table &Table);
private:
    //解析数据中每条record
    Row parser(istrstream& inp,vector<attribute>& attrList);
    //按要求的列返回结果
    void push(Row& oneTuple,Results& results,vector<int> colNamePosVector);
    //判断是否满足约束条件
    bool checkConstraints(Row& oneTuple,vector<attribute>& attrList,vector<string>& condLeftVector,vector<string>& condOpVector,vector<string>& condRightVector);
	recoinfo writeblock(Block& blocks,int j,int tupleLen,vector<attribute>& attrList,vector<string>& colValueVector);//写入一条记录
	Row getOneTuple(Block& blocks,int j,int tupleLen,vector<attribute>& attrList);
};

#endif
