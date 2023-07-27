// stattable.h: interface for the StatTable class.
//
//////////////////////////////////////////////////////////////////////
//**************************************
//关键字统计表.h文件
//
//
//20051009
//**************************************

#if !defined(AFX_STAKEY_H__20051009__INCLUDED_)
#define AFX_STAKEY_H__20051009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "..\\include\\d4all.hpp"

// #define		LOCKTRYTIMES	5
// #define		FILETYPE_DBF    1
// #define     FILETYPE_CDX    2
// 
// class  StatKeyTable
// {
// public:
// 	StatKeyTable();
// 	virtual ~StatKeyTable();
// 	
// public:
// 	CString GetTableInfo(LPCTSTR type);
// 	BOOL	CreateTable(const char *dbfFile = NULL);
// 	CString GetTableName(int WantType);
// 	
// public:
// 	CString AddRecord(CStringArray* valuearr,CString hisfilename);
// };
// 
// class  PreKeyTable
// {
// public:
// 	PreKeyTable();
// 	virtual ~PreKeyTable();
// 	
// public:
// 	CString GetTableInfo(LPCTSTR type);
// 	BOOL	CreateTable(const char *dbfFile = NULL);
// 	CString GetTableName(int WantType);
// 	
// public:
// 	BOOL AddRecord(CStringArray& dataarr, LPCTSTR prekeyname);
// };

// struct ServerData
// {
// 	CString		Server;
// 	DWORD		count;
// };
// 
// typedef CArray<ServerData, ServerData&> CServerDataArray;
// 
// class  IdcDomainTable
// {
// public:
// 	IdcDomainTable();
// 	~IdcDomainTable();
// 	
// public:
// 	time_t				m_tabletime;
// 	CServerDataArray	m_dataarray;
// public:
// 	BOOL	WriteIdcServerTable(LPCTSTR ip);
// 	BOOL	WriteIdcServerPortTable(LPCTSTR ip);
// 	BOOL	AddRecord(CString str, DWORD count)
// 	{
// 		ServerData tmp;
// 		tmp.Server = str;
// 		tmp.count = count;
// 		m_dataarray.Add(tmp);
// 		return TRUE;
// 	}
// 
// 	static void	ClearTemp();
// 	static BOOL	CreateIdcServerTable(LPCTSTR dbfname);
// 	static BOOL	CreateIdcPortTable(LPCTSTR dbfname);
// };
// 
class  QQPassTable
{
public:
	QQPassTable();
	~QQPassTable();
	
public:
	time_t				m_tabletime;

public:
//     BOOL	CreateTable(LPCTSTR dbfname);
// 	CString GetTableName();	

public:
	CString	AddRecord(CStringArray& valuearr);
// 	BOOL    ConvertTmpFile();
// 	BOOL    CheckFileJiYa();
};

class  TAOBAOPassTable
{
public:
	TAOBAOPassTable();
	~TAOBAOPassTable();
	
public:
	time_t				m_tabletime;

public:
//     BOOL	CreateTable(LPCTSTR dbfname);
// 	CString GetTableName();	

public:
	CString	AddRecord(CStringArray& valuearr);
// 	BOOL    ConvertTmpFile();
// 	BOOL    CheckFileJiYa();
};
// class  ERRORTable
// {
// public:
// 	ERRORTable();
// 	~ERRORTable();
// 	
// public:
// 	time_t				m_tabletime;
// 
// public:
//     BOOL	CreateTable(LPCTSTR dbfname);
// 	CString GetTableName();	
// 
// public:
// 	CString	AddRecord(CStringArray& valuearr);
// 	BOOL    ConvertTmpFile();
// 	BOOL    CheckFileJiYa();
// };
/*
	预警告警表类
*/
// class WarnAlarmTable
// {
// public:
// 	WarnAlarmTable();
// 	~WarnAlarmTable();
// 	
// public:
// 	time_t				m_tabletime;
// 	
// public:
//     BOOL	CreateTable(LPCTSTR dbfname);
// //	CString GetTableName();	
// public:
// 	BOOL AddRecord(CStringArray& dataarr, LPCTSTR tmpdbfname);
// 	
// };

/*
	案件告警表类,包括界面告警和手机告警，以及实时跟踪信息
	实时跟踪是属于密取中的，为统一，业务上做了下调整，实时
	跟踪和页面告警统一，只是收集策略应该不一样，实时跟踪不应
	整合到一块，要记录整个上网过程

	edit by ccj,20070705 17:30
*/
// class CaseAlarmTable
// {
// public:
// 	CaseAlarmTable();
// 	~CaseAlarmTable();
// 	
// public:
// 	time_t				m_tabletime;
// 	
// public:
//     BOOL	CreateTable(LPCTSTR dbfname);
// 	CString GetTableName(CString type);
// 
// public:
// 	BOOL AddRecord(CStringArray& dataarr, LPCTSTR tmpdbfname);
// 	
// };

/*
模板的定义
*/
//#define		TABLE_WARNALARMMOBAN		"x:\\netspy\\statdata\\warnalarm\\temp\\alarmmoban3.dbf"

//界面告警和手机告警的模板是一样的
//#define     TABLE_CASEALARMMOBAN     "x:\\netspy\\statdata\\casealarm\\casealarmmoban.dbf"

#endif // !defined(AFX_STAKEY_H__20051009__INCLUDED_)
