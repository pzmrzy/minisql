#include "api.h"
#include "sqlcommand.h"
#include "minisql.h"

api::api(void)
{
}


api::~api(void)
{
}

api::api(int t, SqlCommand& c):type(t), cmd(c){
	switch (t){
		case ( SQL_CREATE_DATABASE ):{

			break;
		}
		case ( SQL_CREATE_TABLE    ):{

			break;
		}
		case ( SQL_CREATE_INDEX    ):{

			break;
		}
		case ( SQL_CREATE_INDEX_ON ):{

			break;
		}
		case ( SQL_DROP_DATABASE   ):{

			break;
		}
		case ( SQL_DROP_TABLE      ):{

			break;
		}
		case ( SQL_DROP_INDEX      ):{

			break;
		}
		case ( SQL_SELECT          ):{

			break;
		}
		case ( SQL_INSERT_INTO     ):{

			break;
		}
		case ( SQL_DELETE          ):{

			break;
		}
		case ( SQL_USE             ):{

			break;
		}
		case ( SQL_QUIT            ):{

			break;
		}
		case ( SQL_ERROR           ):{

			break;
		}
	}
}