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
//#include "..\\include\\d4all.hpp"
#include "IpQueryLib.h"
#include "StatKey.h"
#include "direct.h"
#include <sys/stat.h>
#include "WebHisTable.h"

#define MAXJIYDBF         3
// #define QQPASS_TEMPDIR    "x:\\xxx\\statdata\\qq\\temp\\"
// #define QQPASS_DIR		  "x:\\xxx\\statdata\\qq\\"
// #define QQPASSMOBAN_DIR	  "x:\\xxx\\statdata\\qq\\moban\\"
// #define QQPASSMOBAN_FILE  "x:\\xxx\\statdata\\qq\\moban\\moban.dbf"
// 
// #define TAOBAOPASS_TEMPDIR    v_StatPath+"\\taobao\\temp\\"
// #define TAOBAOPASS_DIR		  v_StatPath+"\\taobao\\"
// #define TAOBAOPASSMOBAN_DIR	  v_StatPath+"\\taobao\\moban\\"
// #define TAOBAOPASSMOBAN_FILE  v_StatPath+"\\taobao\\moban\\moban.dbf"
// 
// #define ERROR_TEMPDIR    "x:\\xxx\\statdata\\password\\temp\\"
// #define ERROR_DIR		  "x:\\xxx\\statdata\\password\\"
// #define ERRORMOBAN_DIR	  "x:\\xxx\\statdata\\password\\moban\\"
// #define ERRORMOBAN_FILE  "x:\\xxx\\statdata\\password\\moban\\moban.dbf"
// 
// #define FIELDCOUNT	           8
// #define PREKEYFIELDCOUNT	  11
// #define WARNALARMFIELDCOUNT	  17
// #define CASEALARMFIELDCOUNT   18
// 
// 
// FIELD4INFO statkey_fields[] =			// 引擎关键字的结构
// {
// 	{"REC_TIME",	r4str, 20, 0},	// 事件发生时间
// 	{"SIP",			r4str, 15, 0},	// 客户端IP
// 	{"DIP",			r4str, 15, 0},	// 服务器IP	
// 	{"SEARCHID",	r4str, 15, 0},	// ID 组合关键字时，拆分关键字匹配标识
// 	{"KEY",			r4str, 50, 0},	// 关键字
// 	{"URL",			r4str, 50, 0},	// 域名
// 	{"SIPADDR",		r4str, 100, 0},	// 客户端IP地区
// 	{"DIPADDR",		r4str, 100, 0},	// 服务器IP地区
// 	{ 0,0,0,0 }
// };
// TAG4INFO statkey_tag[]=				// 引擎关键字表表索引
// {
// 	{"KEY",	"KEY", 0, 0, r4descending},	
// 	{0,		0,	   0, 0, 0}
// };
// 
// FIELD4INFO prekey_fields[] =			// 预设关键字的结构
// {
// 	{"REC_TIME",	r4str, 20, 0},	// 事件发生时间
// 	{"SIP",			r4str, 15, 0},	// 客户端IP
// 	{"DIP",			r4str, 15, 0},	// 服务器IP	
// 	{"SIPPORT",		r4str, 5, 0},	// 客户端IP端口	
// 	{"DIPPORT",		r4str, 5, 0},	// 服务器IP端口		
// 	{"SIPADDR",		r4str, 100, 0},	// 客户端IP地区
// 	{"DIPADDR",		r4str, 100, 0},	// 服务器IP地区	
// 	{"KEY",			r4str, 50, 0},	// 关键字
// 	{"PRO",			r4str, 20, 0},	// 协议 
// 	{"URL",			r4str, 100, 0},	// 域名
// 	{"HOST",		r4str, 100, 0},	// 客户端IP地区
// 	
// 	{ 0,0,0,0 }
// };
// TAG4INFO prekey_tag[]=				// 预设关键字表表索引
// {
// 	{"KEY",		"KEY", 0, 0, r4descending},	
// 	{0, 0, 0, 0, 0}
// };
// 
// FIELD4INFO Domain_fields[] =				
// {
// 	{"TMSECT",		r4str, 12, 0},	// 开始时间 2006022317
// 	{"DOMAIN",		r4str, 60, 0},	// 域名
// 	{"TOPDOMAIN",	r4str, 60, 0},	// 顶级域名
// 	{"SIP",			r4str, 16, 0},	// 服务器IP
// 	{"ISP",			r4str, 60, 0},	// 运营商
// 	{"PRO",			r4str, 20, 0},	// 协议
// 	{"SUBPRO",		r4str, 20, 0},	// 子协议
// 	{"PORT",		r4str, 5,  0},	// 端口
// 	{"ClassType",	r4str, 20, 0},	// 类型,是游戏类，blog类 等
// 	{"TypeState",	r4str, 20, 0},	// 明确标识
// 	{"intestate",	r4str, 20, 0},	// 交互状态(可交互/不可交互)
// 	{"NUM",			r4num, 10, 0},	// 次数
// 	{ 0,0,0,0 }
// };
// 
// TAG4INFO Domain_tag[]=			
// {	
// 	{0, 0, 0, 0, 0}
// }; 
// 
// FIELD4INFO IdcPort_fields[] =				
// {
// 	{"SIP",			r4str, 16, 0},	// 服务器IP
// 	{"PORT",		r4str, 5,  0},	// 子协议
// 	{"PRO",			r4str, 20, 0},	// 协议
// 	{ 0,0,0,0 }
// };
// 
// TAG4INFO IdcPort_tag[]=			
// {	
// 	{0, 0, 0, 0, 0}
// }; 
// 
// // FIELD4INFO qqpas_fields[] =	
// // {
// // 	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
// // 	{"QQID",		r4str, 20,  0},	// qqid
// // 	{"PASS",		r4str, 40, 0},	// 密码
// // 	{"MD5",			r4str, 40, 0},	// md5
// // 	{"SYSTYPE",		r4str, 20, 0},	// 系统类型
// // 	{"SRCDEV",		r4str, 50, 0},	// 设备
// // 	{"MARK",		r4str, 100, 0},	// 备注
// // 	{"MARK2",		r4str, 100, 0},	// 备注2
// // 	{"CAP_TIME",	r4str, 20, 0},	// 截获时间,格式: 2007-02-27 09:58:20
// // 	{"LUSE_TIME",	r4str, 20, 0},	// 最后使用时间,格式: 2007-02-27 09:58:47
// // 	{ 0,0,0,0 }
// // };
// FIELD4INFO qqpas_fields[] =    // 系统参数表结构
// {
//  {"REC_TIME", r4num, 10, 0}, // 记录同步标识(同步时使用)(time_t数据的串)
//  {"QQID",  r4str, 20,  0}, // qqid
//  {"PASS",  r4str, 40, 0}, // 密码
//  {"PASSTYPE",   r4str, 40, 0}, // 密码类型
//  {"SYSTYPE",  r4str, 20, 0}, // 系统类型
//  {"SRCDEV",  r4str, 50, 0}, // 设备
//  {"MARK",  r4str, 100, 0}, // 备注
//  {"MARK2",  r4str, 100, 0}, // 备注2
//  {"CAP_TIME", r4str, 20, 0}, // 截获时间,格式: 2007-02-27 09:58:20
//  {"LUSE_TIME", r4str, 20, 0}, // 最后使用时间,格式: 2007-02-27 09:58:47
//  { 0,0,0,0 }
// };
// 
// FIELD4INFO taobaopas_fields[] =    // 系统参数表结构
// {
// 	{"REC_TIME", r4num, 10, 0}, // 记录同步标识(同步时使用)(time_t数据的串)
// 	{"TAOBAOID",  r4str, 20,  0}, // taobaoid
// 	{"PASS",  r4str, 40, 0}, // 密码
// 	{"PASSTYPE",  r4str, 40, 0}, // 密码类型  0：明文密码； 1：一次MD5； 2：两次MD5
// 	{"SYSTYPE",  r4str, 20, 0}, // 系统类型
// 	{"SRCDEV",  r4str, 50, 0}, // 设备
// 	{"MARK",  r4str, 100, 0}, // 备注
// 	{"MARK2",  r4str, 100, 0}, // 备注2
// 	{"CAP_TIME", r4str, 20, 0}, // 截获时间,格式: 2007-02-27 09:58:20
// 	{"LUSE_TIME", r4str, 20, 0}, // 最后使用时间,格式: 2007-02-27 09:58:47
// 	{ 0,0,0,0 }
// };
// 
// FIELD4INFO error_fields[] =    // 错误的qq，淘宝表结构
// {
// 	{"ID",  r4str, 20,  0}, // taobaoid或qqid
// 	{"PASS",  r4str, 40, 0}, // 密码
// 	{"PASSTYPE",  r4str, 40, 0}, // 密码类型  0：明文密码； 1：一次MD5； 2：两次MD5
// 	{"PRO",  r4str, 20, 0}, // 协议类型 imsg_qq，imsg_taobao
// 	{"TYPE",  r4str, 10, 0}, // 正确的还是错误的 1正确 0 错误
// 	{ 0,0,0,0 }
// };
// 
// // FIELD4INFO taobaopas_fields[] =	
// // {
// // 	{"REC_TIME",	r4num, 10, 0},	// 记录同步标识(同步时使用)(time_t数据的串)
// // 	{"QQID",		r4str, 20,  0},	// qqid
// // 	{"PASS",		r4str, 40, 0},	// 密码
// // 	{"MD5",			r4str, 40, 0},	// md5
// // 	{"SYSTYPE",		r4str, 20, 0},	// 系统类型
// // 	{"SRCDEV",		r4str, 50, 0},	// 设备
// // 	{"MARK",		r4str, 100, 0},	// 备注
// // 	{"MARK2",		r4str, 100, 0},	// 备注2
// // 	{"CAP_TIME",	r4str, 20, 0},	// 截获时间,格式: 2007-02-27 09:58:20
// // 	{"LUSE_TIME",	r4str, 20, 0},	// 最后使用时间,格式: 2007-02-27 09:58:47
// // 	{ 0,0,0,0 }
// // };
// 
// StatKeyTable::StatKeyTable()
// {
// 
// }
// 
// StatKeyTable::~StatKeyTable()
// {
// 	
// }
// 
// BOOL StatKeyTable::CreateTable(const char *dbfFile)
// {
// 	CString dbfname,m_dbfFile,m_mobanFile,m_mobanpath;
// 	m_dbfFile ="x:\\xxx\\statdata\\statkey\\temp";
// 	m_mobanpath ="x:\\xxx\\statdata\\statkey\\moban";
// 	m_mobanFile ="x:\\xxx\\statdata\\statkey\\moban\\moban.dbf";
// 	if(_access(m_dbfFile,0) !=0)
// 		mkdir(m_dbfFile);
// 	if(_access(m_mobanpath,0) !=0)
// 		mkdir(m_mobanpath);
// 	if(_access(m_mobanFile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,m_mobanFile,statkey_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",m_mobanFile);
// 			if(_access(m_mobanFile,0) ==0)
// 				remove(m_mobanFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(_access(dbfFile,0) !=0)
// 	{
// 		if(!CopyFile(m_mobanFile,dbfFile,true))
// 		{
// 			WriteLog("dce", "拷贝模版文件%s到%s失败", m_mobanFile,dbfFile);
// 			Code4 cb;
// 			cb.autoOpen = 0; //不自动打开索引
// 			Data4 data;
// 			int rc = data.create(cb,dbfFile,statkey_fields);
// 			
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce","创建数据库%s失败.",dbfFile);
// 				if(_access(dbfFile,0) ==0)
// 					remove(dbfFile);
// 				return FALSE;
// 			}
// 			cb.closeAll();
// 			cb.initUndo();
// 			return TRUE;
// 		}
// 	}
// 		
// 	return TRUE;
// }
// 
// CString StatKeyTable::AddRecord(CStringArray* valuearr,CString hisfilename)
// {
// 	try
// 	{
// 		CString dbfname,cdxname;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 		dbfname=hisfilename;
// 		cdxname = dbfname;
// 		cdxname.Replace(".dbf",".cdx");
// 		// 
// 		if (_access("x:\\xxx\\statdata\\statkey\\temp",0)!=0)
// 		{
// 			if (_access("x:\\xxx\\statdata\\statkey",0)!=0)
// 			{
// 				_mkdir("x:\\xxx\\statdata\\statkey");
// 			}
// 			_mkdir("x:\\xxx\\statdata\\statkey\\temp");
// 		}
// 		
// 		if (_access(dbfname,0)!=0)
// 		{
// 			CreateTable(dbfname);
// 		}
// 		if(!OpenDbfTable(code, data,dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			WriteLog("dce","引擎关键字打开表失败");
// 			if (_access("x:\\xxx\\statdata\\statkey\\moban\\moban.dbf",0)==0)
// 				DeleteFile("x:\\xxx\\statdata\\statkey\\moban\\moban.dbf");
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			return "打开表失败";
// 		}
// 	
// 		Index4 index;		
// 		// 关联字段
// 		Field4	fields[FIELDCOUNT];
// 		static LPCTSTR FIELDSNAME[FIELDCOUNT] =
// 		{
// 			_T("REC_TIME"),	_T("SIP"),_T("DIP"),
// 			_T("SEARCHID"),	_T("KEY"),_T("URL"),	
// 			_T("SIPADDR"),_T("DIPADDR")
// 		};
// 
// 		for (int i = 0; i < FIELDCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "关联字段失败!", FIELDSNAME[i]);
// 				if (_access("x:\\xxx\\statdata\\statkey\\moban\\moban.dbf",0)==0)
// 					DeleteFile("x:\\xxx\\statdata\\statkey\\moban\\moban.dbf");
// 				if (_access(dbfname,0)==0)
// 					DeleteFile(dbfname);
// 				return "失败";
// 			}
// 		}
// 		int tmpn=valuearr->GetSize();
// 		for(i = 0;i<tmpn;i++)
// 		{
// 			CString tempstr=valuearr->GetAt(i);
// 			int h=tempstr.Find('\1');
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0;h>=0 && m<FIELDCOUNT;m++)
// 			{
// 				h=tempstr.Find('\1');
// 				if(fields[m].field != NULL)
// 				{
// 					if (m==6)
// 						fields[m].assign(k_IpQueryTable.QueryIpAddr(tempstr.Left(h)));
// 					else if (m==7)
// 						fields[m].assign(k_IpQueryTable.QueryIpAddr(tempstr));
// 					else
// 					{
// 						if (h<0)
// 							fields[m].assign(tempstr);
// 						else
// 							fields[m].assign(tempstr.Left(h));
// 					}
// 				}
// 
// 				tempstr=tempstr.Mid(h+1);
// 			}
// 			if(data.append() != r4success)
// 			{
// 				WriteLog("dce","增加信息失败");
// 				
// 				CloseTable(&code, &data, &index, &ac_table);
// 				return "失败";
// 			}
// 		}
// 		CloseTable(&code, &data, &index, &ac_table);
// 		return "ok";
// 			
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," StatKeyTable::AddStatRecord,catch error");
// 		return "失败";
// 	}
// }
// 
// CString StatKeyTable::GetTableName(int WantType)
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
// }
// 
// PreKeyTable::PreKeyTable()
// {
// 
// }
// 
// PreKeyTable::~PreKeyTable()
// {
// 	
// }
// 
// BOOL PreKeyTable::CreateTable(const char *dbfFile)
// {
// 	LPCTSTR path = _T("x:\\xxx\\statdata\\prekey\\temp");
// 	LPCTSTR mobanname = "x:\\xxx\\statdata\\prekey\\temp\\prekeymoban.dbv1";
// 	if(_access(path,0) !=0)
// 		CreateDir(path);
// 	
// 	if(_access(mobanname, 0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb, mobanname, prekey_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建模板库<prekey>失败");
// 			remove(mobanname);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(!CopyFile(mobanname ,dbfFile, TRUE))
// 	{
// 		WriteLog("dce", "拷贝模版文件<prekey>失败");
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb, dbfFile, prekey_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",dbfFile);
// 			remove(dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// 
// BOOL PreKeyTable::AddRecord(CStringArray& dataarr, LPCTSTR prekeyname)
// {
// 	try
// 	{
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 		CString dbfname;
// 		dbfname.Format("x:\\xxx\\statdata\\prekey\\temp\\%s", prekeyname);
// 		if (_access(dbfname, 0)!=0)
// 			CreateTable(dbfname);
// 
// 		if(!OpenDbfTable(code, data, dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			DeleteFile("x:\\xxx\\statdata\\prekey\\temp\\prekeymoban.dbv1");
// 			DeleteFile(dbfname);
// 			WriteLog("dce","预设关键字打开表失败");
// 			return FALSE;
// 		}
// 	
// 		// 关联字段
// 		Field4	fields[PREKEYFIELDCOUNT];
// 		static LPCTSTR FIELDSNAME[PREKEYFIELDCOUNT] =
// 		{
// 			_T("REC_TIME"),_T("SIP"),_T("DIP"),
// 			_T("SIPPORT"),_T("DIPPORT"),_T("SIPADDR"),
// 			_T("DIPADDR"),_T("KEY"),_T("PRO"),_T("URL"),_T("HOST")
// 		};
// 
// 		for (int i = 0; i < PREKEYFIELDCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				DeleteFile("x:\\xxx\\statdata\\prekey\\temp\\prekeymoban.dbv1");
// 				DeleteFile(dbfname);
// 				WriteLog("dce", "关联字段失败!", FIELDSNAME[i]);
// 				return FALSE;
// 			}
// 		}
// 		int tmpn = dataarr.GetSize();
// 		char *tp;
// 		for(i=0; i<tmpn; i++)
// 		{
// #ifdef		_DEBUG
// 			CString tempstr = dataarr.GetAt(i);
// #endif
// 			tp = dataarr[i].GetBuffer(0);
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0; tp!=NULL && m<PREKEYFIELDCOUNT; m++)
// 			{
// 				ASSERT(fields[m].field != NULL);
// 				char *p1 = strchr(tp, '\1');
// 				if (p1!=NULL)
// 					*p1++ = 0;
// 				
// 				if (m==5 || m==6)
// 					fields[m].assign(k_IpQueryTable.QueryIpAddr(tp));
// 				else
// 					fields[m].assign(tp);
// 				
// 				tp = p1;
// 			}
// 			data.append();
// 		}
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return TRUE;
// 			
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," PREKeyTable::AddStatRecord,catch error");
// 		return TRUE;
// 	}
// }
// 
// //////////////////////////////////////////////////////////////////////////
// //
// IdcDomainTable::IdcDomainTable()
// {
// 	m_tabletime = time(0);
// }
// 
// IdcDomainTable::~IdcDomainTable()
// {
// }
// 
// 
// BOOL IdcDomainTable::CreateIdcServerTable(LPCTSTR dbfname)
// {
// 	static LPCTSTR pTableTemple = _T("x:\\xxx\\statdata\\IdcServer\\temp\\idcserver.dbv3");
// 
// 	if (_access(dbfname, 0) == 0)
// 		return TRUE;
// 
// 	CreateDir("x:\\xxx\\statdata\\IdcServer\\temp");
// 	if(_access(pTableTemple,0) != 0)
// 	{
// 		// 创建模板
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb, pTableTemple, Domain_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",pTableTemple);
// 			DeleteFile(pTableTemple);
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	// 拷贝模板
// 	if(!CopyFile(pTableTemple, dbfname, FALSE))
// 	{
// 		DeleteFile(pTableTemple);
// 		if (_access(dbfname, 0)==0)
// 			DeleteFile(dbfname);
// 		WriteLog("dce", "拷贝模版文件%s到%s失败", pTableTemple, dbfname);
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,dbfname,Domain_fields);
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",dbfname);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// 
// BOOL IdcDomainTable::CreateIdcPortTable(LPCTSTR dbfname)
// {
// 	static LPCTSTR pTableTemple = _T("x:\\xxx\\statdata\\IdcPort\\temp\\idcPort.dbv1");
// 
// 	if (_access(dbfname, 0) == 0)
// 		return TRUE;
// 
// 	CreateDir("x:\\xxx\\statdata\\IdcPort\\temp");
// 	if(_access(pTableTemple,0) != 0)
// 	{
// 		// 创建模板
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb, pTableTemple, IdcPort_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",pTableTemple);
// 			DeleteFile(pTableTemple);
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 
// 	}
// 
// 	// 拷贝模板
// 	if(!CopyFile(pTableTemple, dbfname, FALSE))
// 	{
// 		DeleteFile(pTableTemple);
// 		if (_access(dbfname, 0)==0)
// 			DeleteFile(dbfname);
// 		WriteLog("dce", "拷贝模版文件%s到%s失败", pTableTemple, dbfname);
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,dbfname,Domain_fields);
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",dbfname);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// 
// void IdcDomainTable::ClearTemp()
// {
// 	return ;
// }
// 
// 
// // IDC SERVER PORT TABLE
// BOOL IdcDomainTable::WriteIdcServerPortTable(LPCTSTR ip)
// {
// 	static LPCTSTR fieldname[7] = 
// 	{
// 		_T("SIP"),		// 服务器IP
// 		_T("PORT"),		// 开放端口
// 		_T("PRO"),		// 协议
// 	};
// 
// 	ASSERT(ip!=NULL);
// 	ASSERT(ip[0]!=NULL);
// 	try
// 	{
// 		if (m_dataarray.GetSize()<=0)
// 			return TRUE;
// 		// 写IDCServer表
// 		CString dbfname;
// 		dbfname.Format("%sIDCPort\\Temp\\%s.dbf", TABLE_DIR, ip);
// 		if (!CreateIdcPortTable(dbfname))
// 			return FALSE;
// 		// 打开临时表
// 		Code4	table_code;
// 		Data4	table_data;
// 		CTableAccess acTable;
// 		acTable.SetCode4(&table_code, &table_data);
// 		table_code.autoOpen = 0;
// 		table_code.lockAttempts = LOCKTRYTIMES;
// 		if(!OpenDbfTable(table_code, table_data, dbfname))
// 		{
// 			WriteLog("dce", "打开表%s失败!", dbfname);
// 			CloseTable(&table_code, &table_data, NULL, &acTable);
// 			return  FALSE;
// 		}
// 		// 关联字段
// 		Field4 fields[3];
// 		for (int i = 0; i < 3; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, fieldname[i]))
// 			{
// 				WriteLog("dce", "关联字段%s失败!", fieldname[i]);
// 				CloseTable(&table_code, &table_data, NULL, &acTable);
// 				// 字段可能有变化, 删除旧的模版
// 				return FALSE;
// 			}
// 		}
// 		char filetm[24];
// 		GetTimeString(m_tabletime, "%Y%m%d%H%M", filetm, 24);
// 		// 添加记录
// 		for (int j=0; j<m_dataarray.GetSize(); j++)
// 		{
// 			// 拆解串"IP(15)_PRO_Domain"
// 			char buf[400];
// 			ServerData tmpdata = m_dataarray.GetAt(j);
// 			int slen = (tmpdata.Server.GetLength()>380)?380:tmpdata.Server.GetLength();
// 			memset(buf+(slen&~4), 0, 8);
// 			memcpy(buf, tmpdata.Server, slen);
// 
// 			char *p1, *p2, *p3;
// 			p1 = strtok(buf, "_");
// 			if(p1 == NULL) continue;
// 			p2 = strtok(NULL, "_");
// 			if(p2 == NULL) continue;
// 			p3 = strtok(NULL, "");
// 			if(p3 == NULL) continue;
// 			// 写一条记录
// 			table_data.appendStart(0);
// 			table_data.blank();
// 			fields[0].assign(p1);				// 服务器IP
// 			fields[1].assign(p2);				// 服务器Port
// 			fields[2].assign(p3);				// 协议
// 			table_data.append();
// 		}
// 		m_dataarray.RemoveAll();
// 		// 关闭表
// 		CloseTable(&table_code, &table_data, NULL, &acTable);
// 		// 输出到DSE的收集目录中
// 		CString OutDbfNmae;
// 		OutDbfNmae.Format("%sIDCPort\\%s", TABLE_DIR, ip);
// 		if (_access(OutDbfNmae, 0)!=0)
// 			CreateDir(OutDbfNmae);
// 		OutDbfNmae.Format("%sIDCPort\\%s\\%s.dbf", TABLE_DIR, ip, filetm);
// 		rename(dbfname, OutDbfNmae);
// 	}
// 	catch(...)
// 	{
// 		WriteLog("dce", "WriteIdcPortTable(%s) Error!!", ip);
// 		return FALSE;
// 	}
// 	return TRUE;
// }
// 
// 
// CString SplitTopDomain(const char *domain);
// BOOL IdcDomainTable::WriteIdcServerTable(LPCTSTR ip)
// {	
// 	static LPCTSTR fieldname[12] = 
// 	{
// 		_T("TMSECT"),	// 开始时间 2006022317
// 		_T("DOMAIN"),	// 域名
// 		_T("TOPDOMAIN"),// 顶级域名
// 		_T("SIP"),		// 服务器IP
// 		_T("ISP"),		// 运营商
// 		_T("PRO"),		// 协议
// 		_T("SUBPRO"),	// 协议
// 		_T("PORT"),		// 端口
// 		_T("ClassType"),// 类型
// 		_T("TypeState"),// 明确标识
// 		_T("intestate"),// 交互状态
// 		_T("NUM")		// 次数
// 	};
// 
// 	ASSERT(ip!=NULL);
// 	ASSERT(ip[0]!=NULL);
// 	try
// 	{
// 		if (m_dataarray.GetSize()<=0)
// 			return TRUE;
// 		// 写IDCServer表
// 		CString dbfname;
// 		dbfname.Format("%sIDCServer\\Temp\\%s.dbf", TABLE_DIR, ip);
// 		if (!CreateIdcServerTable(dbfname))
// 			return FALSE;
// 
// 		// 打开临时表
// 		Code4	table_code;
// 		Data4	table_data;
// 		CTableAccess acTable;
// 		acTable.SetCode4(&table_code, &table_data);
// 		table_code.autoOpen = 0;
// 		table_code.lockAttempts = LOCKTRYTIMES;
// 		if(!OpenDbfTable(table_code, table_data, dbfname))
// 		{
// 			WriteLog("dce", "打开表%s失败!", dbfname);
// 			CloseTable(&table_code, &table_data, NULL, &acTable);
// 			return  FALSE;
// 		}
// 		// 关联字段
// 		Field4 fields[12];
// 		for (int i = 0; i < 12; i++)
// 		{
// 			if (r4success != fields[i].init(table_data, fieldname[i]))
// 			{
// 				WriteLog("dce", "关联字段%s失败!", fieldname[i]);
// 				CloseTable(&table_code, &table_data, NULL, &acTable);
// 				// 字段可能有变化, 删除旧的模版
// 				return FALSE;
// 			}
// 		}
// 		char nowtm[12];
// 		char filetm[24];
// 		GetTimeString(m_tabletime, "%Y%m%d%H%M", filetm, 24);
// 		memset(nowtm, 0, 12);
// 		memcpy(nowtm, filetm, 10);
// 		// 添加记录
// 		for (int j=0; j<m_dataarray.GetSize(); j++)
// 		{
// 			// 拆解串"SIP\1SUBPRO\11PRO\1PORT\1DOMAIN\1ClassType"
// 			char buf[400];
// 			ServerData tmpdata = m_dataarray.GetAt(j);
// 			int slen = (tmpdata.Server.GetLength()>380)?380:tmpdata.Server.GetLength();
// 			memset(buf+(slen&~4), 0, 8);
// 			memcpy(buf, tmpdata.Server, slen);
// 
// 			char *tmp=buf, *p[9];
// 			for (int k=0; k<9; k++)
// 			{
// 				char *tp = strchr(tmp, '\1');
// 				if (tp != NULL)
// 				{
// 					*tp++ = 0;
// 					p[k] = tmp;
// 					tmp = tp;
// 				}
// 				else
// 				{
// 					if (k == 8)
// 					{
// 						p[k] = tmp;
// 						k++;
// 					}
// 					break;
// 				}
// 			}
// 			if (k < 9)
// 				continue;
// 			// 写一条记录
// 			table_data.appendStart(0);
// 			table_data.blank();
// 			fields[0].assign(nowtm);				// 开始时间 2006022317 
// 			if (p[8]!=NULL && p[8][0]!=0)
// 			{
// 				// 包含域名则提取顶级域名
// 				CString topdomain;
// 				topdomain = SplitTopDomain(p[8]);
// 				fields[1].assign(p[8]);					// 域名
// 				fields[2].assign(topdomain);			// 顶级域名
// 			}
// 			else
// 			{
// 				// 没有域名时填IP
// 				fields[1].assign(p[0]);					// 服务器IP
// 				fields[2].assign(p[0]);					// 服务器IP
// 			}
// 			fields[3].assign(p[0]);					// 服务器IP
// 			fields[4].assign(p[1]);					// 运营商
// 			fields[5].assign(p[3]);					// 协议
// 			fields[6].assign(p[2]);					// 子协议
// 			fields[7].assign(p[4]);					// 端口
// 			fields[8].assign(p[5]);					// 类型
// 			fields[9].assign(p[6]);					// 明确标识
// 			fields[10].assign(p[7]);					// 交互状态
// 			if(tmpdata.count > 0)
// 				fields[11].assignLong(tmpdata.count);	// 次数
// 			else
// 				fields[11].assignLong(1);				// 次数
// 			table_data.append();
// 		}
// 		m_dataarray.RemoveAll();
// 		// 关闭表
// 		CloseTable(&table_code, &table_data, NULL, &acTable);
// 		// 输出到DSE的收集目录中
// 		CString OutDbfNmae;
// 		OutDbfNmae.Format("%sIDCServer\\%s", TABLE_DIR, ip);
// 		if (_access(OutDbfNmae, 0)!=0)
// 			CreateDir(OutDbfNmae);
// 		OutDbfNmae.Format("%sIDCServer\\%s\\%s.dbf", TABLE_DIR, ip, filetm);
// 		rename(dbfname, OutDbfNmae);
// 	}
// 	catch(...)
// 	{
// 		WriteLog("dce", "WriteIdcServerTable(%s) Error!!", ip);
// 		return FALSE;
// 	}
// 	return TRUE;
// }
// 
// CString SplitTopDomain(const char *domain)
// {
// 	CString tmpTop;
// 	CString strDomain = domain;
// 	CString Part1, Part2, Part3;
// 	int offDot;
// 	DWORD havePart = 0;	// 0x4 info, 0x2 com, 0x1 cn
// 
// 	offDot = strDomain.ReverseFind('.');
// 	if(offDot == -1)
// 	{
// 		return "";
// 	}
// 	
// 	Part3 = strDomain.Mid(offDot);
// 	if(strlen(Part3) == 3)		// 国家代码, 2位+'.'共3位
// 		havePart |= 0x1;
// 	else
// 		havePart |= 0x2;
// 
// 	strDomain = strDomain.Left(offDot);
// 	offDot = strDomain.ReverseFind('.');
// 	if(offDot == -1)
// 	{
// 		tmpTop.Format("%s%s", strDomain, Part3);
// 		return tmpTop;
// 	}
// 	Part2 = strDomain.Mid(offDot);
// 	if(havePart&0x2)	// have .com
// 	{
// 		tmpTop.Format("%s%s", Part2.Mid(1), Part3);
// 		return tmpTop;
// 	}
// 	else
// 	{
// 		Search sch(Part2);
// //		sch.InitSearch;
// 		if(sch.find(".com.net.edu.gov.org.aero.biz.cat.coop.info.jobs.mobi.museum.name.pro.travel.mil.int") != -1)
// 			havePart |= 0x2;
// 		else
// 		{
// 			tmpTop.Format("%s%s", Part2.Mid(1), Part3);
// 			return tmpTop;
// 		}
// 	}
// 
// 	strDomain = strDomain.Left(offDot);
// 	offDot = strDomain.ReverseFind('.');
// 	if(offDot == -1)
// 		tmpTop.Format("%s%s%s", strDomain, Part2, Part3);
// 	else
// 	{
// 		Part1 = strDomain.Mid(offDot);
// 		tmpTop.Format("%s%s%s", Part1.Mid(1), Part2, Part3);
// 	}
// 	return tmpTop;
// }
// 
QQPassTable::QQPassTable()
{

}

