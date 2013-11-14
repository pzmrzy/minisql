#include"BPTree.h"

Node::Node(string dbName,PtrType ptr,string indexName,table tableInstance,int n):indexName(indexName),tableInstance(tableInstance),ptrN(n),offset(ptr)
{
	block = bfm->getIndexOneBlock(indexName, ptr);
	Node::read();
}

Node::Node(string dbName,string indexName,table tableInstance,int n):indexName(indexName),tableInstance(tableInstance),ptrN(n),dbName(dbName)
{
	 
	//创建块，返回offset
	block = bfm->newIndexBlock(indexName);
	offset = block.offset;
}

Node::Node(string dbName,PtrType ptr,string indexName,table tableInstance,int n,int tag):indexName(indexName),tableInstance(tableInstance),ptrN(n),offset(ptr)
{
	 
	block = bfm->getIndexOneBlock(indexName, ptr);
}


PtrType Node::getNodePtr()
{
	return offset;
}

Node::~Node()
{
	char coutStream[5000];
	 
	
	char tempNodeType;
	if (nodeType == _LEAFNODE)
		tempNodeType = '1';
	else
		tempNodeType = '0';
	memcpy(coutStream,&tempNodeType,sizeof(char));
	memcpy(coutStream + 1,&count,sizeof(int));
	int j = 0;
	for (int i = 0; i < info.size(); i++)
	{
		switch (info[i].getType())
		{
		case _TYPE_INT:{int temp = info[i].getIntKey(); memcpy(coutStream + 1 + 4 + j, &temp, typeSize(_TYPE_INT)); j += typeSize(_TYPE_INT); break; }
		case _TYPE_FLOAT:{float temp = info[i].getFloatKey(); memcpy(coutStream + 1 + 4 + j, &temp, typeSize(_TYPE_FLOAT)); j += typeSize(_TYPE_FLOAT); break; }
		case _TYPE_STRING:{string temp = info[i].getCharKey(); memcpy(coutStream + 1 + 4 + j, &temp, typeSize(_TYPE_STRING)); j += typeSize(_TYPE_STRING); break;
		}//TODO:可以这样写吗？
		}
	}
	
	bfm->writeIndexData(indexName, offset,coutStream, j+5);
}

void Node::read()
{
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
	memcpy(&nodeType,block.content,1);
	memcpy(&count,block.content + 1,sizeof(int));
	for(int i = 0; i < (ptrN - 1); i++)
	{
		float tempFloat = 0;
		int tempInt = 0;
		string tempString = "";
		PtrType tempPtr = 0;
		Value tempValue(attrType);

		//read ptr//TODO:+5？
		memcpy(&tempPtr,block.content + 5 + i*( typeSize(attrType) + sizeof(int) ),typeSize(_TYPE_INT));
		Value ptrTempValue(_TYPE_INT,tempPtr);//TODO:设置ptr类型
		info.push_back(ptrTempValue);

		//read key
		switch(attrType)//TODO:+5？
		{
		case _TYPE_FLOAT:
			{
				memcpy(&tempFloat,block.content + 5 + i*( typeSize(attrType) + sizeof(int) ) + sizeof(int),typeSize(_TYPE_FLOAT));
				tempValue.setKey(tempFloat);
				break;
			}
		case _TYPE_INT:
			{
				memcpy(&tempInt,block.content + 5 + i*( typeSize(attrType) + sizeof(int) ) + sizeof(int),typeSize(_TYPE_INT));
				tempValue.setKey(tempInt);
				break;
			}
		case _TYPE_STRING:
			{
				memcpy(&tempString,block.content + 5 + i*( typeSize(attrType) + sizeof(int) ) + sizeof(int),typeSize(_TYPE_STRING));
				tempValue.setKey(tempString);
				break;
			}
		}//switch
		info.push_back(tempValue);
		
	}//for
	PtrType tempPtr = 0;
	memcpy(&tempPtr,block.content + 5 + (ptrN - 1)*( typeSize(attrType) + sizeof(int) ),typeSize(_TYPE_INT));
	Value ptrTempValue(_TYPE_INT,tempPtr);//TODO:设置ptr类型
	info.push_back(ptrTempValue);
}


void Node::setLastPtr(PtrType ptr)
{
	Value temp(_TYPE_INT,ptr);
	info[info.size() - 1] = temp;
}

void Node::setLastPtr(Value ptr)
{
	info[info.size() - 1] = ptr;
}

void Node::updateCount()
{
	//(info.size() - 1) / 2;
	count = 0;
	for (int i = 0; i < info.size(); i+=2)
	{
		count++;
	}
}










BPTree::BPTree(string dbName,int type):type(type),dbName(dbName)
{
	int freeSpace = 4*1024 - sizeof(PtrType) - 1 - sizeof(int);
	n  = freeSpace / (sizeof(PtrType) + typeSize(type)) + 1; //除下来应该取整而不是四舍五入
}

