// SyncPara.h: interface for the CSyncPara class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCPARA_H__624FF831_CD38_4854_98C6_C93E2B1C924A__INCLUDED_)
#define AFX_SYNCPARA_H__624FF831_CD38_4854_98C6_C93E2B1C924A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSyncPara  
{
public:
	CSyncPara();
	virtual ~CSyncPara();

	enum syntypes 
	{
		syn_bcp	= 1,	// BCP方式, 搜索远程服务器, 以时间判断更新
		syn_sql	= 2,	// SQL方式, 本地SQL状态表判断更新
		syn_as	= 3		// 管理AS方式
	};
public:
	// 更新, 结果存为本地bcp
	BOOL UpdatePara(LPCTSTR szPath, LPCSTR szName,CStringArray &arrCS,DWORD synMethod = CSyncPara::syn_as,BOOL bForce =FALSE,LPCTSTR szSQL= "");
	// 载入待查
	BOOL LoadPara(LPCTSTR szBcpPath, DWORD &seekPos, BOOL bCrypt);
	// 得到一条记录
	BOOL GetOneRecord(DWORD &seekPos, CStringArray &arrField);
	// 得到一条记录
	BOOL GetAllIpOneRecord(DWORD &seekPos, CStringArray &arrField);
	// 保存
	BOOL SavePara(LPCTSTR szBcpPath, CTime tmModify, BOOL bCrypt);
private:
	BOOL UpdateFromAS(CStringArray &arrCServer, LPCTSTR szTable, CTime &tmVer, CString strSQL, BOOL bForce);
private:
	DWORD	m_lenBuf;		// 表缓冲已用
	char	*m_pBuf;		// 表内容
};

#endif // !defined(AFX_SYNCPARA_H__624FF831_CD38_4854_98C6_C93E2B1C924A__INCLUDED_)
