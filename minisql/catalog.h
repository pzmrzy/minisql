#ifndef _CATALOG_H_
#define _CATALOG_H_
#include"minisql.h"

class catalog
{
public:
	catalog(void);
	virtual ~catalog(void);
	//创建数据库
	catainfo creat_Database(SqlCommand& sqlcmd);
	//创建表
	catainfo creat_Table(SqlCommand& sqlcmd);
	//创建索引
	catainfo creat_Index(SqlCommand& sqlcmd);
	//选择
	catainfo select_Rec(SqlCommand& sqlcmd);
	//插入记录
	catainfo insert_Rec(SqlCommand& sqlcmd);
	//删除记录
	catainfo delete_Rec(SqlCommand& sqlcmd);
	//删除索引
	catainfo drop_Index(SqlCommand& sqlcmd);
	//删除表
	catainfo drop_Table(SqlCommand& sqlcmd);
	//删除数据库
	catainfo drop_Database(SqlCommand& sqlcmd);
	//使用数据库
	catainfo use_Database(SqlCommand& sqlcmd);
private:
	//判断数据库是否存在
	bool exist_Database(string dbname);
	//判断表是否存在
	bool exist_Table(string dbname, string tbname);
	//判断索引是否存在
	bool exist_Index(string dbname, string tbname, string indname);
	//获取属性信息
	attribute get_Attr(string dbname, string tbname, string attrname);

};
#endif