QQPassTable::~QQPassTable()
{
	
}
// 
// BOOL QQPassTable::CreateTable(const char *dbfFile)
// {
// 	CString dbfname;
// 	dbfname.Format("%s",dbfFile);
// 
// 	if(_access(QQPASS_DIR,0) !=0)
// 		mkdir(QQPASS_DIR);
// 
// 	if(_access(QQPASS_TEMPDIR,0) !=0)
// 		mkdir(QQPASS_TEMPDIR);
// 
// 	if(_access(QQPASSMOBAN_DIR,0) !=0)
// 		mkdir(QQPASSMOBAN_DIR);
// 
// 	if(dbfname.IsEmpty())
// 		dbfname = GetTableName();
// 
// 	if(_access(QQPASSMOBAN_FILE,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,QQPASSMOBAN_FILE,qqpas_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",QQPASSMOBAN_FILE);
// 			if(_access(QQPASSMOBAN_FILE,0) ==0)
// 				remove(QQPASSMOBAN_FILE);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(_access(dbfname,0) !=0)
// 	{
// 		if(!CopyFile(QQPASSMOBAN_FILE,dbfname,true))
// 		{
// 			WriteLog("dce", "拷贝模版文件%s到%s失败", QQPASSMOBAN_FILE,dbfname);
// 			Code4 cb;
// 			cb.autoOpen = 0; //不自动打开索引
// 			Data4 data;
// 			int rc = data.create(cb,dbfname,qqpas_fields);
// 			
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce","创建数据库%s失败.",dbfname);
// 				if(_access(dbfname,0) ==0)
// 					remove(dbfname);
// 				return FALSE;
// 			}
// 			cb.closeAll();
// 			cb.initUndo();
// 			return TRUE;
// 		}
// 	}
// 		
// 	return TRUE;
// }
// 
CString QQPassTable::AddRecord(CStringArray& valuearr)
{
	DceProcotol tmpDcePro;
	tmpDcePro.m_ddefiletype = "stat";
	tmpDcePro.m_DseInfoCount = 1;
	tmpDcePro.m_pDseFileInfo = new IpDataStruct[tmpDcePro.m_DseInfoCount];
	for (int i=0; i<tmpDcePro.m_DseInfoCount; i++)
	{
		tmpDcePro.m_pDseFileInfo[i].m_filenum = 0;
		tmpDcePro.m_pDseFileInfo[i].m_realflen = 0;
		tmpDcePro.m_pDseFileInfo[i].m_pindexfile = NULL;
		tmpDcePro.m_pDseFileInfo[i].m_prealfile = NULL;
	}
	LPIpDataStruct pval = NULL;
	pval = tmpDcePro.AllocFileToDse(4, 0);
	DceFileStream StreamInfo;
	memset(&StreamInfo, 0, sizeof(StreamInfo));
	memcpy(StreamInfo.CaseIDorWarnID, "qq", 3);
	memcpy(StreamInfo.Pro, "qq", 3);
	StreamInfo.Type = 5;		// qqpass

	try
	{
		if(valuearr.GetSize() ==0)
			return "ok";
		
		CString strtaobaoid,strtaobaopass,spytm;
		CString dbfname,cdxname;

		int icount=valuearr.GetSize();
		for(int i = 0;i<icount;i++)
		{
			CString tempstr=valuearr.GetAt(i);
			int ipos=tempstr.Find('\1');
			if(ipos >=0 )
				strtaobaoid = tempstr.Left(ipos);
			strtaobaopass = tempstr.Mid(ipos+1);

			ipos=tempstr.Find('\1');
			if(ipos >=0 )
				strtaobaopass = tempstr.Left(ipos);

			spytm=tempstr.Mid(ipos+1);
						
			time_t tt = time(0);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString ttstr;
			ttstr.Format("%04d-%02d-%02d %02d:%02d:%02d",
				now.wYear, now.wMonth, now.wDay,
				now.wHour, now.wMinute, now.wSecond);

			char buffer[1000];
			int len = sprintf(buffer, 
				"REC_TIME=%d\r\n"
				"QQID=%s\r\n"
				"PASS=%s\r\n"
				"PASSTYPE=0\r\n"
				"SYSTYPE=\r\n"
				"SRCDEV=\r\n"
				"MARK=\r\n"
				"CAP_TIME=%s\r\n"
				"LUSE_TIME=%s\r\n",
				time(0), strtaobaoid, strtaobaopass, ttstr, ttstr);
			tmpDcePro.MergeMemDataToStream(pval, &StreamInfo,	buffer, len, NULL);
		}

		// 关闭流文件
		DWORD reallen = ftell(pval->m_prealfile);
		int re = fclose(pval->m_prealfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_realfname, re);
		}
		pval->m_prealfile = NULL;
		re = fclose(pval->m_pindexfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
		}
		pval->m_pindexfile = NULL;
		// 提交流文件
		CString idxfname = pval->m_indexfname;
		CString ralfname = pval->m_realfname;
		idxfname.Replace("tmp20", "20");
		ralfname.Replace("tmp20", "20");
		re = rename(pval->m_realfname, ralfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				tmpDcePro.m_TmpFileArr.Add(pval->m_realfname);
			}
		}
		re = rename(pval->m_indexfname, idxfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				tmpDcePro.m_TmpFileArr.Add(pval->m_indexfname);
			}
		}
		pval->m_realfname.Empty();
		pval->m_indexfname.Empty();
		pval->m_filenum=0;
		pval->m_realflen=0;
		tmpDcePro.RenameTmpDir();

		return "ok";	
	}
	catch (...)
	{
		WriteLog("dce"," TAOBAOPassTable::AddRecord,catch error");
		return "失败";
	}