void BPTree::createBPTree(SqlCommand sql,table tableInstance,string indexName,int type)
{
	 
	int recordLength = tableInstance.recLength;

	Block block = bfm->newIndexBlock(indexName);
	vector<int> blockNum = bfm->getIndexBlocks(indexName);
	Node node(dbName,blockNum[0],indexName,tableInstance,n,1);
	Node newNode(dbName,indexName,tableInstance,n);

	node.setType(_NONLEAFNODE);
	node.setCount(1);
	vector<Value> temp;
	temp.clear();
	Value tempValue(_TYPE_INT,newNode.getNodePtr());
	temp.push_back(tempValue);
	node.set(temp);

	newNode.setType(_LEAFNODE);
	newNode.setCount(1);
	vector<Value> temp2;
	temp2.clear();
	Value tempValue2(_TYPE_INT,-1);
	temp2.push_back(tempValue2);
	if (type == _TYPE_INT)
	{
		Value tempValue2(type,-1);
		temp2.push_back(tempValue2);
	}
	else
	{
		if (type == _TYPE_FLOAT)
		{
			Value tempValue2(type,-1);
			temp2.push_back(tempValue2);
		}
		else
		{
			Value tempValue2(type,"*");
			temp2.push_back(tempValue2);
		}
	}
	newNode.set(temp2);
	newNode.setLastPtr(-1);
}

void BPTree::loadBPTree(string indexName)
{
	 
	int firstBlock = bfm->getIndexBlocks(indexName)[0];//获取开始的块
	Node rootNode(dbName,firstBlock,indexName,tableInstance,n);
	root = rootNode.getInfo()[0].getIntKey();
}

PtrType BPTree::find(Value key)
{
	 
	Node *node = new Node(dbName,root,indexName,tableInstance,n);
	//in nonLeafNode
	while (node->getNodeType() != _LEAFNODE)
	{
		vector<Value> temp = node->getInfo();
		int i = 1;
		for(i = 1; i < temp.size(); i+=2)
		{
			if (isLess(key,temp[i]))//只能是小于，因为小于才进前面
				break;
		}
		if (i >= temp.size())
		{
			delete node;
			Node *node = new Node(dbName,atoi(temp[temp.size() - 1].getKey().c_str()),indexName,tableInstance,n);
		}
		else
		{
			delete node;
			Node *node = new Node(dbName,atoi(temp[i - 1].getKey().c_str()),indexName,tableInstance,n);
		}
	}

	//in leafNode
	vector<Value> temp = node->getInfo();
	int i = 1;
	for(i = 1; i < temp.size(); i+=2)
	{
		if (isEqual(key,temp[i]))
			break;
	}
	if (i >= temp.size() )
		return -1;
	else
		return temp[i-1].getIntKey();
	delete node;
}

PtrType BPTree::findLeafNode(Value key)
{
	 
	Node *node = new Node(dbName,root,indexName,tableInstance,n);
	parentMap.clear();
	ParentMap *tempMap = new ParentMap();
	tempMap->nodePtr = root;
	tempMap->parentPtr = -1;
	parentMap.push_back(*tempMap);
	int j = 0;
	//in nonLeafNode
	while (node->getNodeType() != _LEAFNODE)
	{
		vector<Value> temp = node->getInfo();
		int i = 1;
		for(i = 1; i < temp.size(); i+=2)
		{
			if (isLess(key,temp[i]))//只能是小于，因为小于才进前面
				break;
		}
		if (i >= temp.size())
		{
			delete tempMap;
			ParentMap *tempMap = new ParentMap();
			tempMap->nodePtr =  atoi(temp[temp.size() - 1].getKey().c_str());
			tempMap->parentPtr = node->getNodePtr();
			parentMap.push_back(*tempMap);
			delete node;
			Node *node = new Node(dbName,atoi(temp[temp.size() - 1].getKey().c_str()),indexName,tableInstance,n);	
		}
		else
		{
			delete tempMap;
			ParentMap *tempMap = new ParentMap();
			tempMap->nodePtr =  atoi(temp[temp.size() - 1].getKey().c_str());
			tempMap->parentPtr = node->getNodePtr();
			parentMap.push_back(*tempMap);
			delete node;
			parentMap[j].nodePtr = atoi(temp[temp.size() - 1].getKey().c_str());
			Node *node = new Node(dbName,atoi(temp[i - 1].getKey().c_str()),indexName,tableInstance,n);
		}
	}
	return node->getNodePtr();
}

PtrType BPTree::findParentNode(PtrType ptr)
{
	int i;
	for (i = 0; i < parentMap.size(); i++)
		if (parentMap[i].nodePtr == ptr)
			return parentMap[i].parentPtr;
}

