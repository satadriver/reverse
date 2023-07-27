// QQGet.h: interface for the CQQGet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QQGET_H__1D399FD7_D430_424D_A59D_7CE4CC71BACA__INCLUDED_)
#define AFX_QQGET_H__1D399FD7_D430_424D_A59D_7CE4CC71BACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <map>
#include <vector>
using namespace std;

typedef struct _QQ_GROUP
{
	CString groupid;
	CString groupname;
	map<CString, CString>		qq2name;
}sQQGroup;

class CQQInfo
{
public:
	CQQInfo();
	~CQQInfo();

public:
	CString m_clientuin;	/// �Լ���QQ��
	CString m_clientnick;	/// ���ѵ��ǳ�
	CString m_clientkey;	

	CString DumpQun();
	CString DumpFirend();
	
private:
	void DumpOneQun(CString groupid);
	void GetGroupInfo(CString data);

	map<CString, sQQGroup*>	m_qqgroup;
	map<CString, sQQGroup*>	m_qqfriend;
};

void DealQQGet(DATEHEADER& dataheader,const char *pPacket);
BOOL pushdata(CString clientuin,CString clientkey);
#endif // !defined(AFX_QQGET_H__1D399FD7_D430_424D_A59D_7CE4CC71BACA__INCLUDED_)
