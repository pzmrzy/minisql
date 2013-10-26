#include "catalog.h"

catalog::catalog(void)
{
}

catalog::~catalog(void)
{
}

void catalog::init(string& dbname){
	fstream fout;
	fout.open(dbname + ".list",ios::out | ios::binary);

	tableNum t(0);
	writeHead(fout, t);
}

void catalog::writeHead(fstream& f, tableNum& t)
{
	f.write((char *)(t.num), sizeof(int));
    f.flush();
}

void catalog::readTableNum(fstream& f, tableNum& t){
	f.read((char *)&(t.num), sizeof(int));
}

void catalog::readTable(fstream& f, table& t){
	char buf[MAX_CHAR_LENGTH];
	f.read((char *)buf, MAX_CHAR_LENGTH);
	f.read((char *)&(t.attrNum), sizeof(int));
	f.read((char *)&(t.recLength), sizeof(int));
	t.name = buf;
	for (int i = 0; i < t.attrNum; i++)
		readAttr(f, t.attrList[i]);
}

void catalog::readAttr(fstream&f, attribute& attr){
	char buf[MAX_CHAR_LENGTH];
	f.read((char *)buf, MAX_CHAR_LENGTH);
	f.read((char *)&(attr.datatype), sizeof(int));
	f.read((char *)&(attr.length), sizeof(int));
	f.read((char *)&(attr.PK), sizeof(bool));
	f.read((char *)&(attr.UN), sizeof(bool));
	f.read((char *)&(attr.NN), sizeof(bool));
	attr.name = buf;
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

	fstream f;
	f.open(dbname + ".list", ios::in | ios::out | ios::beg);
	tableNum tnum;
	f.seekg(0, ios::beg);
	readTableNum(f, tnum);
	tnum.num++;
	f.seekp(0, ios::beg);
	writeHead(f, tnum);
}

bool catalog::exist_Table(string& dbname, string& tname){
	bool flag = false;
	fstream f;
	f.open(dbname + ".list", ios::in | ios::binary);

	tableNum tnum;
	table t;
	readTableNum(f, tnum);
	int Maxn = tnum.num;

	for (int i=0; i<Maxn; i++){
		readTable(f, t);
		if (t.name == tname){
			flag = true;
			break;
		}
	}
	f.close();
	return flag;
}

