#ifndef _CATALOG_H_
#define _CATALOG_H_

#include"minisql.h"
#include"sqlcommand.h"
#define CATA "catalog.list"
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
	//初始化 第一次建表时调用
	void init(string& dbname);

	void change_Table(table& t);

	catainfo show_Database(SqlCommand& sqlcmd);

private:
	//判断数据库是否存在
	bool exist_Database(string& dbname);
	//判断表是否存在
	bool exist_Table(string& dbname, string& tbname);
	//判断索引是否存在
	bool exist_Index(string& dbname, string& tbname, string& indname);
	//写表数
	void writeHead(fstream& f, tableNum& t);
	//读表数
	void readHead(fstream& f, tableNum& t);
	//读表
	void readTable(fstream& f, table& t);
	//写表
	void writeTable(fstream& f, table& t);
	//读属性
	void readAttr(fstream& f, attribute& attr);
	//写属性
	void writeAttr(fstream& f, attribute& attr);
	//判断类型是否相符
	bool check(int t, string& str);
};
#endif