// 	try
// 	{
// 		if(valuearr.GetSize() ==0)
// 			return "ok";
// 		
// 		CString strqqid,strqqpass,spytm;
// 		CString dbfname,cdxname;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 	
// 		dbfname = GetTableName();
// 		if (_access(dbfname,0)!=0)
// 		{
// 			CreateTable(dbfname);
// 		}
// 
// 		if(!OpenDbfTable(code, data,dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			WriteLog("dce","引擎关键字打开表失败");
// 			if (_access(QQPASSMOBAN_FILE,0)==0)
// 				DeleteFile(QQPASSMOBAN_FILE);
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			return "打开表失败";
// 		}
// 		// 关联字段
// 		Field4	fields[6];
// 		static LPCTSTR FIELDSNAME[6] =
// 		{	
// 			_T("REC_TIME"),	_T("QQID"),   _T("PASS"),
// 		    _T("PASSTYPE"),_T("CAP_TIME"),_T("LUSE_TIME")
// 		};
// 
// 		for (int i = 0; i < 6; i++)
// 		{
// 			if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!", FIELDSNAME[i]);
// 				if (_access(QQPASSMOBAN_FILE,0)==0)
// 					DeleteFile(QQPASSMOBAN_FILE);
// 				if (_access(dbfname,0)==0)
// 					DeleteFile(dbfname);
// 				return "失败";
// 			}
// 		}
// 
// 		int icount=valuearr.GetSize();
// 		for(i = 0;i<icount;i++)
// 		{
// 			CString tempstr=valuearr.GetAt(i);
// 			int ipos=tempstr.Find('\1');
// 			if(ipos >=0 )
// 				strqqid = tempstr.Left(ipos);
// 			strqqpass = tempstr.Mid(ipos+1);
// 
// // 			ipos=tempstr.Find('\1');
// // 			if(ipos >=0 )
// // 				strqqpass = tempstr.Left(ipos);
// 
// //			spytm=tempstr.Mid(ipos+1);
// 						
// 			data.appendStart(0);
// 			time_t tt = time(0);
// 			SYSTEMTIME now;
// 			GetLocalTime(&now);
// 			CString ttstr;
// 			ttstr.Format("%04d-%02d-%02d %02d:%02d:%02d",
// 				now.wYear, now.wMonth, now.wDay,
// 				now.wHour, now.wMinute, now.wSecond);
// 			fields[0].assignLong(tt);
// 			fields[1].assign(strqqid);
// 			fields[2].assign(strqqpass);
// 			fields[3].assign("0");
// 			fields[4].assign(ttstr)/*assign(spytm)*/;
// 			fields[5].assign(ttstr)/*assign(spytm)*/;
// 
// 			if(data.append() != r4success)
// 			{
// 				WriteLog("dce","增加信息失败");
// 				
// 				CloseTable(&code, &data, NULL, &ac_table);
// 				return "失败";
// 			}
// 		}
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return "ok";	
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," QQPassTable::AddRecord,catch error");
// 		return "失败";
// 	}
}
// 
// CString QQPassTable::GetTableName()
// {
// 	char daytime[20];
// 	CString dbfname;
// 	time_t ttNow = time(0);
// 	struct tm *tmNow = localtime(&ttNow);
// 	strftime(daytime, sizeof(daytime), "%Y%m%d%H", tmNow);				
// 	
// 	dbfname.Format("%s%s.dbf",QQPASS_TEMPDIR,daytime);	
// 	return dbfname;
// }
// 
// BOOL    QQPassTable::ConvertTmpFile()
// {
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	CString  filename;
// 	char curtm[20];
// 	CString srcfile,dstfile;
// 	
// 	time_t lTime;
// 	struct tm* tmTime = NULL;
// 	time(&lTime);
// 	tmTime = localtime(&lTime);
// 
// 	tmTime->tm_hour -= 1;
// 	time_t newtime = mktime(tmTime);
// 	tmTime = localtime(&newtime);
// 	strftime(curtm, 20, "%Y%m%d%H",	tmTime);
// 	
// 	hfindfile = FindFirstFile(QQPASS_TEMPDIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				filename.Format("%s",finddata.cFileName);
// 				filename = filename.Left(10);
// 				if(filename.CompareNoCase(curtm) <=0)
// 				{
// 					srcfile.Format("%s%s",QQPASS_TEMPDIR,finddata.cFileName);
// 					dstfile.Format("%s%s",QQPASS_DIR,finddata.cFileName);
// 					MoveFile(srcfile,dstfile);
// 				}	
// 				
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "QQPassTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	
// 	return TRUE;
// }
// 
// 
// BOOL    QQPassTable::CheckFileJiYa()
// {
// 	int count = 0;
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	hfindfile = FindFirstFile(QQPASS_DIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				if (++count>=MAXJIYDBF)
// 					break;
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "QQPassTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	if(count >=3)
// 		WriteLog("dce", "QQPassTable JIYA!!");
// 	
// 	return TRUE;
// }
TAOBAOPassTable::TAOBAOPassTable()
{

}

TAOBAOPassTable::~TAOBAOPassTable()
{
	
}

// BOOL TAOBAOPassTable::CreateTable(const char *dbfFile)
// {
// 	CString dbfname;
// 	dbfname.Format("%s",dbfFile);
// 
// 	if(_access(TAOBAOPASS_DIR,0) !=0)
// 		mkdir(TAOBAOPASS_DIR);
// 
// 	if(_access(TAOBAOPASS_TEMPDIR,0) !=0)
// 		mkdir(TAOBAOPASS_TEMPDIR);
// 
// 	if(_access(TAOBAOPASSMOBAN_DIR,0) !=0)
// 		mkdir(TAOBAOPASSMOBAN_DIR);
// 
// 	if(dbfname.IsEmpty())
// 		dbfname = GetTableName();
// 
// 	if(_access(TAOBAOPASSMOBAN_FILE,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,TAOBAOPASSMOBAN_FILE,taobaopas_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",TAOBAOPASSMOBAN_FILE);
// 			if(_access(TAOBAOPASSMOBAN_FILE,0) ==0)
// 				remove(TAOBAOPASSMOBAN_FILE);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(_access(dbfname,0) !=0)
// 	{
// 		if(!CopyFile(TAOBAOPASSMOBAN_FILE,dbfname,true))
// 		{
// 			WriteLog("dce", "拷贝模版文件%s到%s失败", TAOBAOPASSMOBAN_FILE,dbfname);
// 			Code4 cb;
// 			cb.autoOpen = 0; //不自动打开索引
// 			Data4 data;
// 			int rc = data.create(cb,dbfname,taobaopas_fields);
// 			
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce","创建数据库%s失败.",dbfname);
// 				if(_access(dbfname,0) ==0)
// 					remove(dbfname);
// 				return FALSE;
// 			}
// 			cb.closeAll();
// 			cb.initUndo();
// 			return TRUE;
// 		}
// 	}
// 		
// 	return TRUE;
// }

CString TAOBAOPassTable::AddRecord(CStringArray& valuearr)
{
	DceProcotol tmpDcePro;
	tmpDcePro.m_ddefiletype = "stat";
	tmpDcePro.m_DseInfoCount = 1;
	tmpDcePro.m_pDseFileInfo = new IpDataStruct[tmpDcePro.m_DseInfoCount];
	for (int i=0; i<tmpDcePro.m_DseInfoCount; i++)
	{
		tmpDcePro.m_pDseFileInfo[i].m_filenum = 0;
		tmpDcePro.m_pDseFileInfo[i].m_realflen = 0;
		tmpDcePro.m_pDseFileInfo[i].m_pindexfile = NULL;
		tmpDcePro.m_pDseFileInfo[i].m_prealfile = NULL;
	}
	LPIpDataStruct pval = NULL;
	pval = tmpDcePro.AllocFileToDse(3, 0);
	DceFileStream StreamInfo;
	memset(&StreamInfo, 0, sizeof(StreamInfo));
	memcpy(StreamInfo.CaseIDorWarnID, "taobao", 7);
	memcpy(StreamInfo.Pro, "taobao", 7);
	StreamInfo.Type = 4;		// taobao

	try
	{
		if(valuearr.GetSize() ==0)
			return "ok";
		
		CString strtaobaoid,strtaobaopass,spytm;
		CString dbfname,cdxname;

		int icount=valuearr.GetSize();
		for(int i = 0;i<icount;i++)
		{
			CString tempstr=valuearr.GetAt(i);
			int ipos=tempstr.Find('\1');
			if(ipos >=0 )
				strtaobaoid = tempstr.Left(ipos);
			strtaobaopass = tempstr.Mid(ipos+1);

			ipos=tempstr.Find('\1');
			if(ipos >=0 )
				strtaobaopass = tempstr.Left(ipos);

			spytm=tempstr.Mid(ipos+1);
						
			time_t tt = time(0);
			SYSTEMTIME now;
			GetLocalTime(&now);
			CString ttstr;
			ttstr.Format("%04d-%02d-%02d %02d:%02d:%02d",
				now.wYear, now.wMonth, now.wDay,
				now.wHour, now.wMinute, now.wSecond);

			char buffer[1000];
			int len = sprintf(buffer, 
				"REC_TIME=%d\r\n"
				"TAOBAOID=%s\r\n"
				"PASS=%s\r\n"
				"PASSTYPE=0\r\n"
				"SYSTYPE=\r\n"
				"SRCDEV=\r\n"
				"MARK=\r\n"
				"CAP_TIME=%s\r\n"
				"LUSE_TIME=%s\r\n",
				time(0), strtaobaoid, strtaobaopass, ttstr, ttstr);
			tmpDcePro.MergeMemDataToStream(pval, &StreamInfo,	buffer, len, NULL);
		}

		// 关闭流文件
		DWORD reallen = ftell(pval->m_prealfile);
		int re = fclose(pval->m_prealfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_realfname, re);
		}
		pval->m_prealfile = NULL;
		re = fclose(pval->m_pindexfile);
		if (re != 0)
		{
			WriteLog("dce", "关闭文件<%s>失败! ErrCode = %d", pval->m_indexfname, re);
		}
		pval->m_pindexfile = NULL;
		// 提交流文件
		CString idxfname = pval->m_indexfname;
		CString ralfname = pval->m_realfname;
		idxfname.Replace("tmp20", "20");
		ralfname.Replace("tmp20", "20");
		re = rename(pval->m_realfname, ralfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pval->m_realfname, ralfname);
			if (re != 0)
			{
				tmpDcePro.m_TmpFileArr.Add(pval->m_realfname);
			}
		}
		re = rename(pval->m_indexfname, idxfname);
		if (re != 0)
		{
			::Sleep(200);
			re = rename(pval->m_indexfname, idxfname);
			if (re != 0)
			{
				tmpDcePro.m_TmpFileArr.Add(pval->m_indexfname);
			}
		}
		pval->m_realfname.Empty();
		pval->m_indexfname.Empty();
		pval->m_filenum=0;
		pval->m_realflen=0;
		tmpDcePro.RenameTmpDir();

		return "ok";	
	}
	catch (...)
	{
		WriteLog("dce"," TAOBAOPassTable::AddRecord,catch error");
		return "失败";
	}
}

