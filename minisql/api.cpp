#include "api.h"
#include <iostream>
using namespace std;
#include "catalog.h"
#include "interpreter.h"
#include "minisql.h"
#include "sqlcommand.h"
#include "record.h"
api::api(void)
{
}

api::~api(void)
{
}

api::api(int t, SqlCommand& c){
	type =t;
	sql = c;
	
	string succ = "Operation Succeeds";
	extern string Wdbname;
	if (Wdbname == ""){
		if (type == SQL_CREATE_DATABASE || type == SQL_USE)
			Wdbname = c.getDatabaseName();
		else{
			cout<<"Please Use database"<<endl;
			return;
		}
	}else
		sql.setDatabaseName(Wdbname);
	c.writelog();
	catalog CL;
	record RE;
        switch (t){
                case ( SQL_CREATE_DATABASE ):{

					sql.setDatabaseName(Wdbname);
					cataInfo = CL.creat_Database(sql);//调用catalog类的creat_Database函数
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                            break;
                        }
                        //成功，输出成功提示
                        else cout<<succ<<endl;
                        break;
                }
                case ( SQL_CREATE_TABLE    ):{
                       cataInfo = CL.creat_Table(sql);//调用catalog函类的creat_table函数
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                             break;
                        }
                        //成功
                        else{
                            Table=cataInfo.gettable();
                           // indexInfo=index::create_index(sql,Table);//根据table信息，是否有主键，如果有主键就按create_index_on 主键;
                            //若建立主键的索引失败
                            //if (!indexInfo.succ) {
                              //  indexInfo.print();//输出失败原因
                                //SqlCommand sql1;//构造新指令，用于撤销刚刚建立的table
                                //sql1.type=SQL_DROP_TABLE;
                                //sql1.tableName=sql.getTableName();
                                //sql1.databaseName=sql.getDatabaseName();
                                cataInfo = CL.drop_Table(sql);//rollback，撤销table
                               // break;
                            //}
                            //else cout<<succ<<endl;//成功，提示成功信息
                        }
                        break;
                }
                case ( SQL_CREATE_INDEX    ):{//???有什么区别吗？
                        break;
                }
                case ( SQL_CREATE_INDEX_ON ):{
                        //???增加一个返回Table信息的函数,确认table存在吗？列名存在吗？index有了没有？若操作成功则需要改写Table信息并存入底层文件，否则给出失败原因
                        //cataInfo=find_Table(sql,Table,0);//0表示create index
						cataInfo = CL.creat_Index(sql);//调用catalog类的creat_Index函数，用于判断table是否存在，属性是否存在，有无index，返回table信息。
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
                             cataInfo.print();
                             break;
                        }
                        //成功，调用index的create_index_on函数，用于建立其他属性的索引
                        else{
							Table = cataInfo.gettable();
                            //indexInfo=index::create_index_on(sql,Table);
                            //失败
                            //if (!indexInfo.succ){
                                //indexInfo.print();
                              //  break;
                            }
                            //成功
                            //else{
                                //更改table信息
                                string attrname=sql.getcolName();
                                attrList=Table.attrList;
                                int size=attrList.size();
                                for (int i=0;i<size;i++){
                                    if (attrList.at(i).name.compare(attrname))
                                        { attrList.at(i).ID=true; break;}
                              //  }
                                Table.attrList=attrList;
                                CL.change_Table(Table);//调用catalog类的change_Tavble函数，回写入物理层
                                //indexInfo.print();
                            //}
                        }
                        break;
                }
                case ( SQL_DROP_DATABASE   ):{
                        cataInfo = CL.drop_Database(sql);//调用catalog类的drop_Database函数
                        //失败，输出失败原因
                        if (!cataInfo.getsucc())
							cataInfo.print();
                        //成功，输出成功信息
                        else cout<<succ<<endl;
                        break;
                }
                case ( SQL_DROP_TABLE      ):{
                        cataInfo = CL.drop_Table(sql);//调用catalog类的drop_Table函数
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()) 
						{ cataInfo.print();}
                        //成功，删除相关的index，输出成功信息
                        else{
                            Table=cataInfo.gettable();
                            //indexInfo=index::drop_index_all(sql,Table);//将table有关的所有index都删除
                            //？？？删除index失败，则没想好怎么办；
                            //if (!indexInfo.succ){
                            //    indexInfo.print();
                            //    break;
                            //}
                            //删除成功，输出成功提示
                            //else cout<<succ<<endl;
                        }
                        break;
                }
               /* case ( SQL_DROP_INDEX      ):{
                        cataInfo=CL.drop_Index(sql);//调用catalog类的drop_Index函数，获得table信息
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                            break;
                        }
                        //成功，输出成功提示，删除index
                        else{
                            Table=cataInfo.gettable();
                           // indexInfo=index::drop_index(sql,Table);//根据指令的index 删除index
                            //失败，输出失败原因
                            //if (!indexInfo.succ){
                             //   indexInfo.print();
                                break;
                            }
                            //成功，更改table信息，输出成功提示
                            //else{
                                //更改table信息
                                string attrname=sql.getcolName();//？？？该类sql语句中无属性名，应该从indexInfo中获取
                                attrList=Table.attrList;
                                int size=attrList.size();
                                for (int i=0;i<size;i++){
                                    if (attrList.at(i).name.compare(attrname))
                                        { attrList.at(i).ID=false; break;}
                                ///}
                                Table.attrList=attrList;
                                CL.change_Table(Table);//调用catalog类的change_Tavble函数，回写入物理层
                                //indexInfo.print();
                            //}
                        }
                        break;
                }*/

                /*case ( SQL_SELECT          ):{
                        //确认table存在吗？确认列名是对的嘛？
                        cataInfo = CL.select_Rec(sql);
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                            break;
                        }
                        //成功，根据有无索引有无where调用不同类的selec_Rec函数
                        else{
                            Table=cataInfo.gettable();
                            //无where条件
                            if (sql.condLeftVector.size()==0){
                                recoInfo=RE.Select_Rec(sql,Table);
                                //失败，则输出失败信息（包括结果为空的信息）
								if (!recoInfo.getsucc()){
									recoInfo.print();
									break;
								}
                                //else RE.print_results(recoInfo.results,-1);//输出结果
                            }
                            //有where条件
                            else{
                                bool f=false;
                                attrList=Table.attrList;
                                int i;
                                int asize=attrList.size();
                                indexList.clear();
                                for (i=0;i<asize;i++){
                                    if (attrList.at(i).ID){indexList.push_back(attrList.at(i).name);f=true;}//???
                                }
                                //有索引
                                if (f){
                                    //indexInfo=index::Select_Rec(sql,Table,indexList);//不知道indexList这种形式是不是会方便查找，可改
                                    //if (!indexInfo.succ){cout<<indexInfo.message<<endl;break;}
                                    //else print_results(indexInfo.results,-1);//???
                                }
                                //无索引
                                else {
                                    recoInfo=RE.Select_Rec(sql,Table);
                                    if (!recoInfo.getsucc()){
										recoInfo.print();
										break;
									}
                                    //else print_results(recoInfo.results,-1);//???
                                }
                            }
                        }
                        break;
                }*/
                /*case ( SQL_INSERT_INTO     ):{
                        //确认table存在吗，值的个数是否是对的
                        cataInfo = CL.insert_Rec(sql);
                        //失败，输出失败原因
                        if (!cataInfo.getsucc()){
                             cataInfo.print();
                             break;
                        }
                        //成功，按分类调用不同模块的insert_Rec函数
                        else{
                            Table=cataInfo.gettable();
                            bool f=false;
                            attrList=Table.attrList;
                            int i;
                            int asize=attrList.size();
                            indexList.clear();
                            for (i=0;i<asize;i++){
                                if (attrList.at(i).ID){indexList.push_back(attrList.at(i).name);f=true;}//???
                            }
                            //有索引
                            if (f){
                                //indexInfo=index::Insert_Rec(sql,Table,indexList);//不知道indexList这种形式是不是会方便查找，可改
                                //if (!indexInfo.succ){cout<<indexInfo.message<<endl;break;}
                                //else {
                                    //Table.attrNum+=indexInfo.number;//更改Table信息
                                    CL.change_Table(Table); //回写
                                    //cout<<indexInfo.message<<endl;
                                //}
                            }
                            //无索引
                            else {
                                recoInfo=RE.Insert_Rec(sql,Table);
                                if (!recoInfo.getsucc()){
									recoInfo.print();
									break;
								}
                                else {
                                    Table.attrNum+=recoInfo.nummes();
                                    CL.change_Table(Table);
                                    recoInfo.print();
                                }
                            }
                        }
                        break;
                }*/
               /* case ( SQL_DELETE          ):{
                        //确认table存在吗？确认列名是对的嘛？
                        cataInfo = CL.delete_Rec(sql);
                        //失败，则输出失败原因
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                             break;
                        }
                        //成功，按照分类调用不同模块的Delete_Rec函数
                        else{
                            Table=cataInfo.gettable();
                            //无where条件
                            if (sql.condLeftVector.size()==0){
                                recoInfo=RE.Delete_Rec(sql,Table);
                                if (!recoInfo.getsucc()){
									recoInfo.print();
									break;
								}
                                else {
									Table.attrNum-=recoInfo.nummes();//更改table信息
                                    CL.change_Table(Table);//回写
									recoInfo.print();
								}
                            }
                            //有where条件
                            else{
                                bool f=false;
                                attrList=Table.attrList;
                                int i;
                                int asize=attrList.size();
                                indexList.clear();
                                for (i=0;i<asize;i++){
                                    if (attrList.at(i).ID){indexList.push_back(attrList.at(i).name);f=true;}
                                }
                                //有索引
                                if (f){
                                    //indexInfo=index::Delete_Rec(sql,Table,indexList);//不知道indexList这种形式是不是会方便查找，可改
                                    //if (!indexInfo.succ){cout<<indexInfo.message<<endl;break;}
                                    //else {
                                        //Table.attrNum-=recoInfo.number;//更改table信息
                                        CL.change_Table(Table);
                                        //cout<<indexInfo.message<<endl;
                                    //}
                                }
                                //无索引
                                else {
                                    recoInfo = RE.Select_Rec(sql,Table);
                                    if (!recoInfo.getsucc()){recoInfo.print(); break;}
                                    else {
                                        Table.attrNum-=recoInfo.nummes();//更改table信息
                                        CL.change_Table(Table);
                                        recoInfo.print();
                                    }
                                }
                            }
                        }
                        break;
                }*/
                case ( SQL_USE             ):{
					cataInfo = CL.use_Database(sql);
					if (!cataInfo.getsucc())
						cataInfo.print();
					else{
						Wdbname = sql.getDatabaseName();
						cout<<succ<<endl;
					}
                        break;
                }
                case ( SQL_QUIT            ):{
					exit(0);
                        break;
                }
                case ( SQL_ERROR           ):{
					cout<<"Syntax Error"<<endl;
                        break;
                }
				case (SQL_SHOW_DATABASE):{
					cataInfo = CL.creat_Database(sql);
                        if (!cataInfo.getsucc()){
							cataInfo.print();
                            break;
                        }
				}

        }
}
