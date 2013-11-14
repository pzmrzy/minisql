
#include "record.h"
#include "buffermanager.h"
using namespace std;
extern BufferManager *bfm;

void record::push(Row& oneTuple,Results& results,vector<int> colNamePosVector){
    Row row;
    int i;

    for (i=0;i<colNamePosVector.size();i++)
    {
        row.col.push_back(oneTuple.col[colNamePosVector[i]]);
    }
    results.row.push_back(row);
}


bool record::checkConstraints(Row& oneTuple,vector<attribute>& attrList,vector<string>& condLeftVector,vector<string>& condOpVector,vector<string>& condRightVector){

    string condLeft;
    int pos;
    int type,condtype;

    for ( int i = 0 ; i < condLeftVector.size(); i ++ )
    {
        condLeft=condLeftVector[i];
        for (int j=0;j<attrList.size();j++){
            if (!condLeft.compare(attrList[j].name)){
                pos=j;
                break;
            }
        }
        if (attrList[pos].datatype==0){type=0;}
        else if (attrList[pos].datatype==-1){type=-1;}
        else type=1;
        if (!condOpVector[i].compare("<")) condtype=1;
        if (!condOpVector[i].compare("<=")) condtype=2;
        if (!condOpVector[i].compare("==")) condtype=3;
        if (!condOpVector[i].compare("<>")) condtype=4;
        if (!condOpVector[i].compare(">=")) condtype=5;
        if (!condOpVector[i].compare(">")) condtype=6;
        switch ( type )
        {

            case 0:
            {
				int value2 = atoi( condRightVector[i].c_str() );
                int value1 = atoi( oneTuple.col[pos].c_str());
                switch ( condtype) {
                    case 1:
                        if ( value1 >= value2 ) //<
                            return false ;
                        break ;
                    case 2:
                        if ( value1 > value2 ) //<=
                            return false;
                        break ;
                    case 3:
                        if ( value1 != value2 ) //==
                            return false ;
                        break ;
                    case 4:
                        if ( value1 == value2 ) //<>
                            return false ;
                        break ;
                    case 5:
                        if ( value1 < value2  ) //>=
                            return false ;
                        break ;
                    case 6:
                        if ( value1 <= value2 ) //>
                            return false ;
                        break ;
                }
            }
                break ;
            case -1:
            {
                float value2 = atof( condRightVector[i].c_str());
				float value1 = atof( oneTuple.col[pos].c_str());
                switch ( condtype) {
                    case 1:
                        if ( value1 >= value2 ) //<
                            return false ;
                        break ;
                    case 2:
                        if ( value1 > value2 ) //<=
                            return false;
                        break ;
                    case 3:
                        if ( value1 != value2 ) //==
                            return false ;
                        break ;
                    case 4:
                        if ( value1 == value2 ) //!=
                            return false ;
                        break ;
                    case 5:
                        if ( value1 < value2  ) //>=
                            return false ;
                        break ;
                    case 6:
                        if ( value1 <= value2 ) //>
                            return false ;
                        break ;
                }
            }
                break ;
            case 1:
            {
                string value2 = condRightVector[i];
                string value1 =  oneTuple.col[pos] ;
                switch ( condtype) {
                    case 1:
                        if ( value1 >= value2 ) //<
                            return false ;
                        break ;
                    case 2:
                        if ( value1 > value2 ) //<=
                            return false;
                        break ;
                    case 3:
                        if ( value1 != value2 ) //==
                            return false ;
                        break ;
                    case 4:
                        if ( value1 == value2 ) //!=
                            return false ;
                        break ;
                    case 5:
                        if ( value1 < value2  ) //>=
                            return false ;
                        break ;
                    case 6:
                        if ( value1 <= value2 ) //>
                            return false ;
                        break ;
                }

            }
                break ;
        }
    }
    return true ;
}