// CString TAOBAOPassTable::GetTableName()
// {
// 	char daytime[20];
// 	CString dbfname;
// 	time_t ttNow = time(0);
// 	struct tm *tmNow = localtime(&ttNow);
// 	strftime(daytime, sizeof(daytime), "%Y%m%d%H", tmNow);				
// 	
// 	dbfname.Format("%s%s.dbf",TAOBAOPASS_TEMPDIR,daytime);	
// 	return dbfname;
// }

// BOOL    TAOBAOPassTable::ConvertTmpFile()
// {
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	CString  filename;
// 	char curtm[20];
// 	CString srcfile,dstfile;
// 	
// 	time_t lTime;
// 	struct tm* tmTime = NULL;
// 	time(&lTime);
// 	tmTime = localtime(&lTime);
// 
// 	tmTime->tm_hour -= 1;
// 	time_t newtime = mktime(tmTime);
// 	tmTime = localtime(&newtime);
// 	strftime(curtm, 20, "%Y%m%d%H",	tmTime);
// 	
// 	hfindfile = FindFirstFile(TAOBAOPASS_TEMPDIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				filename.Format("%s",finddata.cFileName);
// 				filename = filename.Left(10);
// 				if(filename.CompareNoCase(curtm) <=0)
// 				{
// 					srcfile.Format("%s%s",TAOBAOPASS_TEMPDIR,finddata.cFileName);
// 					dstfile.Format("%s%s",TAOBAOPASS_DIR,finddata.cFileName);
// 					MoveFile(srcfile,dstfile);
// 				}	
// 				
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "TAOBAOPassTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	
// 	return TRUE;
// }
// 
// BOOL    TAOBAOPassTable::CheckFileJiYa()
// {
// 	int count = 0;
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	hfindfile = FindFirstFile(TAOBAOPASS_DIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				if (++count>=MAXJIYDBF)
// 					break;
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "TAOBAOPassTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	if(count >=3)
// 		WriteLog("dce", "TAOBAOPassTable JIYA!!");
// 	
// 	return TRUE;
// }
// 
// ERRORTable::ERRORTable()
// {
// 
// }
// 
// ERRORTable::~ERRORTable()
// {
// 	
// }
// 
// BOOL ERRORTable::CreateTable(const char *dbfFile)
// {
// 	CString dbfname;
// 	dbfname.Format("%s",dbfFile);
// 
// 	if(_access(ERROR_DIR,0) !=0)
// 		mkdir(ERROR_DIR);
// 
// 	if(_access(ERROR_TEMPDIR,0) !=0)
// 		mkdir(ERROR_TEMPDIR);
// 
// 	if(_access(ERRORMOBAN_DIR,0) !=0)
// 		mkdir(ERRORMOBAN_DIR);
// 
// 	if(dbfname.IsEmpty())
// 		dbfname = GetTableName();
// 
// 	if(_access(ERRORMOBAN_FILE,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,ERRORMOBAN_FILE,error_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",ERRORMOBAN_FILE);
// 			if(_access(ERRORMOBAN_FILE,0) ==0)
// 				remove(ERRORMOBAN_FILE);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 
// 	if(_access(dbfname,0) !=0)
// 	{
// 		if(!CopyFile(ERRORMOBAN_FILE,dbfname,true))
// 		{
// 			WriteLog("dce", "拷贝模版文件%s到%s失败", ERRORMOBAN_FILE,dbfname);
// 			Code4 cb;
// 			cb.autoOpen = 0; //不自动打开索引
// 			Data4 data;
// 			int rc = data.create(cb,dbfname,error_fields);
// 			
// 			if(!data.isValid())
// 			{
// 				WriteLog("dce","创建数据库%s失败.",dbfname);
// 				if(_access(dbfname,0) ==0)
// 					remove(dbfname);
// 				return FALSE;
// 			}
// 			cb.closeAll();
// 			cb.initUndo();
// 			return TRUE;
// 		}
// 	}
// 		
// 	return TRUE;
// }
// 
// CString ERRORTable::AddRecord(CStringArray& valuearr)
// {
// 	try
// 	{
// 		if(valuearr.GetSize() ==0)
// 			return "ok";
// 		
// 		CString strid,strpass,spytm;
// 		CString dbfname,cdxname;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 	
// 		dbfname = GetTableName();
// 		if (_access(dbfname,0)!=0)
// 		{
// 			CreateTable(dbfname);
// 		}
// 
// 		if(!OpenDbfTable(code, data,dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			WriteLog("dce","引擎关键字打开表失败");
// 			if (_access(ERRORMOBAN_FILE,0)==0)
// 				DeleteFile(ERRORMOBAN_FILE);
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			return "打开表失败";
// 		}
// 		// 关联字段
// 		Field4	fields[5];
// 		static LPCTSTR FIELDSNAME[5] =
// 		{	
// 			_T("ID"),   _T("PASS"),		_T("PASSTYPE"),	
// 			_T("PRO"),	_T("TYPE")
// 		};
// 
// 		for (int i = 0; i < 5; i++)
// 		{
// 			if (r4success != fields[i].init(data, FIELDSNAME[i]))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "关联字段%s失败!", FIELDSNAME[i]);
// 				if (_access(ERRORMOBAN_FILE,0)==0)
// 					DeleteFile(ERRORMOBAN_FILE);
// 				if (_access(dbfname,0)==0)
// 					DeleteFile(dbfname);
// 				return "失败";
// 			}
// 		}
// 
// 		int icount=valuearr.GetSize();
// 		for(i = 0;i<icount;i++)
// 		{
// 			CString tempstr=valuearr.GetAt(i);
// 			CStringArray tmparr;
// 			Splitstr(tempstr,tmparr,'\1');
// 			int ipos=tempstr.Find('\1');
// 			if(ipos >=0 )
// 				strid = tempstr.Left(ipos);
// 			strpass = tempstr.Mid(ipos+1);
// 
// // 			ipos=tempstr.Find('\1');
// // 			if(ipos >=0 )
// // 				strtaobaopass = tempstr.Left(ipos);
// 
// //			spytm=tempstr.Mid(ipos+1);
// 						
// 			data.appendStart(0);
// 			time_t tt = time(0);
// 			fields[0].assign(tmparr.GetAt(0));
// 			fields[1].assign(tmparr.GetAt(1));
// 			fields[2].assign(tmparr.GetAt(2));
// 			fields[3].assign(tmparr.GetAt(3));
// 			fields[4].assign(tmparr.GetAt(4));
// 
// 			if(data.append() != r4success)
// 			{
// 				WriteLog("dce","增加信息失败");
// 				
// 				CloseTable(&code, &data, NULL, &ac_table);
// 				return "失败";
// 			}
// 		}
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return "ok";	
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," ERRORTable::AddRecord,catch error");
// 		return "失败";
// 	}
// }
// 
// CString ERRORTable::GetTableName()
// {
// 	char daytime[20];
// 	CString dbfname;
// 	time_t ttNow = time(0);
// 	struct tm *tmNow = localtime(&ttNow);
// 	strftime(daytime, sizeof(daytime), "%Y%m%d%H", tmNow);				
// 	
// 	dbfname.Format("%s%s.dbf",ERROR_TEMPDIR,daytime);	
// 	return dbfname;
// }
// 
// BOOL    ERRORTable::ConvertTmpFile()
// {
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	CString  filename;
// 	char curtm[20];
// 	CString srcfile,dstfile;
// 	
// 	time_t lTime;
// 	struct tm* tmTime = NULL;
// 	time(&lTime);
// 	tmTime = localtime(&lTime);
// 
// 	tmTime->tm_hour -= 1;
// 	time_t newtime = mktime(tmTime);
// 	tmTime = localtime(&newtime);
// 	strftime(curtm, 20, "%Y%m%d%H",	tmTime);
// 	
// 	hfindfile = FindFirstFile(ERROR_TEMPDIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				filename.Format("%s",finddata.cFileName);
// 				filename = filename.Left(10);
// 				if(filename.CompareNoCase(curtm) <=0)
// 				{
// 					srcfile.Format("%s%s",ERROR_TEMPDIR,finddata.cFileName);
// 					dstfile.Format("%s%s",ERROR_DIR,finddata.cFileName);
// 					MoveFile(srcfile,dstfile);
// 				}	
// 				
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "ERRORTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	
// 	return TRUE;
// }
// 
// 
// BOOL    ERRORTable::CheckFileJiYa()
// {
// 	int count = 0;
// 	HANDLE hfindfile;
// 	WIN32_FIND_DATA finddata;
// 	hfindfile = FindFirstFile(ERROR_DIR, &finddata);
// 	if (hfindfile != INVALID_HANDLE_VALUE)
// 	{
// 		try
// 		{
// 			do
// 			{
// 				if (++count>=MAXJIYDBF)
// 					break;
// 			}while(FindNextFile(hfindfile, &finddata));
// 		}
// 		catch (...)
// 		{
// 			WriteLog("dce", "ERRORTable CheckFileJiYa Error!!");
// 		}
// 		FindClose(hfindfile);
// 	}	
// 
// 	if(count >=3)
// 		WriteLog("dce", "ERRORTable JIYA!!");
// 	
// 	return TRUE;
// }
// 
// // FIELD4INFO AlarmWarn_fields[] =				// 预警告警的结构
// {
// 	{"warn_class",	r4str, 50, 0},	// 预警分类
// 	{"rule_id",		r4str, 50, 0},	// 规则ID
// 	{"im_type",		r4str, 50, 0},	// 重点人员分类
// 	{"im_key",		r4str, 40, 0},	// 重点人员值
// 	{"real_name",	r4str, 40, 0},	// 姓名
// 	{"card_id",		r4str, 20, 0},	// 身份证号码
// 	{"key_type",	r4str, 50, 0},	// 预警关键字分类
// 	{"time_last",	r4str, 20, 0},	// 最后获得时间
// 	{"f_state",		r4str, 10, 0},	// 状态
// 	{"f_pro",		r4str, 20, 0},	// 协议
// 	{"red_num",		r4str, 5, 0},	// 红色告警次数
// 	{"orange_num",	r4str, 5, 0},	// 橙色告警次数
// 	{"yellow_num",	r4str, 5, 0},	// 黄色告警次数
// 	{"beg_tm",		r4str, 20, 0},	// 上线时间
// 	{"end_tm",		r4str, 20, 0},	// 下线时间
// 	{"spyaddr",		r4str, 50, 0},	//  截获地点
// 	{"dummytype",	r4str, 50, 0},	//  虚拟身份类型
// 	{"mark1",		r4str, 50, 0},	//  备用字段
// 	{"mark2",		r4str, 50, 0},	//  备用字段
// 	{"mark3",		r4str, 50, 0},	//  备用字段
// 	{ 0,0,0,0 }
// };
// WarnAlarmTable::WarnAlarmTable()
// {
// 	
// }
// 
// WarnAlarmTable::~WarnAlarmTable()
// {
// 	
// }
// 
// BOOL WarnAlarmTable::CreateTable(const char *dbfFile)
// {
// 	// 创建预警告警表
// 	LPCTSTR hpath = _T("x:\\xxx\\statdata\\warnalarm\\temp");
// 	if (_access(hpath, 0) != 0)
// 		CreateDir(hpath);
// 	LPCTSTR mobanfile = _T(TABLE_WARNALARMMOBAN);
// 	
// 	if(_access(mobanfile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,mobanfile,AlarmWarn_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",mobanfile);
// 			if(_access(mobanfile,0) ==0)
// 				remove(mobanfile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 	
// 	if(!CopyFile(mobanfile, dbfFile, TRUE))
// 	{
// 		WriteLog("dce", "拷贝模版文件%s到%s失败", mobanfile,dbfFile);
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,dbfFile,AlarmWarn_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",dbfFile);
// 			if(_access(dbfFile,0) ==0)
// 				remove(dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// 
// BOOL WarnAlarmTable::AddRecord(CStringArray& dataarr, LPCTSTR tmpdbfname)
// {
// 	int err = 0;
// 	try
// 	{
// err = 1;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 		CString dbfname;
// 		char dstfile[50];
// 		time_t filetm = time(0);
// 		GetTimeString(filetm, "%Y%m%d%H%M%swarnalarm.dbf", dstfile, 50);
// 		
// 		dbfname.Format("x:\\xxx\\statdata\\warnalarm\\temp\\%s", dstfile);
// 		
// err = 2;
// 		if (_access(dbfname,0)!=0)
// 		{
// 			if(!CreateTable(dbfname))
// 			{
// 				WriteLog("dce", "创建预警告警表失败");
// 				return FALSE;
// 			}
// 		}
// 
// err = 3;
// 		if(!OpenDbfTable(code, data, dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			DeleteFile(TABLE_WARNALARMMOBAN);
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			WriteLog("dce", "打开预警告警表失败");
// 			return FALSE;
// 		}
// 	
// err = 4;
// 		// 关联字段
// 		Field4	fields[WARNALARMFIELDCOUNT];
// 
// 		for (int i = 0; i < WARNALARMFIELDCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, AlarmWarn_fields[i].name))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "关联预警告警表字段<%s>失败!", AlarmWarn_fields[i].name);
// 				CloseTable(&code, &data, NULL, &ac_table);
// 				DeleteFile(TABLE_WARNALARMMOBAN);
// 				DeleteFile(dbfname);
// 				return FALSE;
// 			}
// 		}
// 		int tmpn = dataarr.GetSize();
// 
// err = 5;
// 		
// 		for(i = 0;i<tmpn;i++)
// 		{
// err = 51;
// 			
// 			char *tp = dataarr[i].GetBuffer(0);
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0; tp!=NULL && m<WARNALARMFIELDCOUNT; m++)
// 			{
// err = 500+m;
// 				
// 				ASSERT(fields[m].field != NULL);
// 				char *p1 = strchr(tp, '\1');
// 				if (p1!=NULL)
// 					*p1++ = 0;
// 
// 				fields[m].assign(tp);
// 				tp = p1;
// 			}
// 			data.append();
// 		}
// err = 6;
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return TRUE;
// 
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," warnalarmTable::AddRecord() Error!!=%d", err);
// 		return FALSE;
// 	}
// 
// }
// 
// 
// //案件告警相关类
// FIELD4INFO casealarm_fields[] = 
// {
// 	{"PRO",			r4str,	20,		0},		// 协议类型
// 	{"SPYADDR",		r4str,	50,		0},		// 截获点
// 	{"TM",			r4str,	20,		0},		// 告警时间
// 	{"USERID",		r4str,	50,		0},		// 用户ID
// 	{"CASEID",		r4str,	50,		0},		// 案件ID
// 	{"CASENAME",	r4str,	40,		0},		// 案件名称
// 	{"OBJID",		r4str,	66,		0},		// 对象类型_对象名
// 	{"LIMITTYPE",	r4str,	2,		0},		// 地址类型
// 	{"LIMITADDR",	r4str,	200,	0},		// 地址名
// 	{"REMARK",      r4str,  200,    0},		// 线索备注		
// 	{"SIP",			r4str,	16,		0},		// 源IP
// 	{"DIP",			r4str,	16,		0},		// 目标IP
// 	{"SPORT",		r4str,	6,		0},		// 源端口
// 	{"DPORT",		r4str,	6,		0},		// 目标端口
// 	{"ACCOUNT",		r4str,	50,		0},		// 帐号
// 	{"PHONE",		r4str,	50,		0},		// 电话
// 	{"DACCOUNT",	r4str,	50,		0},		// 帐号2
// 	{"DPHONE",		r4str,	50,		0},		// 电话2
// 	{0,				0,		0,		0}
// };
// 
// CaseAlarmTable::CaseAlarmTable()
// {
// 	LPCTSTR hpathcaseweb  = _T("x:\\xxx\\statdata\\casealarm\\web\\temp");
// 	LPCTSTR hpathcasehand = _T("x:\\xxx\\statdata\\casealarm\\hand\\temp");
// 
// 	if (_access(hpathcaseweb, 0) != 0)
// 		CreateDir(hpathcaseweb);
// 	if (_access(hpathcasehand, 0) != 0)
// 		CreateDir(hpathcasehand);
// }
// 
// CaseAlarmTable::~CaseAlarmTable()
// {
// 	
// }
// 
// /*************************************************
//   Function:       GetTableName
//   Description:    获取要使用的表名称
//   Calls:          
//   Called By:      AddRecord
//   Table Accessed: 
//   Table Updated:  
//   Input:          type 其值包括 web hand follow
//                   
//   Output:         
//   Return:         组合后的表名称
//   Others:         
// *************************************************/
// CString CaseAlarmTable::GetTableName(CString type)
// {
// 	CString dbfname;
// 	char dstfile[50];
// 	time_t filetm = time(0);
// 
// 	GetTimeString(filetm, "%Y%m%d%H%M%scasealarm.dbf", dstfile, 50);
// 	
// 	dbfname.Format("x:\\xxx\\statdata\\casealarm\\%s\\temp\\%s", type,dstfile);
// 
// 	return dbfname;
// }
// 
// /*************************************************
//   Function:       CreateTable
//   Description:    创建表
//   Calls:          
//   Called By:      AddRecord
//   Table Accessed: 
//   Table Updated:  
//   Input:          dbfFile 要创建的表名称
//                   
//   Output:         
//   Return:         成功返回 TRUE ，否则 FALSE
//   Others:         
// *************************************************/
// BOOL CaseAlarmTable::CreateTable(const char *dbfFile)
// {
// 	// 创建案件告警表临时目录
// 
// 	LPCTSTR mobanfile = _T(TABLE_CASEALARMMOBAN);
// 	
// 	if(_access(mobanfile,0) !=0)
// 	{
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,mobanfile,casealarm_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",mobanfile);
// 			if(_access(mobanfile,0) ==0)
// 				remove(mobanfile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 	}
// 	
// 	if(!CopyFile(mobanfile, dbfFile, TRUE))
// 	{
// 		WriteLog("dce", "拷贝模版文件%s到%s失败", mobanfile,dbfFile);
// 		Code4 cb;
// 		cb.autoOpen = 0; //不自动打开索引
// 		Data4 data;
// 		int rc = data.create(cb,dbfFile,AlarmWarn_fields);
// 		
// 		if(!data.isValid())
// 		{
// 			WriteLog("dce","创建数据库%s失败.",dbfFile);
// 			if(_access(dbfFile,0) ==0)
// 				remove(dbfFile);
// 			return FALSE;
// 		}
// 		cb.closeAll();
// 		cb.initUndo();
// 		return TRUE;
// 	}
// 	return TRUE;
// }
// 
// /*************************************************
//   Function:       AddRecord
//   Description:    往表里面追加记录
//   Calls:          GetTableName ，CreateTable，OpenDbfTable，CloseTable
//   Called By:      ColletTotal
//   Table Accessed: 案件界面、手机、实时跟踪表
//   Table Updated:  
//   Input:          dataarr  传入的是数组类型的一条记录的组合
//                   type     web     hand  follow
// 						   web     是指的页面告警
// 						   hand	   是指的手机告警
// 						   follow  是指的实时跟踪
//   Output:         
//   Return:         成功返回 TRUE ，否则 FALSE
//   Others:         
// *************************************************/
// BOOL CaseAlarmTable::AddRecord(CStringArray& dataarr,CString type)
// {
// 	int err = 0;
// 	try
// 	{
// err = 1;
// 		CTableAccess ac_table;
// 		Code4 code;
// 		code.autoOpen = 0;
// 		code.lockAttempts = LOCKTRYTIMES;
// 		Data4 data;
// 		ac_table.SetCode4(&code, &data);
// 
// 		CString dbfname;
// 		dbfname = GetTableName(type);
// 		
// err = 2;
// 		if (_access(dbfname,0)!=0)
// 		{
// 			if(!CreateTable(dbfname))
// 			{
// 				WriteLog("dce", "创建预警告警表失败");
// 				return FALSE;
// 			}
// 		}
// 
// err = 3;
// 		if(!OpenDbfTable(code, data, dbfname))
// 		{
// 			CloseTable(&code, &data, NULL, &ac_table);
// 			DeleteFile(TABLE_CASEALARMMOBAN);
// 			if (_access(dbfname,0)==0)
// 				DeleteFile(dbfname);
// 			WriteLog("dce", "打开预警告警表失败");
// 			return FALSE;
// 		}
// 	
// err = 4;
// 		// 关联字段
// 		Field4	fields[CASEALARMFIELDCOUNT];
// 
// 		for (int i = 0; i < CASEALARMFIELDCOUNT; i++)
// 		{
// 			if (r4success != fields[i].init(data, casealarm_fields[i].name))
// 			{
// 				code.closeAll();
// 				code.initUndo();
// 				WriteLog("dce", "关联预警告警表字段<%s>失败!", casealarm_fields[i].name);
// 				CloseTable(&code, &data, NULL, &ac_table);
// 				DeleteFile(CASEALARMFIELDCOUNT);
// 				DeleteFile(dbfname);
// 				return FALSE;
// 			}
// 		}
// 		int tmpn = dataarr.GetSize();
// err = 5;
// 		
// 		for(i = 0;i<tmpn;i++)
// 		{
// err = 51;
// 			
// 			char *tp = dataarr[i].GetBuffer(0);
// 			data.appendStart(0);
// 			data.blank();
// 			for(int m=0; tp!=NULL && m<CASEALARMFIELDCOUNT; m++)
// 			{
// err = 500+m;
// 				
// 				ASSERT(fields[m].field != NULL);
// 				char *p1 = strchr(tp, '\1');
// 				if (p1!=NULL)
// 					*p1++ = 0;
// 
// 				fields[m].assign(tp);
// 				tp = p1;
// 			}
// 			data.append();
// 		}
// err = 6;
// 		CloseTable(&code, &data, NULL, &ac_table);
// 		return TRUE;
// 
// 	}
// 	catch (...)
// 	{
// 		WriteLog("dce"," CaseAlarmTable::AddRecord() Error!!=%d", err);
// 		return FALSE;
// 	}
// 
//}