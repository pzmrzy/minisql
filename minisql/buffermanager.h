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
#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "block.h"

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// 缓冲区中最大文件个数
#define MAX_FILE_ACTIVE  5
// 缓冲区中最大块数
#define MAX_BLOCK_ACTIVE 40

/* 弃用
// 文件头结构
struct fileInfo {
	// 0=数据，1=索引
	int type;
	// 文件名
	string fileName;
	// 记录数目
	int recordAmount;
	// 可用块数
	int freeNum;
	// 记录长度
	int recordLength;
	// 下一个文件的指针
	fileInfo *next;
	// 第一个块指针
	blockInfo *firstBlock;
};*/

class BufferManager {

private:
	// 数据库名
	string dbName;
	// 数据库文件名
	string dbFileName;
	// 数据库文件
	fstream dbFile;

public:
	// 内存中的缓冲区
	Block buffer[MAX_BLOCK_ACTIVE];

public:							// 构造，析构
	// 用数据库名初始化buffermanager
	BufferManager(string name);
	// 写回所有脏块，销毁buffermanager
	~BufferManager();

private:						// 自用方法，读写
	// 从某文件读一个block
	Block readBlock(int offset);
	// 从某文件读n个连续存放的block
	vector<Block> readBlocks(string fileName, int offset, int n);
	// 将block写到文件（若是干净的就不写了）
	bool writeBlock();
	// 写回所有缓冲区中的脏block到文件
	bool writeAllBlocks();

private:						// 自用方法，LRU相关
	// 更新某块的LRU
	void updateLru(Block& b);
	// 更新缓冲中所有块的LRU
	void updateAllLru();

public:							// RecordManager使用
	// 返回tableName的所有块
	static vector<Block> getTableBlocks(string tableName);
	// 返回tableName的某个块（offset从IndexManager取得？）
	static Block getTableBlock(string tableName, int offset);
	// 给tableName添加一个新块
	static Block newBlock(string tableName);
	// 要求写块到硬盘

public:							// IndexManager使用
	// 返回indexName的所有块
	vector<Block> getIndexBlocks(string IndexName);

};

#endif