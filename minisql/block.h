/**
 * @file  block.h
 * @brief 缓冲数据块
 */

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <iostream>
#include <vector>

using namespace std;

//块内数据最大尺寸
#define BLOCK_LEN 4096

class Block {

public:
	string fileName;			// 文件名(数据库名)
	string tableName;			// 表名
	int offset;					// block在文件中的偏移量
	char content[BLOCK_LEN];	// 数据
	bool dirty;					// 是否需要写回文件
	bool active;				// for lru
	int value;					// for lru

public:							// 构造，析构
	// 初始化一个新块, fileName, tableName, offset均需设置
	Block();

	// 如果dirty，先写回文件再销毁
	~Block();

private:						// 自用方法，读写
	// 从某文件读一个block
	Block read(string fileName, int offset);

	// 将block写到文件
	bool write();
	
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