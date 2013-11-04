/**
 * @file   block.h
 * @brief  缓冲数据块
 * @author tgmerge
 * 
 * 当收到BufferManager传回的块后，
 * 使用getContent方法得到内部数据（字符串数组）指针；
 * 使用getSize方法得到内部数据的有效长度
 */

/*
文件中的块结构
[   HEAD   ][          CONTENT          ]
HEAD_LEN   64   HEAD的长度
BLOCK_LEN  4096  CONTENT的长度

HEAD结构
TABLE_NAME      char*  32    该块的表名
NEXT_OFFSET     int    4     下一块的偏移量
CONTENT_SIZE    int    4     块内有效数据的长度
IS_ALIVE        bool   1     该块是否已被删除

CONTENT结构
CONTENT         char*  4096  数据
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
//块头尺寸
#define HEAD_LEN  64

class Block {

private:					// 块头数据
	string tableName;			// 表名
	int nextOffset;             // 下一块在文件中的偏移量
	int contentSize;			// 块内有效数据长度
	bool isAlive;               // 该块是否已被删除
public:						// 块数据
	char content[BLOCK_LEN];	// 数据
private:					// 其他
	int offset;					// 该block在文件中的偏移量
	bool isDirty;				// 是否需要写回文件
	bool isActive;				// for lru
	int value;					// for lru

public:							// 构造，析构
	// 初始化一个新块
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