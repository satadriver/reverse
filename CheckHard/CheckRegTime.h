// CheckRegTime.h: interface for the CCheckRegTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_)
#define AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 据本机时间检查是否允许
BOOL CheckLocalAllow(time_t &tmBegin, time_t tmEnd);
// 据时间服务器检查
BOOL CheckNtpAllow(time_t tmBegin, time_t tmEnd, BOOL &bAllow);
// 检查一个200OK
BOOL CheckOne200OK(const char *buf, int len, time_t tmEnd);

#endif // !defined(AFX_CHECKREGTIME_H__1B962D3D_3627_4703_8C65_1371883298F3__INCLUDED_)
