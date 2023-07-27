// PostSplit.h: interface for the Split class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PostSPLIT_H__8D0C5120_B231_4EB4_B3DA_6A4B5C26D456__INCLUDED_)
#define AFX_PostSPLIT_H__8D0C5120_B231_4EB4_B3DA_6A4B5C26D456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HttpSession.h"

//httpup�ַ����ָ�(���⴦��Ĳ��)
class PostSplit  
{
public:
	//������: ֵ \1 ֵ \1 ֵ	
	static BOOL Split(CString &str ,CStringArray &array,LPCTSTR e);
	//������: ֵ \1 ֵ \1 ֵ
	//static void Split(CString str ,CStringArray &array,CString e);
	//�� ����:����=hzs,�õ�������hzs
	static void Split(CString &str,CString &field,CString &value,LPCTSTR e); 
	//���ֿ��ֶκ�ֵ ��: ���=1001 \n ����=dfff \n 
	static void	Split(CString &str,CStringArray &fields,CStringArray &values,LPCTSTR e,LPCTSTR d); 
	static void	Split(CString &str,PostFieldinfoList &postinfolst,LPCTSTR e,LPCTSTR d); 
	//�Ƿ�ascii�봮
	static BOOL  IsAsciiStr(CString &Str);
public:
	PostSplit();
	~PostSplit();
};

void	BuildDeHttpUpInfo();
DWORD	SplitHttpup(CString topdomain,
					char*	pstrpost,
					CString &strCrcBase, 
					PostFieldinfoList & outlist);

BOOL GetHttpupHisRecArray(
					PostFieldinfoList &inlist, 
					HistoryRecord &hisRec,
					CStringArray &arrHisRec,
					SocketSession* pSion);

//*************************************************
//	��������:		SplitHttpupToHis
//	����:			���HTTP�ϴ�
//	�������:		url 
//	�������:		
//	����ֵ:			
//	����:			
//*************************************************
void	SplitHttpupToHis(CString url,
					CString contenttype,
					HttpSession* pSion,
					LPCTSTR pro,
					DATEHEADER& packh,
					CString host,
					bool bhttpproxy);

DWORD	SplitHttpDown(CString topdomain,
					CString &strCrcBase,
					PostFieldinfoList & outlist);

void SplitHttpDownToHis(CString url,
					CString contenttype,
					HttpSession* pSion);


#endif // !defined(AFX_PostSPLIT_H__8D0C5120_B231_4EB4_B3DA_6A4B5C26D456__INCLUDED_)