vector<int> BPTree::findToBehind(Value key)
{
	vector<int> result;
	Node *node = new Node(dbName,findLeafNode(key),indexName,tableInstance,n);
	while (true)
	{
		vector<Value> info = node->getInfo();
		for (int i = 0; i < info.size() - 1; i+=2)
		{
			if (isLess(key,info[i+1]))//大于
				result.push_back(info[i].getIntKey());
		}
		if (node->getLastPtr().getIntKey() != -1)
		{
			delete node;
			Node *node = new Node(dbName,node->getLastPtr().getIntKey(),indexName,tableInstance,n);
		}
		else
			break;
	}
	return result;
}

vector<int> BPTree::findToBehindIF(Value key)
{
	vector<int> result;
	int j = -1;
	Node *node = NULL;
	while (true)
	{
		do 
		{
			j++;
			node = new Node(dbName,bfm->getIndexBlocks(indexName)[j],indexName,tableInstance,n);
		}while(node->getNodeType() == _NONLEAFNODE);
		vector<Value> info = node->getInfo();
		for (int i = 0; i < info.size() - 1; i+=2)
		{
			if (isLess(info[i+1],key))
				result.push_back(info[i].getIntKey());
		}
		if (node->getLastPtr().getIntKey() != -1)
			delete node;
		else
			break;
	}
	return result;
}

void BPTree::insert(Value key,PtrType pointer)
{
	 
	PtrType nodePtr = findLeafNode(key);
	Node node(dbName,nodePtr,indexName,tableInstance,n);
	if (node.getCount() < (n - 1))
		insertLeaf(node,key,pointer);
	else
	{
		//排序
		vector<Value> keyList = node.getInfo();//只读键值对
		if (isLess(key,keyList[0]))
		{
			keyList.insert(keyList.begin(),key);
			Value temp(_TYPE_INT,pointer);
			keyList.insert(keyList.begin(),temp);
		}
		else
		{
			for (int i = (keyList.size() - 1 - 1); i >= 0; i-=2)
			{
				if (isLessEqual(keyList.at(i),key))
				{
					Value tempPtr(_TYPE_INT,pointer);
					keyList.insert(keyList.begin() + i+1,tempPtr);
					keyList.insert(keyList.begin() + i+2,key);
					break;
				}
			}
		}
		
		//更新尾指针
		Node newNode(dbName,indexName,tableInstance,n);
		PtrType newNodePtr = newNode.getNodePtr();
		newNode.setLastPtr(node.getLastPtr());
		node.setLastPtr(newNodePtr);

		//赋值元素到该到的地方
		int breakPoint = 0;
		if (n % 2 == 0)
			breakPoint = (n /2)*2;
		else
			breakPoint = ((n / 2) + 1)*2;
		vector<Value> temp(keyList.begin(),keyList.begin() + breakPoint);
		node.set(temp);//只写键值对
		vector<Value> temp2(keyList.begin() + breakPoint,keyList.end());//TODO:左闭右开?
		newNode.set(temp2);

		insertNonleaf(node,temp2[0],newNodePtr);
	}
}





void BPTree::insertLeaf(Node node,Value key,PtrType pointer)
{
	vector<Value> keyList = node.getInfo();//只读键值对
	if (isLess(key,keyList[0]))
	{
		keyList.insert(keyList.begin(),key);
		Value temp(_TYPE_INT,pointer);
		keyList.insert(keyList.begin(),temp);
	}
	else
	{
		for (int i = (keyList.size() - 1 - 1); i >= 0; i-=2)
		{
			if (isLessEqual(keyList.at(i),key))
			{
				Value tempPtr(_TYPE_INT,pointer);
				keyList.insert(keyList.begin() + i+1,tempPtr);
				keyList.insert(keyList.begin() + i+2,key);
				break;
			}
		}
	}
	node.set(keyList);
}





