// stattable.h: interface for the StatTable class.
//
//////////////////////////////////////////////////////////////////////
//**************************************
//网络活动统计表.h文件
//
//
//20051009
//**************************************

#if !defined(AFX_STATABLE_H__20051009__INCLUDED_)
#define AFX_STATABLE_H__20051009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "..\\include\\d4all.hpp"

#define		LOCKTRYTIMES	5
#define		FILETYPE_DBF    1
#define     FILETYPE_CDX    2

typedef struct WebFigure
{
	WebFigure*	pNext;
	LPCTSTR		fields;//[19];
}WebFigure;

class  WebHisTable
{
public:
	WebHisTable();
	~WebHisTable();
	
public:
	CString GetTableInfo(LPCTSTR type);
	BOOL CreateTable(const char *dbfFile = NULL);
	CString GetTableName(int WantType);
	BOOL WebfigCreateTable(const char *dbfFile);

//	void CheckWebFigureDbf();
//	void RenameWebFigureDbf(LPCTSTR dstdbfname);
public:
	BOOL AddRecord(CStringArray& dataarr, LPCTSTR hisdbfname);
 	BOOL AddRecord(CStringArray& valuearr);
 	BOOL WriteDbf(WebFigure* pRecord, LPCTSTR dseip);
	void ReleaseUnit(WebFigure* pUnit)
	{
		pUnit->pNext = m_pFreeList;
		m_pFreeList = pUnit;
	}
	
	WebFigure* GetNewUnit()
	{
		static int unitcount = 0;
		WebFigure* tp;
		if (m_pFreeList==NULL)
		{
			tp = new WebFigure[256];
			unitcount += 256;
			if (unitcount>1024)
			{
				WriteLog("dce", "WebFigure Unit Count: %d!!!", unitcount);
			}
			for (int i=0; i<256; i++)
			{
				ReleaseUnit(tp);
				tp++;
			}
		}
		tp = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		return tp;
	}
	WebFigure*		m_pDseRecord[80];
	WebFigure*		m_pFreeList;
// 	UINT*			m_pActiveDse;
// 	int				m_ActiveDseNum;
};

// BOOL CheckDbfStruct(const char *dbfFile, FIELD4INFO *pfieldinfo);
BOOL UpdateDbfData(const char *oldName, const char *newDbf);
BOOL CheckWebfigerJiYa(BOOL bIncludeJiYa, int iCurSelect);
BOOL tmpfiletofile(LPCTSTR type, time_t filetm);
void writetotmpfile(CStringArray& dataarr, LPCTSTR type);

#endif // !defined(AFX_STATABLE_H__20051009__INCLUDED_)
