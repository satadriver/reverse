// public.h: interface for the publicc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLIC_H__55FCEE8C_4828_4481_8644_75D1CCB56A59__INCLUDED_)
#define AFX_PUBLIC_H__55FCEE8C_4828_4481_8644_75D1CCB56A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <io.h>
#include <WinSock2.h>

typedef struct FRAMETAGINFO
{
	WORD	tagtype;
	WORD	tagdata;
}FRAMETAGINFO;

typedef struct PACKETINFO
{
	BYTE			mac[12];
	BYTE			sip[4];
	BYTE			dip[4];
	WORD			sport;
	WORD			dport;
	int				frametagnum;				// 帧携带的标签个数(Vlan / PPPoE)
	FRAMETAGINFO	tagarr[8];
	BYTE			pro;						// tcp or udp
	BYTE			flag;
	WORD			datalen;
	DWORD			seq;
	DWORD			ack;
	BYTE			iphlen;
	BYTE			tcphlen;
	WORD			winsize;
	BYTE			pData[2000];
	BYTE			tcpoperation[48];			// TCP 选项
	time_t			packtm;
}PACKETINFO;

#define		FRAMETYPE_VLAN			1
#define		FRAMETYPE_PPPOE			2

CString Utf82GB2312(LPCTSTR strMessage);
void FormatData(BYTE* pData, int datalen, CString& strData, CString& strASC);
BOOL GetPacketInfo(PACKETINFO& packinfo, const BYTE* pPacket, int len);
int mystrstr(char strcstr[],char deststr[],int len);
#endif // !defined(AFX_PUBLIC_H__55FCEE8C_4828_4481_8644_75D1CCB56A59__INCLUDED_)