recoinfo record::writeblock(Block& blocks,int j,int tupleLen,vector<attribute>& attrList,vector<string>& colValueVector){
	int i,k,p=1;
	float floatNum;
	int intNum;
	string str;
	bool succ=false;
	Results results;
	int num=0;
	string message="";

	for (k=0;k<attrList.size();k++){
		//float
        if (attrList[k].datatype==-1){
			floatNum=atof(colValueVector[k].c_str());
            memcpy(blocks.content+j*tupleLen+p,&floatNum,sizeof(float));
			p+=sizeof(float);
			//写进去
        }
		//int
        else if(attrList[k].datatype==0) {
			intNum=atof(colValueVector[k].c_str());
			memcpy(blocks.content+j*tupleLen+p,&intNum,sizeof(int));
			p+=sizeof(int);
        }
		//string
        else {
            str=colValueVector[k];
			//长度比datatype长，返回错误信息
			if ((str.length())>attrList[k].datatype){
				succ=false;
				message="Insertion failed. Format inconsistency in "+attrList[k].name;
				return recoinfo(succ,message,results,num);
			}
			//长度比datatype短，不定长要补成定长
			else if ((str.length())<attrList[k].datatype){
				int diff=attrList[k].datatype-str.length();
				for (i=0;i<diff;i++){ str.insert(0,"\0");}
			}
			memcpy(blocks.content+j*tupleLen+p,str.c_str(),attrList[k].datatype+1);
			p+=attrList[k].datatype+1;
		}
    }
	blocks.content[j*tupleLen]=Used;
	blocks.isDirty=true;
	succ=true;
	message="Insertion suceeded";
	num=num+1;
	return recoinfo(succ,message,results,num);
}

Row record::getOneTuple(Block& blocks,int j,int tupleLen,vector<attribute>& attrList){
	int i,k,p=1;
	float floatNum;
	int intNum;
	string str;
	char ch[255];
	bool succ=false;
	int num=0;
	Row oneTuple;
	stringstream ss;

	for (k=0;k<attrList.size();k++){
		//float
        if (attrList[k].datatype==-1){
            memcpy(&floatNum,blocks.content+j*tupleLen+p,sizeof(float));
			p+=sizeof(float);
			ss<<floatNum;
            oneTuple.col.push_back(ss.str());	
        }
		//int
        else if(attrList[k].datatype==0) {
			memcpy(&intNum,blocks.content+j*tupleLen+p,sizeof(int));
			p+=sizeof(int);
			ss<<intNum;
			oneTuple.col.push_back(ss.str());
        }
		//string
        else {
			memcpy(ch,blocks.content+j*tupleLen+p,attrList[k].datatype+1);//如果底层strin类型的存储有问题的话，试着改一改datatype+1.把1去掉或怎么的都行
			p=p+attrList[k].datatype+1;
			//若是不定长储存，则去掉前面的'\0'
			str="";
			for (int ii=0;ii<attrList[k].datatype;ii++){
				if (ch[ii]!='\0') str=str+ch[ii];
			}
			str=str+"\0";
			oneTuple.col.push_back(str);
		}
    }
	return oneTuple;
}
/*
recoinfo record::Index_Delete_Rec(SqlCommand& sql,table &Table,vector<int> offset){
	int i,j;
	long num=0;//查找到的记录数
	Row row;//每行
	Results results;//总结果
	bool succ=false;//查找是否成功
	string message="";//查找失败的信息
    string databaseName=Table.dbname;
	BufferManager BM(Table.dbname);
	string tableName=Table.name;//相关的表名字
	vector<attribute> attrList=Table.attrList;//表的所有属性列表
	vector<string> condLeftVector=sql.getCondLeftVector();//条件左值
	vector<string> condOpVector=sql.getCondOpVector();//条件操作符
	vector<string> condRightVector=sql.getCondRightVevtor();//条件右值
    vector<int> blockVector=bfm.getTableBlocks(tableName);//调用buffer，得到block
    bool whereFlag=false;//判断sql中有木有where
    int blockLen;//当前block中有几条记录
    int tupleLen=Table.recLength+1;//数据中每条rec的长度
    Block blocks;
    Row oneTuple;
	int blockID;
	int recordID;

	blockLen=4096/tupleLen;
	for(i=0;i<offset.size();i++){
	recordID = i%blockLen;
	blockID=i/blockLen;
	blocks=bfm.getBlocks(blockID);
	if (blocks.content[recordID*tupleLen]==Used){
		oneTuple=getOneTuple(blocks,recordID,tupleLen,attrList);
                //如果有where，则根据条件比较查找
                //如果木有where，则不用比较
                if (!whereFlag)
				{ blocks.content[recordID*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
                else
                    if (checkConstraints(oneTuple,attrList,condLeftVector,condOpVector,condRightVector))
					{ blocks.content[recordID*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
            }
	}

}
*/
recoinfo record::Select_Rec(SqlCommand& sql,table &Table, bool indexflag, vector<int> offset )
{
	int i,j;
	long num=0;//查找到的记录数
	Row row;//每行
	Results results;//总结果
	
	bool succ=false;//查找是否成功
	string message="";//查找失败的信息
    string databaseName=Table.dbname;
	BufferManager BM(Table.dbname);
	string tableName=Table.name;//相关的表名字
	vector<attribute> attrList=Table.attrList;//表的所有属性列表
	vector<string> colNameVector=sql.getcolNameVector();//需要查找的属性
	vector<int> colNamePosVector;
	vector<string> condLeftVector=sql.getCondLeftVector();//条件左值
	vector<string> condOpVector=sql.getCondOpVector();//条件操作符
	vector<string> condRightVector=sql.getCondRightVevtor();//条件右值
    vector<int> blockVector=bfm->getTableBlocks(tableName);//调用buffer，得到block
    bool whereFlag=false;//判断sql中有木有where
    int blockLen;//当前block中有几条记录
    int tupleLen=Table.recLength+1;//数据中每条rec的长度
    //Block blocks;
    Row oneTuple;

	//如果colname=‘*’,从attrList中重生colname；
	if (!colNameVector.at(0).compare("*"))
	{
		colNameVector.clear();
		for (i=0;i<attrList.size();i++)
			colNameVector.push_back(attrList.at(i).name);
	}
	//记录查询的属性在表属性的位置，方便之后的操作。
	string colName;
	colNamePosVector.clear();
	for (i=0;i<condLeftVector.size();i++){
        colName=condLeftVector[i];
        for (j=0;j<attrList.size();j++){
            if (!colName.compare(attrList[j].name)) colNamePosVector.push_back(j);
        }
	}
	//查询结果第一行储存属性名（列名）
	row.col=colNameVector;
	results.row.push_back(row);

if (indexflag) {
		int blockID;
		int recordID;

		blockLen=4096/tupleLen;
		for(i=0;i<offset.size();i++){
			recordID = i%blockLen;
			blockID=i/blockLen;
			Block& blocks=bfm->getBlocks(blockID);
			if (blocks.content[recordID*tupleLen]==Used){
				oneTuple=getOneTuple(blocks,recordID,tupleLen,attrList);
                //如果有where，则根据条件比较查找
                //如果木有where，则不用比较
                if (!whereFlag)
                    { push(oneTuple,results,colNamePosVector); num++;succ=true;}
                else
                    if (checkConstraints(oneTuple,attrList,condLeftVector,condOpVector,condRightVector))
                        { push(oneTuple,results,colNamePosVector);num++;succ=true;}
            }	
		}
}
else{
	blockLen=4096/tupleLen;
	if (condLeftVector.size()) whereFlag=true;//判断是不是含where查找
    for (i=0;i<blockVector.size();i++){
        Block& blocks=bfm->getBlocks(i);
        for (j=0;j<blockLen;j++){
            //不是删除数据，则读出数据并解析
            if (blocks.content[j*tupleLen]==Used){
				oneTuple=getOneTuple(blocks,j,tupleLen,attrList);
                //如果有where，则根据条件比较查找
                //如果木有where，则不用比较
                if (!whereFlag)
                    { push(oneTuple,results,colNamePosVector); num++;succ=true;}
                else
                    if (checkConstraints(oneTuple,attrList,condLeftVector,condOpVector,condRightVector))
                        { push(oneTuple,results,colNamePosVector);num++;succ=true;}
            }
        }
    }
}

//返回信息
if (num==0) {succ=false; message="The results is null.";}
return recoinfo(succ,message,results,num);
}


