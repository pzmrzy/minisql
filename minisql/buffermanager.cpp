
#include "buffermanager.h"

/**
 * @brief  用数据库名新建buffer。
 */
BufferManager::BufferManager(string name) {
  dbName = name;
  dbFileName = name + ".db";
  infoFileName = name + ".blk";
  dbFile.open(dbFileName, std::fstream::out | std::fstream::in );
  infoFile.open(infoFileName, std::fstream::out | std::fstream::in );
  if( !dbFile ) {
	  // TODO: 若不存在则新建文件
  }
  readDbInfo();
}

/**
 * @brief  关闭buffer之前，从buffer写回所有脏块
 */
BufferManager::~BufferManager() {
	writeAllBlocks();
	writeDbInfo();
}

/**
 * @brief  从数据库文件读取一个block，指定偏移量
 */
Block BufferManager::readBlock(int offset) {
	char temp[33];
	Block block;

	// 读块头
	dbFile.seekg(offset);
	dbFile.read(block.tableName, MAX_TABLE_NAME);
	dbFile.read((char *)&(block.nextOffset), sizeof(int));
	dbFile.read((char *)&(block.contentSize), sizeof(int));
	dbFile.read((char *)&(block.isAlive), sizeof(bool));
	dbFile.read((char *)&(block.isIndex), sizeof(bool));


	// 读块
	dbFile.seekg(offset+HEAD_LEN);
	dbFile.read(block.content, BLOCK_LEN);
	
	block.isDirty = false;
	block.isActive = true;
	block.value = 0;

	return block;
}


void BufferManager::writeBlock(Block &block) {
	if( !block.isDirty ) {
		return;
	}

	// 写块头
	dbFile.seekp(block.offset);
	dbFile.write(block.tableName, MAX_TABLE_NAME);
	dbFile.write((char *)&(block.nextOffset), sizeof(int));
	dbFile.write((char *)&(block.contentSize), sizeof(int));
	dbFile.write((char *)&(block.isAlive), sizeof(bool));
	dbFile.write((char *)&(block.isIndex), sizeof(bool));

	// 写块
	dbFile.seekp(block.offset+HEAD_LEN);
	dbFile.write(block.content, BLOCK_LEN);

	return;
}

void BufferManager::writeAllBlocks() {
	for(list<Block>::iterator i = buffer.begin(); i != buffer.end(); i ++ ) {
		writeBlock(*i);
	}
	return;
}


Block BufferManager::findBlock(int offset) {
	// 先在缓存中查找，找到则将其挂在缓存最前头
	for(list<Block>::iterator i = buffer.begin(); i != buffer.end(); i ++ ) {
		if( i->offset == offset ) {
			buffer.splice( buffer.begin(), buffer, i, std::next(i) );
			return *(buffer.begin());
		}
	}
	// 找不到？若缓存满写并删除缓存末尾块，读文件并挂入缓存
	if(buffer.size() >= MAX_BLOCK_ACTIVE) {
		writeBlock(*(buffer.end()));
		buffer.pop_back();
	}
	buffer.push_front(readBlock(offset));
	return *(buffer.begin());
}


void BufferManager::readDbInfo() {
	char tableName[MAX_TABLE_NAME];
	int offset;
	infoFile.seekg(0);
	while( !infoFile.eof() ) {
		infoFile.read(tableName, MAX_TABLE_NAME);
		infoFile.read((char *)offset, sizeof(int));
		firstBlock.insert(pair<char[MAX_TABLE_NAME], int>(tableName, offset));
		infoFile.read((char *)offset, sizeof(int));
		lastBlock.insert(pair<char[MAX_TABLE_NAME], int>(tableName, offset));
	}
}


void BufferManager::writeDbInfo() {
	// 清空文件
	infoFile.close();
	infoFile.open(infoFileName, std::fstream::out | std::fstream::in | std::fstream::trunc );
	for(
		hash_map<char[MAX_TABLE_NAME], int>::iterator i = firstBlock.begin(), j = lastBlock.begin();
		i != firstBlock.end();
		i ++, j++
	   ) {
		infoFile.write((char *)(i->first), MAX_TABLE_NAME);
		infoFile.write((char *)(i->second), sizeof(int));
		infoFile.write((char *)(j->second), sizeof(int));
	}
}


vector<int> BufferManager::getTableBlocks(string tableName) {
	//转换tableName为char[]
	const char *tableChar = tableName.c_str();
	hash_map<char[MAX_TABLE_NAME], int>::iterator i;
	int offset;

	//从firstBlock中查找tableName的第一块地址
	for( i = firstBlock.begin(); i != firstBlock.end(); i ++ ) {
		if( strcmp(tableChar, i->first) == 0 ) {
			offset = i->second;
		}
	}

	if( i == firstBlock.end() ) {
		// TODO 未找到错误
	}

	//建立vector，查找各块中不是index的块
	vector<int> result;
	Block block;

	do {
		block = findBlock(offset);
		if( block.isAlive && !(block.isIndex) ) {
			result.push_back(offset);
		}
		offset = block.nextOffset;
	} while(block.nextOffset == 0);
	
	return result;
}

Block BufferManager::newBlock(string tableName) {
	//转换tableName为char[]
	const char *tableChar = tableName.c_str();
	hash_map<char[MAX_TABLE_NAME], int>::iterator i;
	int offset;

	//新建空Block，table为tableName, offset为db文件末尾，并直接写到文件末尾
	Block block();
	


	//从firstBlock中查找tableName的第一块地址
	for( i = firstBlock.begin(); i != firstBlock.end(); i ++ ) {
		if( strcmp(tableChar, i->first) == 0 ) {
			offset = i->second;
		}
	}

	//查找LastOffset，读最后一块，设置它的nextoffset为新块的offset，设置dirty

	//返回新块
}
