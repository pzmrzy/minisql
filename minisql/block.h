/**
 * @file   block.h
 * @brief  缓冲数据块
 * @author tgmerge
 * 
 * 当收到BufferManager传回的块后，
 * 使用getContent方法得到内部数据（字符串数组）指针；
 * 使用getSize方法得到内部数据的有效长度
 */

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include <string>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//块内数据最大尺寸
#define BLOCK_LEN 4096

class Block {

public:
	string fileName;			// 文件名(数据库名)
	string tableName;			// 表名
	int offset;					// block在文件中的偏移量
	char content[BLOCK_LEN];	// 数据
	int contentSize;			// 数据长度
	bool isDirty;					// 是否需要写回文件
	bool active;				// for lru
	int value;					// for lru

public:							// 构造，析构
	// 初始化一个新块, fileName, tableName, offset均需设置
	Block();
	// 如果dirty，先写回文件再销毁
	~Block();

private:						// 读写文件
	void write();

public:							// 建议用下列方法读写属性
	// 设置这个块是脏的，以便写回文件
	void dirty();

	// 读取这个块内部的数据
	char* getContent();

	// 读取这个块内部的数据长度
	int getSize();

}

#endif