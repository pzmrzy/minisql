/**
 * @file  interpreter.cpp
 * @brief 解释器的实现
 *
   大概流程
   cin:    "drop table    tableA;"
   |  readInput();  单词等格式整理
   v
   string: "drop table tableA ;"
   |  getExpression();  获取SqlCommand，中间会调用dropTable();
   v
   SqlCommand 包含各种信息，发给API
   
 */
#include "interpreter.h"
#include <cstring>
#include <vector>

using namespace std;

string delFirstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	    start   = str.find_first_of(split, start);
		start   = str.find_first_not_of(split, start);
	return str.substr(start);
}
string firstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	int end     = str.find_first_of(split, start);
	return str.substr(start, end-start);
}

/**
 * @brief  取字符串的第一个单词
 * @author tgmerge
 * @param  string& 要处理的串
 *         string  分隔符
 * @return string  第一个单词
 */
string Interpreter::firstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	int end     = str.find_first_of(split, start);
	return str.substr(start, end-start);
}

/**
 * @brief  删除字符串的第一个单词
 * @author tgmerge
 * @param  string& 要处理的串
 *         string  分隔符
 * @return string  剩下的字符串
 */
string Interpreter::delFirstWord(string& str, string split) {
	int start   = str.find_first_not_of(split);
	    start   = str.find_first_of(split, start);
		start   = str.find_first_not_of(split, start);
	return str.substr(start);
}

/**
 * @brief  从用户读取输入，删除多余空格
 * @author tgmerge
 * @return 整理后的字符串
 */
string Interpreter::readInput() {
	string sql("");
	string temp("");
	bool fin = false;

	

	while( !fin ) {
		cin >> temp;
		sql += temp + " ";
		if( sql.at(sql.length()-2) == ';' ) {
			sql = sql.substr(0, sql.length()-2);
			sql += " ;";
			fin = true;
		}
	}

	return sql;
}

SqlCommand Interpreter::dropDatabase(string& str) {
	SqlCommand sql;
	string name;
	sql.setType(SQL_DROP_DATABASE);
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");
	name = firstWord(str, " ;");
	sql.setDatabaseName(name);
	return sql;
}

SqlCommand Interpreter::dropTable(string& str) {
	SqlCommand sql;
	string name;
	sql.setType(SQL_DROP_TABLE);
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");
	name = firstWord(str, " ;"); // ←readInput()那里应该已经处理过";"让它变成" ;"了 by tgmerge
	sql.setTableName(name);
	return sql;
}

/**
 * @brief  检验drop index语句合法性
 * @author tgmerge
 * @param  string&    SQL指令
 * @return SqlCommand 类型为SQL_DROP_INDEX的内部命令
 */
SqlCommand Interpreter::dropIndex(string& str){
	// 现在的str示例  drop index indexA ;
	SqlCommand sql;
	string name;

	// 删除"drop index"
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");

	// 取索引名
	name = firstWord(str, " ");

	// TODO: 检验索引存在性

	sql.setType(SQL_USE);
	sql.setIndexName(name);

	return sql;
}

/**
 * @brief  检验use语句合法性
 * @author tgmerge
 * @param  string&    SQL指令
 * @return SqlCommand 类型为SQL_USE的内部命令
 */
SqlCommand Interpreter::useClause(string& str){
	// 现在的str示例  use database1 ;
	SqlCommand sql;
	string name;

	//删除"use"
	str = delFirstWord(str, " ");
	
	//取数据库名称
	name = firstWord(str, " ");

	// TODO: 检验数据库名称存在性
	sql.setType(SQL_USE);
	sql.setDatabaseName(name);

	return sql;
}

/**
 * @brief  检验create database语句合法性
 * @author tgmerge
 */
SqlCommand Interpreter::createDatabase(string& str) {
	// 现在的str: create database database1 ;
	SqlCommand sql;
	string name;

	// 删除"create database"
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");

	// 取数据库名称
	name = firstWord(str, " ");

	sql.setType(SQL_CREATE_DATABASE);
	sql.setDatabaseName(name);

	return sql;
}

