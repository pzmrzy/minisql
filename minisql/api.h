#ifndef _API_H_
#define _API_H_
#include"sqlcommand.h"
class api
{
public:
	api(void);
	api(int t, SqlCommand& c):type(t), cmd(c){

	}
	virtual ~api(void);
private:
	int type;
	SqlCommand cmd;
};

#endif