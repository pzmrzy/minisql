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
	//create index on时调用
	bool createIndexOn(SqlCommand sql,table tableInstance,string indexName,int type);
	//drop index时调用
	bool dropIndex(SqlCommand sql,table tableInstance);
	//drop table时调用
	bool dropIndexAll(SqlCommand sql,table tableInstance);
	//select rec时调用
	vector<int> selectRec(SqlCommand sql,table tableInstance,vector<string> indexList,string key);
	//insert rec时调用
	void insertRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key, int blockPtr, int inBlockPtr);
	//delete rec时调用
	vector<int> deleteRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key);
};

#endif