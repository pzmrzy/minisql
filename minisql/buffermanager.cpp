#include "buffermanager.h"

/**
 * @brief  用数据库名新建buffer。
 */
BufferManager::BufferManager(string name) {
  // 定义文件名为"数据库名.db"
  dbName = name;
  dbFileName = name + ".db";
  // TODO 将数据库名转换为
  dbFile.open(dbFileName, std::fstream::out | std::fstream::in );
  // 若不存在则新建文件
  if( !dbFile ) {
	  // TODO: 若不存在则新建文件
  }
}

/**
 * @brief  关闭buffer之前，从buffer写回所有脏块
 */
BufferManager::~BufferManager() {
	writeAllBlocks();
}

/**
 * @brief  从数据库文件读取一个block，指定偏移量
 */
BufferManager::readBlock(string fileName, int offset) {
	// TODO 说明fileName已经弃用了
	