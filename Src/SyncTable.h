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
		syn_bcp	= 1,	// BCP��ʽ, ����Զ�̷�����, ��ʱ���жϸ���
		syn_sql	= 2,	// SQL��ʽ, ����SQL״̬���жϸ���
		syn_as	= 3		// ����AS��ʽ
	};
public:
	// ����, �����Ϊ����bcp
	BOOL UpdatePara(LPCTSTR szPath, LPCSTR szName,CStringArray &arrCS,DWORD synMethod = CSyncPara::syn_as,BOOL bForce =FALSE,LPCTSTR szSQL= "");
	// �������
	BOOL LoadPara(LPCTSTR szBcpPath, DWORD &seekPos, BOOL bCrypt);
	// �õ�һ����¼
	BOOL GetOneRecord(DWORD &seekPos, CStringArray &arrField);
	// �õ�һ����¼
	BOOL GetAllIpOneRecord(DWORD &seekPos, CStringArray &arrField);
	// ����
	BOOL SavePara(LPCTSTR szBcpPath, CTime tmModify, BOOL bCrypt);
private:
	BOOL UpdateFromAS(CStringArray &arrCServer, LPCTSTR szTable, CTime &tmVer, CString strSQL, BOOL bForce);
private:
	DWORD	m_lenBuf;		// ��������
	char	*m_pBuf;		// ������
};

#endif // !defined(AFX_SYNCPARA_H__624FF831_CD38_4854_98C6_C93E2B1C924A__INCLUDED_)
