/**
 * @file  buffer.h
 * @brief 缓冲区管理头文件
 * @author tgmerge
 * 缓冲区管理器。检查缓冲区中是否有要求的块，如有则返回，否则从磁盘读取。
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "block.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

// 数据块尺寸
#define BLOCK_LEN        4096
// 缓冲区中最大文件个数
#define MAX_FILE_ACTIVE  5
// 缓冲区中最大块数
#define MAX_BLOCK_ACTIVE 40


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
};

/*
class BufferManager {

public:
	// 函数功能和文档里面一模一样w
	blockInfo *findBlock(string dbName);

	void replace(fileInfo *m_fileInfo, blockInfo *m_blockInfo);

	blockInfo *get_file_block(string dbName, string tableName, int fileType, int blockNum);

	void closeDataBase(string dbName, bool mFlag);

	void closeFile(string dbName, string m_fileName, int m_fileType, bool mFlag);

	void writeBlock(string dbName, blockInfo *block);

	fileInfo *get_file_info(string dbName, string fileName, int m_fileType);

	blockInfo* readBlock(string dbName,Cstring m_fileName, int m_blockNum, int m_fileType);

	void quitProg(string dbName);

	void Get_Table_Info(string dbName, string tableName, int recordCount, int recordLen, int free);

	void Get_Index_Info(string dbName, string indexName, int recordCount, int recordLen, int free);

	void Write_Table_Info(Cstring dbName,Cstring tableName,int recordCount,int recordLen,int free);

	void Write_ Index_Info(Cstring dbName,Cstring indexName,int recordCount,int recordLen,int free);
}*/

class BufferManager {
public:
	// 内存中的缓冲区
	Block buffer[MAX_BLOCK_ACTIVE];

public:
	// 初始化
	BufferManager();
	// 销毁全部临时数据
	~BufferManager();

private:
	// 写块到文件
	void writeBlock(int index);
	// 从文件读块
	void readBlock(string fileName, int offset, int num);

private:
	// 更新LRU值
	void updateLRU();

public:
	// 需要的功能

#endif