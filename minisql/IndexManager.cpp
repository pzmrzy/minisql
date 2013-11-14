#include"indexManager.h"

IndexManager::IndexManager(string DBName):buff(DBName),dbName(DBName)
{
}

//create table时调用
void IndexManager::createIndex(string indexName)
{
	//buff.storeIndex(indexName,"");??
}





//create index on时调用
void IndexManager::createIndexOn(SqlCommand sql,table tableInstance,string indexName)
{
	if (tableInstance.recNum == 0)
	{
		this->createIndex(indexName);
		return;
	}
	int attrType = 0;
	for(int i = 0; i < tableInstance.attrList.size(); i++)
		if (tableInstance.attrList[i].name == indexName)
		{
			switch(tableInstance.attrList[i].datatype)
			{
			case -1:attrType = _TYPE_FLOAT;break;
			case 0:attrType = _TYPE_INT;break;
			case 1:attrType = _TYPE_STRING;break;
			}
			break;
		}
	BPTree tree(dbName,attrType);
	tree.createBPTree(sql,tableInstance,sql.getTableName()+"."+indexName,attrType);
}




/*
//drop index时调用
void IndexManager::dropIndex(SqlCommand sql,table tableInstance)
{
}





//drop table时调用
bool IndexManager::dropIndexAll(SqlCommand sql,table tableInstance)
{
}

*/



//select rec时调用
vector<int> IndexManager::selectOne(SqlCommand sql,table tableInstance,string indexName,string key)
{
	vector<int> result;
	result.clear();
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexName)
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+ indexName);
		Value temp(attrType,key);
		result.push_back(tree.find(temp));
	
	return result;
}

vector<int> IndexManager::selectMany_d(SqlCommand sql,table tableInstance,string indexName,string key)
{
	vector<int> result;
	result.clear();
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexName)
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+ indexName);
		Value temp(attrType,key);
		return tree.findToBehind(temp);
}

vector<int> IndexManager::selectMany_x(SqlCommand sql,table tableInstance,string indexName,string key)
{
	vector<int> result;
	result.clear();
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexName)
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+ indexName);
		Value temp(attrType,key);
		return tree.findToBehindIF(temp);
}

vector<int> IndexManager::selectMany_dd(SqlCommand sql,table tableInstance,string indexName,string key)
{
	vector<int> result,tempRe;
	result.clear();
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexName)
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+ indexName);
		Value temp(attrType,key);
		result.push_back(tree.find(temp));
		tempRe = tree.findToBehind(temp);
		for (int i = 0; i < tempRe.size(); i++)
		{
			result.push_back(tempRe[i]);
		}
		return result;
}

vector<int> IndexManager::selectMany_xd(SqlCommand sql,table tableInstance,string indexName,string key)
{
	vector<int> result,tempRe;
	result.clear();
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexName)
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+ indexName);
		Value temp(attrType,key);
		tempRe = tree.findToBehindIF(temp);
		for (int i = 0; i < tempRe.size(); i++)
		{
			result.push_back(tempRe[i]);
		}
		result.push_back(tree.find(temp));
		return result;
}

//insert rec时调用
void IndexManager::insertRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key, int blockPtr, int inBlockPtr)
{
	for (int j = 0; j < indexList.size(); j++)//维护每一个索引
	{
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexList[j])
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+indexList[j]);
		Value temp(attrType,key);
		int recordCount = 4096 / (tableInstance.recLength + 1);//满载记录的块的条数
		tree.insert(temp,blockPtr*recordCount + inBlockPtr);
	}
}




//delete rec时调用
vector<int> IndexManager::deleteRec(SqlCommand sql, table tableInstance, vector<string> indexList, string key)
{
	vector<int> result;
	for (int j = 0; j < indexList.size(); j++)//维护每一个索引
	{
		int attrType = 0;
		for(int i = 0; i < tableInstance.attrList.size(); i++)
			if (tableInstance.attrList[i].name == indexList[j])
			{
				switch(tableInstance.attrList[i].datatype)
				{
				case -1:attrType = _TYPE_FLOAT;break;
				case 0:attrType = _TYPE_INT;break;
				case 1:attrType = _TYPE_STRING;break;
				}
				break;
			}
		BPTree tree(dbName,attrType);
		tree.loadBPTree(sql.getTableName()+"."+indexList[j]);
		Value temp(attrType,key);
		result.push_back(tree.deleteNode(temp));
	}
	return result;
}
