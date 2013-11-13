#include"indexManager.h"

IndexManager::IndexManager(string DBName):buff(DBName)
{
}

//create table时调用
bool IndexManager::createIndex(string indexName)
{
	buff.storeIndex(indexName,"");
}





//create index on时调用
bool IndexManager::createIndexOn(SqlCommand sql,table tableInstance,string indexName)
{
	if (tableInstance.recNum == 0)
	{
		this->createIndex(indexName);
		return;
	}
	BPTree tree;
	tree.createBPTree(sql,tableInstance,BufferManager buff);
}





//drop index时调用
bool IndexManager::dropIndex(SqlCommand sql,table tableInstance)
{
}





//drop table时调用
bool IndexManager::dropIndexAll(SqlCommand sql,table tableInstance)
{
}





//select rec时调用
indexInfo IndexManager::selectRec(SqlCommand sql,table tableInstance,vector<string> indexList)
{
}




//insert rec时调用
indexInfo IndexManager::insertRec(SqlCommand sql,table tableInstance,vector<string> indexList)
{
}




//delete rec时调用
indexInfo IndexManager::deleteRec(SqlCommand sql,table tableInstance,vector<string> indexList)
{
}