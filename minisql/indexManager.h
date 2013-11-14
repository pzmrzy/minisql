#ifndef _INDEXMANAGER_H_
#define _INDEXMANAGER_H_
/**
 * @file   indexManager.h
 * @brief  索引管理
 * @author co8bit
 */

#include "minisql.h"
#include"BPTree.h"

class IndexManager{
private:
	BufferManager buff;
	string dbName;
public:
	//用数据库名称初始化
	IndexManager(string DBName);

	//create table时调用
	void createIndex(string indexName);

	//create index on时调用，创建一棵空树，然后等待上层调用插入操作，创建这个属性上的索引
	void createIndexOn(SqlCommand sql,table tableInstance,string indexName);

	//drop index时调用
	bool dropIndex(SqlCommand sql,table tableInstance);

	//drop table时调用
	bool dropIndexAll(SqlCommand sql,table tableInstance);

	//select rec时调用
	vector<int> selectOne(SqlCommand sql,table tableInstance,string indexName,string key);//选择一个的操作
	vector<int> selectMany_x(SqlCommand sql,table tableInstance,string indexName,string key);//选择多个的操作（小于）
	vector<int> selectMany_d(SqlCommand sql,table tableInstance,string indexName,string key);//选择多个的操作（大于）
	vector<int> selectMany_xd(SqlCommand sql,table tableInstance,string indexName,string key);//选择多个的操作（小于等于）
	vector<int> selectMany_dd(SqlCommand sql,table tableInstance,string indexName,string key);//选择多个的操作（大于等于）

	//给相应索引树插入记录索引，insert rec时调用
	void insertRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key, int blockPtr, int inBlockPtr);

	//将相应的索引树删除记录索引，delete rec时调用
	vector<int> deleteRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key);
};

#endif