/**
 * @brief  检验create table语句合法性
 * @author tgmerge
 */
SqlCommand Interpreter::createTable(string& str) {
	// 现在的str: create table table1 ;
	SqlCommand sql;
	string name;

	// 删除"create table"
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");

	// 取表名
	name = firstWord(str, " ");

	sql.setType(SQL_CREATE_TABLE);
	sql.setTableName(name);

	// ---

	string attr, type, temp, special;
	for( temp=firstWord(str, " (),"); temp == ""; temp=firstWord(str, " (),")) {
		attr = firstWord(str, " ");
		delFirstWord(str, " ");
		type = firstWord(str, " ");
		delFirstWord(str, " ");
		special = firstWord(str, ",");
		sql.colNameVector.push_back(attr);
		//sql.colValueVector.push_back(type);
		int tmp;
		if (type == "int")
			tmp = 0;
		else if (type == "float")
			tmp = -1;
		else {
			type = delFirstWord(type, "( ");
			temp = firstWord(type, " )");
			tmp = atof(temp.c_str());
		}
		sql.colType.push_back(tmp);
		sql.colSpecialVector.push_back(special);
	}

	return sql;
}

/**
 * @brief  检验create index语句合法性
 * @author tgmerge
 */
SqlCommand Interpreter::createIndex(string& str) {
	SqlCommand sql;
	string indexName;
	string tableName;
	string colName;

	// 删除"create index"
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");

	// 取索引名
	indexName = firstWord(str, " ");

	// 删除"索引名 on"
	// TODO: 判断是否是ON
	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");
	tableName = firstWord(str, " ()");
	str = delFirstWord(str, " ()");
	colName = firstWord(str, " ()");

	sql.setType(SQL_CREATE_INDEX);
	sql.setIndexName(indexName);
	sql.setTableName(tableName);
	sql.setcolName(colName);

	return sql;
}

/**
 * @brief  解析命令，调用各检测方法，返回一个SqlCommand对象供API使用
 * @author tgmerge
 * @param  string     要解析的字符串
 * @return SqlCommand 解析完成的命令对象
 * TODO: 未完成
 */
SqlCommand Interpreter::getExpression(string input) {

	// 要返回的对象
	SqlCommand sql;
	
	// 取出的第一个单词
	string firstStr = firstWord(input, " ");
	
	// 根据第一个单词判断
	// 第一个是use
	if( firstStr == "use" ) {
		sql = useClause(input);
	}
	// 第一个是drop
	else if( firstStr == "drop" ) {
		// 根据第二个单词判断
		string secondStr = delFirstWord(input, " ");
		       secondStr = firstWord(secondStr, " ");
		if( secondStr == "database" ) {
			sql = dropDatabase(input);
		}
		else if( secondStr == "table" ) {
			sql = dropTable(input);
		}
		else if( secondStr == "index" ) {
			sql = dropIndex(input);
		}
		// 无法匹配
		else {
			sql.setType(SQL_ERROR);
		}
	}
	// 第一个是create
	else if( firstStr == "create" ) {
		// 根据第二个单词判断
		string secondStr = delFirstWord(input, " ");
			   secondStr = firstWord(secondStr, " ");
		if( secondStr == "database" ) {
			sql = createDatabase(input);
		}
		else if( secondStr == "table" ) {
			sql = createTable(input);
		}
		else if( secondStr == "index" ) {
			sql = createIndex(input);
		}
		// 无法匹配
		else {
			sql.setType(SQL_ERROR);
		}
	}
	// 第一个是select
	else if( firstStr == "select" ) {
		sql = selectClause(input);
	}
	// 第一个是delete
	else if( firstStr == "delete" ) {
		sql = deleteFromWhere(input);
	}
	// 第一个是insert
	else if( firstStr == "insert" ) {
		sql = insertIntoValues(input);
	}
	// 无法匹配
	else {
		sql.setType(SQL_ERROR);
	}

	return sql;
}

