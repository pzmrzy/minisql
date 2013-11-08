/**
 * @file  buffermanager.h
 * @brief 缓冲区管理头文件
 * @author tgmerge
 * 缓冲区管理器。检查缓冲区中是否有要求的块，如有则返回，否则从磁盘读取。
 * 为Index和Record提供所需的块
 *
 * 每个BufferManager对象只打开一个数据库（文件），文件名是"数据库名.db"。
 * 构造方法需要传入数据库名。
 * 使用BufferManager时不需要申请写块到文件，只需要在块发生改变时调用块的dirty()方法把它设置成脏的
 * BufferManager会在脏块被替换出缓存，或自身被销毁之前将它们写回文件。
 */

/**
表的块索引dbName.blk
记录每个table第一个块和最后一个块的偏移量
是二进制文件
tableName[MAX_TABLE_NAME]  Firstoffset[4]  Lastoffset[4]
tableName[MAX_TABLE_NAME]  Firstoffset[4]  Lastoffset[4]
...
*/


#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "block.h"

#include <string>
#include <iostream>
#include <list>
#include <hash_map>
#include <fstream>

using namespace std;

// 缓冲区中最大块数
#define MAX_BLOCK_ACTIVE 40
#define MAX_TABLE_NAME   32

class BufferManager {

private:
	string dbName;
	string dbFileName;
	string infoFileName;
	fstream dbFile;
	fstream infoFile;
private:
	// 各表第一块的偏移string:tableName, int:offset
	hash_map<char[MAX_TABLE_NAME], int> firstBlock;
	// 各表最后一块的偏移string:tableName, int:offset
	hash_map<char[MAX_TABLE_NAME], int> lastBlock;

public:
	// 内存中的缓冲区
	list<Block> buffer;

public:							// 构造，析构
	// 用数据库名初始化buffermanager
	BufferManager(string name);
	// 写回所有脏块，销毁buffermanager
	virtual ~BufferManager();

private:						// 自用方法，读写
	// 从某文件读一个block
	Block readBlock(int offset);
	// 将block写到文件（若是干净的就不写了）
	void writeBlock(Block &block);
	// 写回所有缓冲区中的脏block到文件
	void writeAllBlocks();

private:						// 自用方法，查找块相关，LRU
	// 按offset在缓存和文件中查找块，并存入缓存
	Block findBlock(int offset);

private:						// 自用，更新表索引.blk
	// 读取dbFile的表块索引文件, 若失败则新建一个
	void readDbInfo();
	// 写索引文件
	void writeDbInfo();

public:							// RecordManager使用
	// TODO 返回tableName的所有块号
	vector<int> getTableBlocks(string tableName);
	// 给tableName添加一个新块,若不在firstLast中也添加至其中
	Block newBlock(string tableName);
	// 写数据
	Block StoreData(string tableName, char[] content);

public:							// IndexManager使用
	// TODO 返回indexName的所有块偏移量
	vector<int> getIndexBlocks(string IndexName);
	// 给tableName添加一个新块
	Block newIndexBlock(string IndexName);
	// 写index数据
	Block storeIndex(string IndexName, char[] content);
	// 传入offset, 读块并返回content
	char[] readIndexContent( int offset );
};

#endif