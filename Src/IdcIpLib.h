// IdcIpLib.h: interface for the IdcIpLib class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDCIPLIB_H__471D16DF_4025_49E5_BD5A_83BC4AB571F5__INCLUDED_)
#define AFX_IDCIPLIB_H__471D16DF_4025_49E5_BD5A_83BC4AB571F5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dcepub.h"

class  IdcIpLib
{
public:
	IdcIpLib();
	~IdcIpLib();

	void AddIdcIp(DWORD begip, DWORD endip, CString strisp);
	BOOL QueryIdcIP(DWORD ip, CString& strisp);
	void Optimize();
	void ClearAllIp();
	int  GetIpCount() {return m_ipcount[m_readflg];}
protected:
	long			m_readflg;			// 缓冲区读写标志
	DWORD			m_ipcount[2];		// IDC ip的个数
	int				m_hashsize[2];		// 使用哈希查找时的哈希表宽度
	DWORD			*m_pIpArr[2];		// 使用哈希查找时哈希表的单元指针
	DWORD			*m_pNodePtr[2];		// 使用段查找时段的指针
	int				m_blockcount[2];
	CStringArray	m_textArr[2];
	CMapStringToPtr m_ispnamemap;
};

extern  IdcIpLib	v_IdcIpLib;

#endif // !defined(AFX_IDCIPLIB_H__471D16DF_4025_49E5_BD5A_83BC4AB571F5__INCLUDED_)
