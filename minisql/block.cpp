#include "block.h"

Block::Block() {
	tableName[32];
	nextOffset = 0;
	contentSize = 0;
	isAlive = false;
	offset = 0;
	isDirty = true;
	isActive = true;
    value = 0;
	isIndex = false;
}

Block::Block(bool index) {
	tableName[32];
	nextOffset = 0;
	contentSize = 0;
	isAlive = false;
	offset = 0;
	isDirty = true;
	isActive = true;
    value = 0;
	isIndex = index;
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