void BPTree::insertNonleaf(Node node,Value key,PtrType pointer)
{
	 
	if (node.getNodePtr() == root)
	{
		Node newNode(dbName,indexName,tableInstance,n);
		vector<Value> keyList;
		Value temp1(_TYPE_INT,node.getNodePtr());
		Value temp2(_TYPE_INT,pointer);
		keyList.push_back(temp1);
		keyList.push_back(key);
		keyList.push_back(temp2);
		newNode.set(keyList);
		root = newNode.getNodePtr();
		//把根节点的更改写入磁盘
		int firstBlock = bfm->getIndexBlocks(indexName)[0];//获取开始的块
		Node rootNode(dbName,firstBlock,indexName,tableInstance,n);
		Value temp3(_TYPE_INT,root);
		vector<Value> temp4;
		temp4.push_back(temp3);
		rootNode.set(temp4);
	}
	else
	{
		Node parentNode(dbName,findParentNode(node.getNodePtr()),indexName,tableInstance,n);
		if (parentNode.getCount() < n)
		{
			vector<Value> keyList = parentNode.getInfo();//只读键值对
			if (isLess(key,keyList[0]))
			{
				Value tempPtr(_TYPE_INT,pointer);
				keyList.insert(keyList.begin()+1,tempPtr);
				keyList.insert(keyList.begin()+1,key);
			}
			else
			{
				for (int i = (keyList.size() - 1 - 1); i >= 0; i-=2)
				{
					if (isLessEqual(keyList.at(i),key))
					{
						Value tempPtr(_TYPE_INT,pointer);
						keyList.insert(keyList.begin() + i+2,tempPtr);//TODO:最后一个会怎么样?
						keyList.insert(keyList.begin() + i+2,key);
						break;
					}
				}
			}
			parentNode.set(keyList);
		}
		else//分裂ParentNode
		{
			//排序
			vector<Value> keyList = parentNode.getInfo();//只读键值对
			if (isLess(key,keyList[0]))
			{
				Value tempPtr(_TYPE_INT,pointer);
				keyList.insert(keyList.begin()+1,tempPtr);
				keyList.insert(keyList.begin()+1,key);
			}
			else
			{
				for (int i = (keyList.size() - 1 - 1); i >= 0; i-=2)
				{
					if (isLessEqual(keyList.at(i),key))
					{
						Value tempPtr(_TYPE_INT,pointer);
						keyList.insert(keyList.begin() + i+2,tempPtr);//TODO:最后一个会怎么样?
						keyList.insert(keyList.begin() + i+2,key);
						break;
					}
				}
			}

			//赋值元素到该到的地方
			int breakPoint = 0;
			if (n % 2 == 0)
				breakPoint = (n /2);
			else
				breakPoint = ((n / 2) + 1);
			vector<Value> temp;
			temp.push_back(keyList[0]);
			int j = 1;
			for (int i = 1; i < breakPoint; i++)
			{
				temp.push_back(keyList[j]);
				temp.push_back(keyList[++j]);
			}
			parentNode.set(temp);

			Value newK = keyList[++j];
			vector<Value> temp2;
			for (int i = j + 1; i < keyList.size(); i++)
			{
				temp2.push_back(keyList[i]);
			}
			Node newNode(dbName,indexName,tableInstance,n);
			newNode.set(temp2);

			insertNonleaf(parentNode,newK,newNode.getNodePtr());
		}
	}
}

string Value::getKey()
{
	switch (type)
	{
	case _TYPE_STRING:return charKey;
	case _TYPE_INT:{char temp[260]; sprintf(temp, "%i", intKey); return temp; break; }//或者sprintf_s(temp,sizeof(float)*260...
	case _TYPE_FLOAT:{char temp[260]; sprintf(temp, "%f", floatKey); return temp; break; }
		default:return 0;
	}
}

PtrType BPTree::deleteNode(Value key)
{
	 
	PtrType ptr = find(key);
	Node node(dbName,findLeafNode(key),indexName,tableInstance,n);

	vector<Value> temp = node.getInfo();
	int i = 1;
	for(i = 1; i < temp.size(); i+=2)
	{
		if (isEqual(key,temp[i]))
			break;
	}
	if (i >= temp.size() )
		return -1;
	else
	{
		PtrType reTemp = temp[i-1].getIntKey();
		temp[i-1].setKey(-1);
		node.set(temp);
		return reTemp;
	}


}

bool BPTree::isLess(Value& key1,Value& key2)
{
	switch(key1.getType())
	{
	case _TYPE_INT:if (key1.getIntKey() < key2.getIntKey()) return true;else return false;break;
	case _TYPE_FLOAT:if (key1.getFloatKey() < key2.getFloatKey()) return true;else return false;break;
	case _TYPE_STRING:if (key1.getCharKey() < key2.getCharKey()) return true;else return false;break;
	default:
		return false;
	}
}

bool BPTree::isLessEqual(Value& key1,Value& key2)
{
	switch(key1.getType())
	{
	case _TYPE_INT:if (key1.getIntKey() <= key2.getIntKey()) return true;else return false;break;
	case _TYPE_FLOAT:if (key1.getFloatKey() <= key2.getFloatKey()) return true;else return false;break;
	case _TYPE_STRING:if (key1.getCharKey() <= key2.getCharKey()) return true;else return false;break;
	default:
		return false;
	}
}

bool BPTree::isEqual(Value& key1,Value& key2)
{
	switch(key1.getType())
	{
	case _TYPE_INT:if (key1.getIntKey() == key2.getIntKey()) return true;else return false;break;
	case _TYPE_FLOAT:if (key1.getFloatKey() == key2.getFloatKey()) return true;else return false;break;
	case _TYPE_STRING:if (key1.getCharKey() == key2.getCharKey()) return true;else return false;break;
	default:
		return false;
	}
}