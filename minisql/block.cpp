#include "block.h"

Block::Block() {
	tableName[0] = 0;
	nextOffset = 0;
	contentSize = 0;
	isAlive = true;
	offset = 0;
	isDirty = true;
	isIndex = false;
}

Block::Block(bool index) {
	tableName[0] = 0;
	nextOffset = 0;
	contentSize = 0;
	isAlive = false;
	offset = 0;
	isDirty = true;
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

void Block::debug(bool withContent) {
	cout << "\n[BLOCK][" << tableName << "][" << offset << "]" << endl;
	if(!isAlive)cout << "NOT "; cout << "alive" << endl;
	if(!isIndex)cout << "NOT "; cout << "index" << endl;
	if(!isDirty)cout << "NOT "; cout << "dirty" << endl;
	cout << "next=" << nextOffset << endl;
	cout << "len =" << contentSize << endl;
	if(withContent) cout << content << endl;
	return;
}