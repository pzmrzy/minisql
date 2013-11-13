#ifndef _API_H_
#define _API_H_

#include "minisql.h"
#include "sqlcommand.h"
#include "catalog.h"
#include <vector>
using namespace std;

class api
{
public:
		api();
        api(int t, SqlCommand& c);
        virtual ~api(void);
        catainfo cataInfo;
        recoinfo recoInfo;
        //indexinfo indexInfo;
        table Table;
        attribute Attribute;
        vector<attribute> attrList;
        vector<string> indexList;

private:
        int type;
        SqlCommand sql;
};



#endif

