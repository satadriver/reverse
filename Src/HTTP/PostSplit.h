// PostSplit.h: interface for the Split class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PostSPLIT_H__8D0C5120_B231_4EB4_B3DA_6A4B5C26D456__INCLUDED_)
#define AFX_PostSPLIT_H__8D0C5120_B231_4EB4_B3DA_6A4B5C26D456__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "HttpSession.h"

//httpup字符串分割(特殊处理的拆分)
class PostSplit  
{
public:
	//拆形如: 值 \1 值 \1 值	
	static BOOL Split(CString &str ,CStringArray &array,LPCTSTR e);
	//拆形如: 值 \1 值 \1 值
	//static void Split(CString str ,CStringArray &array,CString e);
	//拆 形如:姓名=hzs,得到姓名，hzs
	static void Split(CString &str,CString &field,CString &value,LPCTSTR e); 
	//区分开字段和值 拆: 编号=1001 \n 姓名=dfff \n 
	static void	Split(CString &str,CStringArray &fields,CStringArray &values,LPCTSTR e,LPCTSTR d); 
	static void	Split(CString &str,PostFieldinfoList &postinfolst,LPCTSTR e,LPCTSTR d); 
	//是否ascii码串
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
//	函数名称:		SplitHttpupToHis
//	描述:			拆解HTTP上传
//	输入参数:		url 
//	输出参数:		
//	返回值:			
//	其它:			
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