recoinfo record::Delete_Rec(SqlCommand& sql,table &Table,bool indexflag, vector<int> offset)
{
	int i,j;
	long num=0;//查找到的记录数
	Row row;//每行
	Results results;//总结果
	bool succ=false;//查找是否成功
	string message="";//查找失败的信息
    string databaseName=Table.dbname;
	BufferManager BM(Table.dbname);
	string tableName=Table.name;//相关的表名字
	vector<attribute> attrList=Table.attrList;//表的所有属性列表
	vector<string> condLeftVector=sql.getCondLeftVector();//条件左值
	vector<string> condOpVector=sql.getCondOpVector();//条件操作符
	vector<string> condRightVector=sql.getCondRightVevtor();//条件右值
    vector<int> blockVector=bfm->getTableBlocks(tableName);//调用buffer，得到block
    bool whereFlag=false;//判断sql中有木有where
	int tupleLen=Table.recLength+1;//数据中每条rec的长度
    int blockLen=4096/tupleLen;//当前block中有几条记录
    //Block blocks;
    Row oneTuple;
	int recordID;
	int blockID;

if (indexflag){
	for(i=0;i<offset.size();i++){
	recordID = i%blockLen;
	blockID=i/blockLen;
	Block & blocks=bfm->getBlocks(blockID);
	if (blocks.content[recordID*tupleLen]==Used){
		oneTuple=getOneTuple(blocks,recordID,tupleLen,attrList);
                //如果有where，则根据条件比较查找
                //如果木有where，则不用比较
                if (!whereFlag)
				{ blocks.content[recordID*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
                else
                    if (checkConstraints(oneTuple,attrList,condLeftVector,condOpVector,condRightVector))
					{ blocks.content[recordID*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
            }
	bfm->storeBlocks(i,blocks);
	}		
}
else{
	if (condLeftVector.size()) whereFlag=true;//判断是不是含where查找
    blockLen=4096/tupleLen;
	for (i=0;i<blockVector.size();i++){
        Block & blocks=bfm->getBlocks(i);
        for (j=0;j<blockLen;j++){
            //不是删除数据，则读出数据并解析
            if (blocks.content[j*tupleLen]==Used){
				oneTuple=getOneTuple(blocks,j,tupleLen,attrList);
                //如果有where，则根据条件比较查找
                //如果木有where，则不用比较
                if (!whereFlag)
				{ blocks.content[j*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
                else
                    if (checkConstraints(oneTuple,attrList,condLeftVector,condOpVector,condRightVector))
					{ blocks.content[j*tupleLen]=Unused;num++;blocks.isDirty=true; blocks.contentSize-=tupleLen; succ=true;}
            }
        }
		bfm->storeBlocks(i,blocks);
    }
}

//返回信息
stringstream ss;
ss<<num;
message="Total "+ss.str()+" record(s) in deletion";
return recoinfo(succ,message,results,num);
}


recoinfo record::Insert_Rec(SqlCommand& sql,table &Table, int &blockID, int &recordID)
{

	int i,j,k;
	long num=0;//查找到的记录数
	Row row;//每行
	Results results;//总结果
	recoinfo recoInfo;
	bool succ=false;//查找是否成功
	string message="";//查找失败的信息
    string databaseName=Table.dbname;
	BufferManager BM(Table.dbname);
	string tableName=Table.name;//相关的表名字
	vector<attribute> attrList=Table.attrList;//表的所有属性列表
    vector<string> colValueVector=sql.getcolValueVector();//需要插入的rec的值
    vector<int> blockVector=bfm->getTableBlocks(tableName);//调用buffer，得到block
    bool whereFlag=false;//判断sql中有木有where
    int blockLen;//当前block中有几条记录
    int tupleLen=Table.recLength+1;//数据中每条rec的长度
    //Block blocks;
    Row oneTuple;
	int p;

    //假设在catalog中已经判断insert的值数量与属性数量一致
    int intNum;
    float floatNum;
    string str;


	blockLen=4096/tupleLen;
    for (i=0;i<blockVector.size();i++){
		Block & blocks=bfm->getBlocks(i);
		p=1;
        for (j=0;j<blockLen;j++){
            //若是删除数据，则将删除数据替换成新数据
            if (blocks.content[j*tupleLen]==Unused){
				recoInfo=writeblock(blocks,j,tupleLen,attrList,colValueVector);//写入一条记录
				if (recoInfo.getsucc()) {blocks.content[j*tupleLen]=Used; blocks.isDirty=true;blocks.contentSize+=tupleLen;
				bfm->storeBlocks(i,blocks);
				blockID=i;
				recordID=j; 
				return recoInfo;}
				else  return recoInfo;

			}
        }
    }

	//写在最后面
	{
		//最后一块block还写的下
		/*if (blockLen<(BLOCK_LEN/tupleLen)){
			i=blockVector.size()-1;
			recoInfo=writeblock(blocks,j,tupleLen,attrList,colValueVector);//写入一条记录
			if (recoInfo.getsucc()) {blocks.contentSize+=tupleLen;blockID=i;recordID=j; bfm.storeBlocks(i,blocks); return recoInfo;}
		}*/
		//新建立一个block
		{
			Block& blocks=bfm->newBlock(tableName);
			recoInfo=writeblock(blocks,0,tupleLen,attrList,colValueVector);//写入一条记录
			if (recoInfo.getsucc()) {
				blocks.contentSize+=tupleLen;
				blocks.content[0*tupleLen]=Used;blocks.isDirty=true;
				blockID=i;recordID=0;
				bfm->storeBlocks(i,blocks);

				return recoInfo;}
		}
	}
//返回信息
if (num==0) {succ=false; message="Insert Unsucceed.";}
return recoinfo(succ,message,results,num);
}

