/**
 * @file  buffer.h
 * @brief 缓冲区管理头文件
 * @author tgmerge
 * 缓冲区管理器。检查缓冲区中是否有要求的块，如有则返回，否则从磁盘读取。
 */

#ifndef _BUFFER_H_
#define _BUFFER_H_

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


// 块信息结构
struct blockInfo {
	// 块编号
	int blockNum;
	// 脏
	bool dirty;
	// 下一块指针
	blockInfo *next;
	// 所属文件
	fileInfo *file;
	// 块内数据长度
	int charNum;
	// 数据指针
	char *cBlock;
	// 进入缓冲区的时间
	int iTime;
	// 锁
	int lock;
};

class Buffer {

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
}
#endif