/**
 * @brief  测试select语句
 * @author tgmerge
 * 要支持的语句
 * select a from xxx
 * select a, b from xxx
 * select * from xxx
 * where a > 10 and b < 10 ;
 */
SqlCommand Interpreter::selectClause(string& str) {
	// 现在的语句示例 select colName, colName from tableName where condcol condOp condValue and condcol condop condvalue, etc
	SqlCommand sql;
	string tableName;
	string temp;

	str = delFirstWord(str, " ");	// 删除"select"

	// 处理colName
	for( temp = firstWord(str, " ,"); !(temp == "from"); temp = firstWord(str, " ,") ) {
		// TODO: 验证col的存在性？
		sql.pushColNameVector( temp );
		str = delFirstWord(str, " ,");
	}

	// 处理tableName
	str = delFirstWord(str, " ");	// 删除from
	tableName = firstWord(str, " ");
	str = delFirstWord(str, " ");   // 删除tablename

	// 处理条件, 如果有where...
	if( firstWord(str, " ") == "where" ) {
		str = delFirstWord(str, " "); // 删除where
		for( temp = firstWord(str, " "); !(temp == ";"); temp = firstWord(str, " ") ) {
			if(temp == "and") {
				str = delFirstWord(str, " ");
			}
			sql.pushCondLeftVector(firstWord(str, " ")); // 比较的属性名
			str = delFirstWord(str, " ");
			sql.pushCondOpVector(firstWord(str, " "));   // 比较的操作符
			str = delFirstWord(str, " ");
			sql.pushCondRightVector(firstWord(str, " "));// 比较的数值
			str = delFirstWord(str, " ");
		}
	}

	sql.setType(SQL_SELECT);
	sql.setTableName(tableName);

	return sql;
}

/**
 * @brief  测试delete from语句
 * @author tgmerge
 * 要支持的语句
 * delete from tableX
 * delete from tableX where a > 10 and b < 10 ;
 * TODO: 验证tableX存在性
 */
SqlCommand Interpreter::deleteFromWhere(string& str) {
	// 现在的语句示例 delete from X where a > 10 and b < 10 ;
	SqlCommand sql;
	string tableName;
	string temp;

	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");	// 删除"delete from"
	// TODO: delete后面不是from的情况

	// 处理tableName
	tableName = firstWord(str, " ");
	str = delFirstWord(str, " ");
	// TODO: 验证tableName的存在性

	// 如果后面有where
	if( firstWord(str, " ") == "where" ) {
		str = delFirstWord(str, " "); // 删除where
		for( temp = firstWord(str, " "); !(temp == ";"); temp = firstWord(str, " ") ) {
			if(temp == "and") {
				str = delFirstWord(str, " ");
			}
			sql.pushCondLeftVector(firstWord(str, " ")); // 比较的属性名
			str = delFirstWord(str, " ");
			sql.pushCondOpVector(firstWord(str, " "));   // 比较的操作符
			str = delFirstWord(str, " ");
			sql.pushCondRightVector(firstWord(str, " "));// 比较的数值
			str = delFirstWord(str, " ");
		}
	}

	sql.setType(SQL_DELETE);
	sql.setTableName(tableName);

	return sql;
}

/**
 * @brief  测试insert语句
 * @author tgmerge
 * 要支持的语句
 * insert into tableA values( 10, '100' );
 * TODO: 验证tableX存在性, 验证各列类型
 */
SqlCommand Interpreter::insertIntoValues(string& str) {
	SqlCommand sql;
	string tableName;
	string temp;

	str = delFirstWord(str, " ");
	str = delFirstWord(str, " ");	// 删除"insert into"
	// TODO: insert后面不是into

	// 处理tableName;
	tableName = firstWord(str, " ");
	str = delFirstWord(str, " ");

	str = delFirstWord(str, " (");	// 删除"values ("

	// 处理值
	while( !(firstWord(str, " )") == ";") ) {
		sql.pushColValueVector( firstWord(str, " ),") );
		str = delFirstWord(str, " ),");
	}

	sql.setType(SQL_INSERT_INTO);
	sql.setTableName(tableName);

	return sql;
}