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

#define _LEAFNODE 1
#define _NONLEAFNODE 0

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
	Value(int type):type(type){}
	Value(int type,string key):type(type),charKey(key){}
	Value(int type,int key):type(type),intKey(key){}
	Value(int type,float key):type(type),floatKey(key){}
	//得到key，统一以字符串返回
	string getKey();
	int getType(){return type;}
	int getIntKey(){return intKey;}
	float getFloatKey(){return floatKey;}
	string getCharKey(){return charKey;}
	void setKey(int key){intKey = key;}
	void setKey(string key){charKey = key;}
	void setKey(float key){floatKey = key;}
};
/*
class  KeyAndPtr
{
private:
	int type;
	Value value;
	PtrType ptr;

public:
	KeyAndPtr(int type,Value value,PtrType ptr):type(type),value(value),ptr(ptr){}
};
*/
class Node
{
private:
	vector<Value> info;
	//当前指向的位置，还未返回
	int now;
	string indexName;
	//当前节点所拥有的指针数
	int count;
	//节点类型，0是非叶节点，1是叶节点
	int nodeType;
	//节点应拥有的指针数
	int ptrN;
	//indexName 所对应的key类型
	int attrType;
	//块号即偏移量
	int offset;
	
	BufferManager indexBuff;
	Block block;
	table tableInstance;
	TYPE_SIZE;

	//把块的内容读出来
	string read();

public:
	//创建一个节点时调用的构造函数
	Node(BufferManager indexBuff,string indexName,table tableInstance,int n):indexBuff(indexBuff){}
	//得到一个节点时调用的构造函数
	Node(BufferManager indexBuff,PtrType ptr,string indexName,table tableInstance,int n);
	//析构函数：讲块的内容写入磁盘，再销毁
	~Node();

	//得到节点的偏移量
	PtrType getNodePtr();

	void set(vector<Value> temp){info.clear();info = temp;updateCount();}
	void updateCount();//更新该节点当前指针数
	void setLastPtr(Value ptr);
	void setLastPtr(PtrType ptr);
	Value getLastPtr(){return info[info.size() - 1];}
	vector<Value> getInfo(){return info;}
	int getCount(){return count;} 
	int getNodeType(){return nodeType;}

};


class BPTree{
public:
	//索引树的buff管理器与键值类型
	BPTree(BufferManager indexBuff,int type);
	bool createBPTree(SqlCommand sql,table tableInstance,string indexName);
	bool loadBPTree(string indexName);//TODO:改成构造函数
	void insert(Value key,PtrType pointer);

private:
	BufferManager indexBuff;//怎么初始化它？
	int n;//节点指针数
	int type;//键值类型
	string indexName;
	table tableInstance;
	PtrType root;
	class ParentMap
	{
	public:
		PtrType nodePtr;
		PtrType parentPtr;
	};
	vector<ParentMap> parentMap;//父亲列表

	
	void insertLeaf(Node node,Value key,PtrType pointer);
	void insertNonleaf(Node node,Value key,PtrType pointer);
	//没找到返回-1，找到返回偏移量
	PtrType find(Value key);
	PtrType findLeafNode(Value key);
	PtrType findParentNode(PtrType ptr);//利用parentMap查找ptr节点的父节点
	//求类型大小
	TYPE_SIZE;
};
//TODO： 比较函数
#endif