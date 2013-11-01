#include "block.h"

Block::Block() {
	value = 0;
	dirty = false;
	active = false;
}

string Block::getContentFrom(int start, int length) {
	string result("");
	for( int i = start; i < start+length; i ++ )
		result += content[i];
	return result;
}