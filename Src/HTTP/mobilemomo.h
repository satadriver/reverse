// mobilemomo.h: interface for the mobilemomo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOBILEMOMO_H__A703A0B7_1D55_4624_8111_B3388C9FF9E4__INCLUDED_)
#define AFX_MOBILEMOMO_H__A703A0B7_1D55_4624_8111_B3388C9FF9E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "http.h"
#include "HttpSession.h"
#include "PublicFun.h"
#include "..\\DceMission.h"

#include "StringManipulation.h"
struct MOMOSTRUCT
{
	CString from;
	CString to;
	CString msg;
};

BOOL Momo_DealPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);
CString Momo_U8ToUnicode(char *szU8);
int Momo_getMyKey(long momoid, int cb, int v);
void Momo_reverseBytes(byte* a1, int a2);
void Momo_L1Decrypt(byte* src, int cb, long momoid, byte* des);
BOOL Momo_passwordSave(int momoNum,int momoKey);
BOOL Momo_SplitMsg(CString msg,MOMOSTRUCT& momost);
CString Momo_Utf82GB2312(LPCTSTR strMessage);
BOOL IsMomoPacket(const char* pPacket, DATEHEADER& packh);
BOOL ProcessMomoPacket(const char* pPacket, DATEHEADER& packh,HttpProtocol* httpprotocl);
#endif // !defined(AFX_MOBILEMOMO_H__A703A0B7_1D55_4624_8111_B3388C9FF9E4__INCLUDED_)
