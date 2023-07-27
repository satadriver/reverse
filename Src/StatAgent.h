#if !defined(AFX_STAAGENT_H__20051009__INCLUDED_)
#define AFX_STAAGENT_H__20051009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "..\\include\\d4all.hpp"

#define		LOCKTRYTIMES	5
#define		FILETYPE_DBF    1
#define     FILETYPE_CDX    2

class  StatAgentTable
{
public:
	StatAgentTable();
	virtual ~StatAgentTable();
	
public:
	CString GetTableInfo(LPCTSTR type);
	BOOL CreateTable(const char *dbfFile = NULL);
	CString GetTableName(int WantType);
	
public:
	CString AddRecord(CStringArray valuearr);
};

#endif // !defined(AFX_STAAGENT_H__20051009__INCLUDED_)
