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

extern BufferManager *bfm;
typedef int PtrType;

class Value//存放值的类
{
private:
	int type;//存储对应的值
	string charKey;
	int intKey;
	float floatKey;

public:
	//用值类型和key初始化这个类
	//_TYPE_STRING 0; _TYPE_INT 1; _TYPE_FLOAT 2;
	Value(int type):type(type){}
	Value(int type,string key):type(type),charKey(key){}
	Value(int type,int key):type(type),intKey(key){}
	Value(int type,float key):type(type),floatKey(key){}
	
	//得到key，统一以字符串返回
	string getKey();
	
	//获得值类型
	int getType(){return type;}
	
	//获得对应的类型的值
	int getIntKey(){return intKey;}
	float getFloatKey(){return floatKey;}
	string getCharKey(){return charKey;}
	
	//设置值
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
class Node//B+树节点类
{
private:
	//B+树的节点内容（除去附加的节点头）
	vector<Value> info;

	//索引名称
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

	//数据库名称
	string dbName;

	//块
	Block block;

	//表信息
	table tableInstance;

	//获得类型大小的函数
	TYPE_SIZE;

	//把块的内容读出来
	void read();

public:
	//创建一个节点时调用的构造函数
	Node(string dbName, string indexName, table tableInstance, int n);

	//得到一个节点时调用的构造函数
	Node(string dbName,PtrType ptr,string indexName,table tableInstance,int n);

	Node(string dbName,PtrType ptr,string indexName,table tableInstance,int n,int tag);//无read构造函数
	//析构函数：讲块的内容写入磁盘，再销毁
	~Node();

	//得到节点的偏移量
	PtrType getNodePtr();

	//设置B+树的节点内容（除去附加的节点头）
	void set(vector<Value> temp){info.clear();info = temp;updateCount();}

	//更新该节点当前指针数
	void updateCount();

	//设置最后的指针地址
	void setLastPtr(Value ptr);
	void setLastPtr(PtrType ptr);

	//设置节点的键值类型
	void setType(int type){nodeType = type;}

	//设置节点应拥有的指针数
	void setCount(int t){count = t;}

	//得到最后的指针
	Value getLastPtr(){return info[info.size() - 1];}

	//得到节点信息（除去表头）
	vector<Value> getInfo(){return info;}

	//得到节点应拥有的指针数
	int getCount(){return count;} 

	//得到节点类型
	int getNodeType(){return nodeType;}
};


class BPTree{
public:
	//用索引树的buff管理器与键值类型构造
	BPTree(string dbName,int type);

	//创建一个空的B+树
	void createBPTree(SqlCommand sql,table tableInstance,string indexName,int type);

	//读取B+树
	void loadBPTree(string indexName);//TODO:改成构造函数

	//插入节点
	void insert(Value key,PtrType pointer);

	//在B+树中找到key值的位置，并返回与key值绑定的偏移量（即该记录在总文件中的偏移量）
	PtrType find(Value key);

	//删除key值对应的键值、地址对
	PtrType deleteNode(Value key);

	//从开始点直到最后去取值，用于大于等于
	vector<int> findToBehind(Value key);

	//从开始点直到不满足条件去取值，用于小于等于
	vector<int> findToBehindIF(Value key);

private:
	//节点指针数
	int n;

	//键值类型
	int type;

	//索引名称
	string indexName;

	//表信息
	table tableInstance;

	//根节点
	PtrType root;

	//数据库名称
	string dbName;

	//当前节点与父亲节点的映射类
	class ParentMap
	{
	public:
		//当前节点
		PtrType nodePtr;
		//对应的父亲节点
		PtrType parentPtr;
	};

	//父亲列表
	vector<ParentMap> parentMap;

	//Value类的比较函数 if key1 < key2 then true
	bool isLess(Value& key1,Value& key2);

	//Value类的比较函数 if key1 <= key2 then true
	bool isLessEqual(Value& key1,Value& key2);

	//Value类的比较函数 if key1 == key2 then true
	bool isEqual(Value& key1,Value& key2);
	
	//插入节点到叶节点
	void insertLeaf(Node node,Value key,PtrType pointer);

	//插入节点到非叶节点（传的node是要更新的父节点的子节点）
	void insertNonleaf(Node node,Value key,PtrType pointer);
	
	//查找叶子节点，没找到返回-1，找到返回叶子节点的偏移量（是叶子节点整个节点的）
	PtrType findLeafNode(Value key);

	//利用parentMap查找ptr节点的父节点
	PtrType findParentNode(PtrType ptr);

	//求类型大小
	TYPE_SIZE;
};

#endif