// StatKeyTable.cpp: interface for the StatKeyTable class.
//
//////////////////////////////////////////////////////////////////////
//**************************************
//统计表.cpps文件
//
//
//20051009
//**************************************
#include "stdafx.h"
// #include "..\\include\\d4all.hpp"
#include "IpQueryLib.h"
#include "StatKey.h"
#include "direct.h"
#include <sys/stat.h>
#include "StatAgent.h"

#define TABLE_DIR     v_WorkPath+"\\statdata\\"
#define FIELDCOUNT	  8
// 
// FIELD4INFO agentkey_fields[] =			// 引擎关键字的结构
// {
// 	{"SEARCHID",	r4str, 5, 0},	// ID 组合关键字时，拆分关键字匹配标识
// 	{"KEY",			r4str, 50, 0},	// 关键字
// 	{"URL",			r4str, 50, 0},	// 域名
// 	{"SIP",			r4str, 16, 0},	// 原IP
// 	{"SIPADDR",		r4str, 100, 0},	// 客户端IP地区
// 	{"DIP",			r4str, 16, 0},	// 服务器IP
// 	{"DIPADDR",		r4str, 100, 0},	// 服务器IP地区
// 	{"REC_TIME",	r4str, 20, 0},	// 事件发生时间
// 	{ 0,0,0,0 }
// };
// 
// TAG4INFO agentkey_tag[]=				// 引擎关键字表表索引
// {
// 	{"KEY",		"KEY", 0, 0, r4descending},	
// 	{0, 0, 0, 0, 0}
// };
// 
// StatAgentTable::StatAgentTable()
// {
// 
// }
// 
// StatAgentTable::~StatAgentTable()
// {
// 	
// }
// extern BOOL CheckDbfStruct(const char *dbfFile, FIELD4INFO *pfieldinfo);
// extern BOOL UpdateDbfData(const char *oldName, const char *newDbf);
// 
// BOOL StatAgentTable::CreateTable(const char *dbfFile)
// {
// 	CString dbfname,m_dbfFile,m_cdxFile;
// 
// 	if(_access(TABLE_DIR,0) !=0)
// 	{
// 		CreateDir(TABLE_DIR);
// 	}
// 
// 	if(dbfFile == NULL)
// 	{
// 		dbfname=GetTableName(FILETYPE_DBF);
// 	}
// 	else
// 		dbfname.Format("%s%s",TABLE_DIR,dbfFile);
// 	
// 
// 	m_dbfFile = m_cdxFile = dbfname;
// 	m_cdxFile.Replace("dbf","cdx");
// 
// 	if(_access(m_dbfFile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,m_dbfFile,agentkey_fields,agentkey_tag);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",m_dbfFile);
// 			if(_access(m_cdxFile,0) ==0)
// 				remove(m_cdxFile);
// 			if(_access(m_dbfFile,0) ==0)
// 				remove(m_dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 	else	//检测结构是否变化
// 	{
// 		if(!CheckDbfStruct(dbfname, agentkey_fields))
// 		{		
// 			WriteLog("dce", "检查到表 %s 结构不符", dbfname);
// 			CString strNewCdx = m_cdxFile;
// 			strNewCdx.Replace(".cdx", "bak.cdx");
// 			CString strNewDbf = dbfname;
// 			strNewDbf.Replace(".dbf", "bak.dbf");
// 			remove(strNewDbf);
// 			remove(strNewCdx);
// 			
// 			Code4 cb;
// 			cb.autoOpen	= 0;	// 不自动打开索引文件
// 			Data4 data;
// 			int rc = data.create(cb, strNewDbf,agentkey_fields,agentkey_tag);
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce", "创建文件失败:%s", strNewDbf);
// 				if(_access(strNewCdx, 0) == 0)
// 					remove(strNewCdx);
// 				return FALSE;
// 			}
// 			cb.closeAll();
// 			cb.initUndo();
// 			
// 			if(UpdateDbfData(dbfname, strNewDbf))
// 			{
// 				remove(dbfname);
// 				remove(m_cdxFile);
// 				remove(strNewCdx);
// 				rename(strNewDbf, dbfname);
// 				WriteLog("dce", "成功恢复表 %s 中的老数据", dbfname);
// 			}
// 		}
// 	}
// 	return TRUE;
// }
// 
// CString StatAgentTable::AddRecord(CStringArray valuearr)
// {
// 	try{
// 			CString dbfname,cdxname;
// 			CTableAccess ac_table;
// 			Code4 code;
// 			code.autoOpen = 0;
// 			code.lockAttempts = LOCKTRYTIMES;
// 			Data4 data;
// 			ac_table.SetCode4(&code, &data);
// 			dbfname=GetTableName(FILETYPE_DBF);
// 			cdxname = dbfname;
// 			cdxname.Replace(".dbf",".cdx");
// 
// 			if(!OpenDbfTable(code, data,dbfname))
// 			{
// 				CloseTable(&code, &data, NULL, &ac_table);
// 				return "打开表失败";
// 			}
// 	
// 			Index4 index;
// //			if(!OpenIndex(data, index,statkey_tag,cdxname))
// //			{
// //				CloseTable(&code, &data, &index, &ac_table);
// //				return "打开索引文件失败";
// //			}
// //			ac_table.SetIndex4(&index);
// 	
// 			
// 			// 关联字段
// 			Field4	fields[FIELDCOUNT];
// 			static LPCTSTR FIELDSNAME[FIELDCOUNT] =
// 			{
// 				_T("SEARCHID"),		_T("KEY"),		_T("URL"),		
// 				_T("SIP"),			_T("SIPADDR"),	_T("DIP"),	
// 				_T("DIPADDR"),		_T("REC_TIME")
// 			};
// 			
// 			for (int i = 0; i < FIELDCOUNT; i++)
// 			{
// 				if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 				{
// 					code.closeAll();
// 					code.initUndo();
// 					WriteLog("dce", "关联字段失败!", FIELDSNAME[i]);
// 					return "失败";
// 				}
// 			}
// 
// 			data.appendStart(0);
// 			for (i = 0; i < FIELDCOUNT; i++)
// 			{
// 				if(fields[i].field != NULL)
// 				{
// 					fields[i].assign(valuearr.GetAt(i));
// 				}
// 			}
// 	
// 			if(data.append() == r4success)
// 			{
// 				CloseTable(&code, &data, &index, &ac_table);
// 				return "成功";
// 			}
// 			else
// 			{
// 				CloseTable(&code, &data, &index, &ac_table);
// 				return "失败";
// 			}
// 	}catch (...) {
// 		WriteLog("dce"," StatKeyTable::AddStatRecord,catch error");
// 		return "失败";
// 	}
// }
// 
// CString StatAgentTable::GetTableName(int WantType)
// {
// 	char filename[MAX_PATH];
// 	char daytime[20];
// 	CString dbfname;
// 	time_t ttNow = time(0);
// 	struct tm *tmNow = localtime(&ttNow);
// 	strftime(daytime, sizeof(daytime), "%Y%m%d", tmNow);				
// 
// 	switch(WantType)
// 	{	
// 		case FILETYPE_DBF:
// 			strftime(filename, sizeof(filename), "%Y%m.dbf", tmNow);
// 		case FILETYPE_CDX:
// 			strftime(filename, sizeof(filename), "%Y%m.cdx", tmNow);	
// 	}
// 
// 	dbfname.Format("%s%s\\%s",TABLE_DIR,daytime,filename);	
// 
// 	return dbfname;
/*}*/