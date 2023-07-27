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
	long			m_readflg;			// ��������д��־
	DWORD			m_ipcount[2];		// IDC ip�ĸ���
	int				m_hashsize[2];		// ʹ�ù�ϣ����ʱ�Ĺ�ϣ����
	DWORD			*m_pIpArr[2];		// ʹ�ù�ϣ����ʱ��ϣ��ĵ�Ԫָ��
	DWORD			*m_pNodePtr[2];		// ʹ�öβ���ʱ�ε�ָ��
	int				m_blockcount[2];
	CStringArray	m_textArr[2];
	CMapStringToPtr m_ispnamemap;
};

extern  IdcIpLib	v_IdcIpLib;

#endif // !defined(AFX_IDCIPLIB_H__471D16DF_4025_49E5_BD5A_83BC4AB571F5__INCLUDED_)
