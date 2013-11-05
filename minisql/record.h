#ifndef _RECORD_H_
#define _RECORD_H_

#include "minisql.h"
#include "sqlcommand.h"
//#include "BufferManager.h"
#include "block.h"
#include <string>
#include <iostream>
#include <vector>
#include <iostream>
#include <strstream>
#include <sstream>
#include "minisql.h"
using namespace std;

#define BLOCK_LEN        4096
#define Used 0x01
#define Unused 0x00
///////////////////////////////////////////////////
class record
{
public:
	record(void){}
	virtual ~record(void){}
	//查找操作
	recoinfo Select_Rec(SqlCommand& sql,table &Table);
	//删除操作
	recoinfo Delete_Rec(SqlCommand& sql,table &Table);
	//插入操作
	recoinfo Insert_Rec(SqlCommand& sql,table &Table);
private:
    //解析数据中每条record
    Row parser(istrstream& inp,vector<attribute>& attrList);
    //按要求的列返回结果
    void push(Row& oneTuple,Results& results,vector<int> colNamePosVector);
    //判断是否满足约束条件
    bool checkConstraints(Row& oneTuple,vector<attribute>& attrList,vector<string>& condLeftVector,vector<string>& condOpVector,vector<string>& condRightVector);
	recoinfo writeblock(Block& blocks,int j,int tupleLen,vector<attribute>& attrList,vector<string>& colValueVector);//写入一条记录
	Row getOneTuple(Block& blocks,int j,int tupleLen,vector<attribute>& attrList);
};
class BufferManager {
/*public:
        // 内存中的缓冲区
        Block buffer[MAX_BLOCK_ACTIVE];*/

public:                                                        // 构造，析构
        // 用数据库名初始化buffermanager
        //BufferManager(string dbName);
	BufferManager(){}
        // 写回所有脏块，销毁buffermanager
	~BufferManager(){}

/*private:                                                // 自用方法，读写
        // 从某文件读一个block
        Block readBlock(string fileName, int offset);
        // 从某文件读n个连续存放的block
        vector<Block> readBlocks(string fileName, int offset, int n);
        // 将block写到文件
        bool writeBlock();
        // 写回所有缓冲区中的block到文件
        bool writeAllBlocks();

private:                                                // 自用方法，LRU相关
        // 更新某块的LRU
        void updateLru(Block& b);
        // 更新缓冲中所有块的LRU
        void updateAllLru();
*/
public:                                                        // RecordManager使用
        // 返回tableName的所有块
        static vector<Block> getTableBlocks(string tableName){
			Block block;
			vector<Block> blockVector;
			
			for (int i=0;i<2;i++){
			if (i>=1){
				block.fileName="student.table";                        // 文件名(数据库名)
				block.tableName="student";                        // 表名
				//block.offset=i*4096;
				// block在文件中的偏移量
				block.content[0]=1;block.content[1]='2';block.content[2]='3';block.content[3]='4';
				block.content[4]='5';block.content[5]='6';block.content[6]='\0';block.content[7]='W';
				block.content[8]='D';block.content[9]='\0' ;block.content[10]=22;block.content[11]=0;
				block.content[12]=0;block.content[13]=0;block.content[14]='M';block.content[15]='\0';

				block.content[16]=1;block.content[17]='2';block.content[18]='2';block.content[19]='2';
				block.content[20]='2';block.content[21]='2';block.content[22]='\0';block.content[23]='Y';
				block.content[24]='S';block.content[25]='\0' ;block.content[26]=20;block.content[27]=0;
				block.content[28]=0;block.content[29]=0;block.content[30]='M';block.content[31]='\0';

				block.content[32]=1;block.content[33]='3';block.content[34]='3';block.content[35]='3';
				block.content[36]='3';block.content[37]='3';block.content[38]='\0';block.content[39]='S';
				block.content[40]='F';block.content[41]='\0' ;block.content[42]=16;block.content[43]=0;
				block.content[44]=0;block.content[45]=0;block.content[46]='F';block.content[47]='\0';

				block.content[48]=1;block.content[49]='4';block.content[50]='4';block.content[51]='4';
				block.content[52]='4';block.content[53]='4';block.content[54]='\0';block.content[55]='Z';
				block.content[56]='H';block.content[57]='\0' ;block.content[58]=22;block.content[59]=0;
				block.content[60]=0;block.content[61]=0;block.content[62]='F';block.content[63]='\0';

				/*for (int j=0;j<BLOCK_LEN;j++){
					if ((j%16)==0) block.content[j]=0x00;
					else
					block.content[j]=0x01;       // 数据
				}*/
				block.contentSize=BLOCK_LEN;                        // 数据长度
			    block.dirty=false;                                        // 是否需要写回文件
				//bool active;                                // for lru
				//int value;                                        // for lru
			}
			else {
				block.fileName="student.table";                        // 文件名(数据库名)
				block.tableName="student";                        // 表名
				//block.offset=i*4096;                                        // block在文件中的偏移量
				for (int j=0;j<BLOCK_LEN;j++){
					if ((j>=0)&&(j<=31)) block.content[j]=0x00;
					else block.content[j]=0x00;       // 数据
				}
				block.contentSize=BLOCK_LEN;                        // 数据长度
				block.dirty=false;                                        // 是否需要写回文件
				//bool active;                                // for lru
				//int value;							// for lru
			}
			blockVector.push_back(block);
			}
			return blockVector;
		}

		static Block newBlock(string tableName){
			Block block;
			block.fileName="student.table";                        // 文件名(数据库名)
				block.tableName="student";                        // 表名
				//block.offset=i*4096;                                        // block在文件中的偏移量
				//block.content[BLOCK_LEN];        // 数据
				for (int j=0;j<BLOCK_LEN;j++){
					block.content[j]=0x00;;        // 数据
				}
				block.contentSize=BLOCK_LEN;                        // 数据长度
			    block.dirty=true;                                        // 是否需要写回文件
				//bool active;                                // for lru
				//int value;                                        // for lru
				return block;
		}
        // 返回tableName的某个块（offset从IndexManager取得？）
        //static Block getTableBlock(string tableName, int offset);

//public:                                                        // IndexManager使用
        // 返回indexName的所有块
        //vector<Block> getIndexBlocks(string IndexName);
};
#endif
