// StatKeyTable.cpp: interface for the StatKeyTable class.
//
//////////////////////////////////////////////////////////////////////
//**************************************
//ͳ�Ʊ�.cpps�ļ�
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
// FIELD4INFO agentkey_fields[] =			// ����ؼ��ֵĽṹ
// {
// 	{"SEARCHID",	r4str, 5, 0},	// ID ��Ϲؼ���ʱ����ֹؼ���ƥ���ʶ
// 	{"KEY",			r4str, 50, 0},	// �ؼ���
// 	{"URL",			r4str, 50, 0},	// ����
// 	{"SIP",			r4str, 16, 0},	// ԭIP
// 	{"SIPADDR",		r4str, 100, 0},	// �ͻ���IP����
// 	{"DIP",			r4str, 16, 0},	// ������IP
// 	{"DIPADDR",		r4str, 100, 0},	// ������IP����
// 	{"REC_TIME",	r4str, 20, 0},	// �¼�����ʱ��
// 	{ 0,0,0,0 }
// };
// 
// TAG4INFO agentkey_tag[]=				// ����ؼ��ֱ������
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
// 		cb.autoOpen = 0; //���Զ�������
// 		Data4 data;
// 		int rc = data.create(cb,m_dbfFile,agentkey_fields,agentkey_tag);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","�������ݿ�%sʧ��.",m_dbfFile);
// 			if(_access(m_cdxFile,0) ==0)
// 				remove(m_cdxFile);
// 			if(_access(m_dbfFile,0) ==0)
// 				remove(m_dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 	else	//���ṹ�Ƿ�仯
// 	{
// 		if(!CheckDbfStruct(dbfname, agentkey_fields))
// 		{		
// 			WriteLog("dce", "��鵽�� %s �ṹ����", dbfname);
// 			CString strNewCdx = m_cdxFile;
// 			strNewCdx.Replace(".cdx", "bak.cdx");
// 			CString strNewDbf = dbfname;
// 			strNewDbf.Replace(".dbf", "bak.dbf");
// 			remove(strNewDbf);
// 			remove(strNewCdx);
// 			
// 			Code4 cb;
// 			cb.autoOpen	= 0;	// ���Զ��������ļ�
// 			Data4 data;
// 			int rc = data.create(cb, strNewDbf,agentkey_fields,agentkey_tag);
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce", "�����ļ�ʧ��:%s", strNewDbf);
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
// 				WriteLog("dce", "�ɹ��ָ��� %s �е�������", dbfname);
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
// 				return "�򿪱�ʧ��";
// 			}
// 	
// 			Index4 index;
// //			if(!OpenIndex(data, index,statkey_tag,cdxname))
// //			{
// //				CloseTable(&code, &data, &index, &ac_table);
// //				return "�������ļ�ʧ��";
// //			}
// //			ac_table.SetIndex4(&index);
// 	
// 			
// 			// �����ֶ�
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
// 					WriteLog("dce", "�����ֶ�ʧ��!", FIELDSNAME[i]);
// 					return "ʧ��";
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
// 				return "�ɹ�";
// 			}
// 			else
// 			{
// 				CloseTable(&code, &data, &index, &ac_table);
// 				return "ʧ��";
// 			}
// 	}catch (...) {
// 		WriteLog("dce"," StatKeyTable::AddStatRecord,catch error");
// 		return "ʧ��";
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