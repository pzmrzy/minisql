#ifndef _BPTREE_H_
#define _BPTREE_H_
/**
 * @file   BPTree.h
 * @brief   B+树的实现
 * @author co8bit
 */

#include "minisql.h"
#include"buffermanager.h"
#include"stdio.h";
typedef int PtrType;

class Value
{
private:
	int type;
	string charKey;
	int intKey;
	float floatKey;
public:
	//用类型和key初始化这个类
	//_TYPE_STRING 0; _TYPE_INT 1; _TYPE_FLOAT 2;
	Value(int type,string key):type(type),charKey(key){}
	Value(int type,int key):type(type),intKey(key){}
	Value(int type,float key):type(type),floatKey(key){}
	//得到key，统一以字符串返回
	string getKey();
};

class Node
{
private:
	vector<string> nodeText;
	//当前指向的位置，还未返回
	int now;
public:
	//得到key
	int getIntKey();
	string getCharKey();
	float getFloatKey();
};

class LeafNode: public Node
{
	int getPtr();
	int getLastPtr();
};

class NonleafNode: public Node
{
	int getPtr();
	int getFirstPtr();
};

class BPTree{
public:
	//索引树的buff管理器与键值类型
	BPTree(BufferManager indexBuff,int type);
	bool createBPTree(SqlCommand sql,table tableInstance,string indexName);
	bool loadBPTree(BufferManager buff,string indexName);//TODO:改成构造函数

private:
	BufferManager indexBuff;//怎么初始化它？
	int n;//节点指针数
	int type;//键值类型

	void insert(Value key,PtrType pointer);
	void insertLeaf(LeafNode node,Value key,PtrType pointer);
	void insertNonleaf(NonleafNode node,Value key,PtrType pointer);
	PtrType find(Value key);
};

#endif