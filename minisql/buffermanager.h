/**
 * @file  buffermanager.h
 * @brief 缓冲区管理头文件
 * @author tgmerge
 * 缓冲区管理器。检查缓冲区中是否有要求的块，如有则返回，否则从磁盘读取。
 * 为Index和Record提供所需的块
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "block.h"

#include <string>
#include <iostream>
#include <vector>

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
public:
	// 内存中的缓冲区
	Block buffer[MAX_BLOCK_ACTIVE];

public:							// 构造，析构
	// 用数据库名初始化buffermanager
	BufferManager(string dbName);
	// 写回所有脏块，销毁buffermanager
	~BufferManager();

private:						// 自用方法，读写
	// 从某文件读一个block
	Block readBlock(string fileName, int offset);
	// 从某文件读n个连续存放的block
	vector<Block> readBlocks(string fileName, int offset, int n);
	// 将block写到文件
	bool writeBlock();
	// 写回所有缓冲区中的block到文件
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

public:							// IndexManager使用
	// 返回indexName的所有块
	vector<Block> getIndexBlocks(string IndexName);
};

#endif