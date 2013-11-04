#include "block.h"

Block::Block() {
	fileName = "";
	tableName = "";
	offset = 0;
	contentSize = 0;
	isDirty = true;
	active = false;
    value = 0;
}

void Block::dirty() {
	isDirty = true;
}

char* Block::getContent() {
	return content;
}

int Block::getSize() {
	return contentSize;
}

void Block::write() {
  fstream fs;
  fs.open(fileName, std::fstream::out );
  fs.seekp(offset);
  fs.write(content, BLOCK_LEN);
  fs.close();
}