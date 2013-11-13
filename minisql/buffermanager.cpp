
#include "buffermanager.h"

/**
 * @brief  用数据库名新建buffer。
 */
BufferManager::BufferManager(string name) {
  dbName = name;
  dbFileName = name + ".db";
  infoFileName = name + ".blk";
  dbFile.open(dbFileName, std::fstream::in | std::fstream::out );
  if( !dbFile.is_open() ) {
	  dbFile.open(dbFileName, std::ios::in | std::ios::out | std::ios::trunc );
  }
  infoFile.open(infoFileName, std::fstream::in | std::fstream::out );
  if( !infoFile.is_open() ) {
	  infoFile.open(infoFileName, std::ios::in | std::ios::out | std::ios::trunc );
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
	Block block;

	// 读块头
	dbFile.seekg(offset);
	dbFile.read(block.tableName, MAX_TABLE_NAME);
	dbFile.read((char *)&(block.offset), sizeof(int));
	dbFile.read((char *)&(block.nextOffset), sizeof(int));
	dbFile.read((char *)&(block.contentSize), sizeof(int));
	dbFile.read((char *)&(block.isAlive), sizeof(bool));
	dbFile.read((char *)&(block.isIndex), sizeof(bool));

	// 读块
	dbFile.seekg(offset+HEAD_LEN);
	dbFile.read(block.content, BLOCK_LEN);
	
	// 
	block.isDirty = false;

	return block;
}


void BufferManager::writeBlock(Block &block) {
	// 如果脏则洗净，否则不写
	if( !block.isDirty ) {
		return;
	} else {
		block.isDirty = false;
	}

	// 写块头
	dbFile.seekp(block.offset);
	dbFile.write(block.tableName, MAX_TABLE_NAME);
	dbFile.write((char *)&(block.offset), sizeof(int));
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


Block& BufferManager::findBlock(int offset) {
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
	string s;
	int offset = 0;

	infoFile.seekg(0, ios::beg);
	infoFile.read(tableName, MAX_TABLE_NAME);
	if( infoFile.eof() )
		return;

	infoFile.seekg(0, ios::beg);
	while( !infoFile.eof() ) {
		infoFile.read(tableName, MAX_TABLE_NAME);
		s = tableName;
		infoFile.read((char *)offset, sizeof(int));
		firstBlock.insert(pair<string, int>(s, offset));
		infoFile.read((char *)offset, sizeof(int));
		lastBlock.insert(pair<string, int>(s, offset));
	}
}


void BufferManager::writeDbInfo() {
	// 清空文件
	infoFile.close();
	infoFile.open(infoFileName, std::fstream::out | std::fstream::in | std::fstream::trunc );
	for(
		hash_map<string, int>::iterator i = firstBlock.begin(), j = lastBlock.begin();
		i != firstBlock.end();
		i ++, j++
	   ) {
		//cout << (char *)((i->first).c_str()) << (i->first).size() << endl;
		//cout << (i->second) << endl;
		infoFile.write((char *)((i->first).c_str()), (i->first).size());
		infoFile.write((char *)&(i->second), sizeof(int));
		infoFile.write((char *)&(j->second), sizeof(int));
	}
}


vector<int> BufferManager::getTableBlocks(string tableName) {
	//转换tableName为char[]
	hash_map<string, int>::iterator i;
	int offset;

	//从firstBlock中查找tableName的第一块地址
	for( i = firstBlock.begin(); i != firstBlock.end(); i ++ ) {
		if( tableName == i->first ) {
			offset = i->second;
		}
	}

	if( i == firstBlock.end() ) {
		return vector<int>();
		// TODO 未找到错误
	}

	//建立vector，查找各块中不是index的块
	vector<int> result;

	do {
		Block &block = findBlock(offset);
		if( block.isAlive ) {
			result.push_back(offset);
		}
		offset = block.nextOffset;
	} while(offset != 0);
	
	return result;
}

Block& BufferManager::newBlock(string tableName) {
	//转换tableName为char[]
	const char *tableChar = tableName.c_str();
	hash_map<string, int>::iterator i;

	//新建空Block，table为tableName, offset为db文件末尾，并直接写到文件末尾
	Block block;
	memcpy(block.tableName, tableChar, MAX_TABLE_NAME);
	dbFile.seekp(0, ios_base::end);
	block.offset = dbFile.tellp();
	writeBlock(block);
	
	//从firstBlock中查找tableName的第一块地址
	for( i = firstBlock.begin(); i != firstBlock.end(); i ++ ) {
		if( tableName == i->first ) {
			break;
		}
	}
	if( i == firstBlock.end() ) {
		// 没找到，添加一条
		firstBlock.insert(pair<string, int>(block.tableName, block.offset));
	}

	//查找LastOffset，读最后一块，设置它的nextoffset为新块的offset，设置dirty
	for( i = lastBlock.begin(); i != lastBlock.end(); i ++ ) {
		if( tableName == i->first ) {
			break;
		}
	}
	if( i == lastBlock.end() ) {
		// 没找到，添加一条
		lastBlock.insert(pair<string, int>(block.tableName, block.offset));
	} else {
		Block& temp = findBlock(i->second);
		temp.nextOffset = block.offset;
		temp.dirty();
		writeBlock(temp);
	}

	return findBlock(block.offset);
}

void BufferManager::debug(bool isContent = false) {
    cout << "BUFFER" << endl;
	for(list<Block>::iterator i = buffer.begin(); i != buffer.end(); i ++ )
	{
		i->debug(isContent);
	}
	cout << "END" << endl;
}

/* 以下部分减智商向 */

Block& BufferManager::getBlocks(int offset) {
	return findBlock(offset);
}

void BufferManager::storeBlocks(int offset, Block& block) {
	block.offset = offset;
	writeBlock(block);
}

vector<int> BufferManager::getIndexBlocks(string indexName) {
	return getTableBlocks("!"+indexName);
}

Block& BufferManager::newIndexBlock(string indexName) {
	Block b = newBlock("!"+indexName);
	b.isIndex = true;
	return b;
}

Block& BufferManager::getIndexOneBlock(string indexName, int offset) {
	return getBlocks(offset);
}

void BufferManager::writeIndexData(string indexName, char *content, int length) {
	for(int i = 0, j = 0; i < length; i += j) {
		j = (i>BLOCK_LEN) ? BLOCK_LEN : i;
		Block b = newIndexBlock(indexName);
		memcpy(b.content, content+i, j);
		b.dirty();
	}
}

void BufferManager::writeIndexData(string indexName, int offset, char* content, int length) {
	Block b = findBlock(offset);
	memcpy(b.content, content, length);
	b.dirty();
}