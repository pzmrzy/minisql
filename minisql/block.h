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

#define BLOCK_LEN        4096

class Block {
public:
	string fileName;
	int offset;
	char content[BLOCK_LEN];
	bool dirty;
	bool active; // for lru
	int value;   // for lru

public:
	Block();
	string getContentFrom(int start, int length);
};

#endif