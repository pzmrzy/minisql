#include "catalog.h"


catalog::catalog(void)
{
}


catalog::~catalog(void)
{
}

bool catalog::init(){
	fstream fout;
	fout.open(CATA,ios::out | ios::binary);

	tableNum t(0);
	return writeHead(fout, t);
}

bool catalog::writeHead(fstream& f, tableNum& t)
{
	f.write((char *)(t.getnum()), sizeof(int));
    f.flush();
}

catainfo catalog::creat_Table(SqlCommand& cmd){
	string dbname = cmd.getDatabaseName();
	bool existdb = exist_Database(dbname);
	if (!existdb)
		return catainfo(false, "Database " + dbname + " Do Not Exist!");

	string tname = cmd.getTableName();
	bool existt = exist_Table(dbname, tname);
	if (existt)
		return catainfo(false, "Table " + tname + " Has Already Exist!");


}