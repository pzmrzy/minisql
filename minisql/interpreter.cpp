#include "interpreter.h"

using namespace std;

Interpreter::Interpreter() {
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

/**
 * @brief  解析命令，返回一个SqlCommand对象供API使用
 * @author tgmerge
 * @param  string     要解析的字符串
 * @return SqlCommand 解析完成的命令对象
 */
SqlCommand Interpreter::getExpression() {
	// 要返回的内部命令对象
	SqlCommand command();


}