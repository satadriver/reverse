// CheckRegTime.h: interface for the CCheckRegTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_)
#define AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �ݱ���ʱ�����Ƿ�����
BOOL CheckLocalAllow(time_t &tmBegin, time_t tmEnd);
// ��ʱ����������
BOOL CheckNtpAllow(time_t tmBegin, time_t tmEnd, BOOL &bAllow);
// ���һ��200OK
BOOL CheckOne200OK(const char *buf, int len, time_t tmEnd);

#endif // !defined(AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_)
