// PublicFun.h: interface for the PublicFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PUBLICFUN_H__A754DC8B_B99C_4721_96EE_9E2B0653E8B3__INCLUDED_)
#define AFX_PUBLICFUN_H__A754DC8B_B99C_4721_96EE_9E2B0653E8B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PostStruct.h"

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace


//*************************************************
//	函数名称:		ProcessHttpGet
//	描述:			
//	输入参数:		
//	输出参数:		
//	返回值:			
//	其它:			
//*************************************************
void	ProcessHttpGet(DATEHEADER& packh, const char* packet,HttpProtocol* httpprotocl);

//*************************************************
//	函数名称:		ProcessHttpPost
//	描述:			处理POST的包
//	输入参数:		packh 包头
//					packet 包数据
//	输出参数:		
//	返回值:			
//	其它:			单包的POST就输出文件否则就建会话
//*************************************************
void	ProcessHttpPost(DATEHEADER& packh, const char* packet,HttpProtocol* httpprotocl);

//*************************************************
//	函数名称:		
//	描述:			
//	输入参数:		
//	输出参数:		
//	返回值:			
//	其它:			
//*************************************************


int		SearchIPUrl(LPCTSTR ip1, LPCTSTR ip2, LPCTSTR url, int urllen, CaseInfo& caseinfo);
BOOL	SearchUrlAndKey(LPCTSTR url, int urllen, CaseInfo& caseinfo);

// void	WriteHttpUrlFile(
// 						 PACKETHEADER& packheader, 
// 						 HttpCommInfo*
// 						 pResquest, 
// 						 CString sipaddr,
// 						 CString dipaddr);
BOOL	DealEspecialBrowse(HttpCommInfo& Request,HttpSession* s);

CString GetUnixShortName(CString fname);

// void	WriteTeUrlIndexFile(
// 				DATEHEADER& packh, 
// 				int PacketFrom,		// 单包直接带
// 				SocketSession& socketsion,
// 				int TeType);			// 类型

void	WriteFreeGateFile(
				DATEHEADER& packh, 
				HttpCommInfo& Request,
				HttpSession& httpsession,
				LPCTSTR pSpyaddr, 
				LPCTSTR pro);

BOOL	InsertImsgProxy(
				IMSGPROXY &imsgproxy,
				HttpCommInfo &PackInfo,
				LPCTSTR spyaddr, 
				HttpSession* pSession);

BOOL	CreateFilename(HttpSession*s);
BOOL	WriteIndexFIle(HttpCommInfo* pResquest,
					   CaseInfo& caseinfo, DATEHEADER& packh, LPCTSTR pro);

BOOL	ParserPostData(HttpCommInfo& Request,
					   int headpos,
					   DATEHEADER& packh,
					   LPCTSTR pPacket,
					   HttpSession& httpsession);

int		DealOnePacketGet(DATEHEADER& packh, const char* Packet);
int		DealOnePacketPost(DATEHEADER& packh, const char* Packet);
//int		DealOnePacketHttpOk(DATEHEADER& packh, const char* Packet);

BOOL	ISSavePost(DATEHEADER& packh, const char* Packet);
void	ConverHttp2Html(char *tmp_content, int tmp_content_len);
BOOL	FormatHttpReqHeader(HttpCommInfo& header, char *buf, int len,
			BOOL unixhead, unsigned char ServerIp[4], WORD port, DATEHEADER* packh=NULL);

int		DealWebMail(HttpSession *s, HttpCommInfo* phttpInfo, CString& attflst);
int		DealHotMailFile(HttpSession *s ,HttpCommInfo* phttpinfo);
void	DealCompressedFile(CString &filename, HttpCommInfo* phttpinfo);

CString Getemlfield(IMessagePtr& iMsg);
//CString GetemlText(IMessagePtr& iMsg);
UINT	CheckSuffix(LPCTSTR fname ,CString &suffix);
//BOOL WriteIndexFIle(CString sip, CString dip, LPCTSTR smac, LPCTSTR dmac, HttpCommInfo* pResquest,
//					LPCTSTR UserAccount, LPCTSTR Phone, CaseInfo& caseinfo, CString& indexfile, int IsProxy);
UINT	CheckContenttype(CString &contenttype,CString &suffix);
CString WriteHttpIndexFile(HttpSession * s,HttpCommInfo* pResquest,HttpCommInfo* pResponse, BOOL& caseflag, BOOL& indexflag);
int		DealOneToHotKey(DATEHEADER& packh, const char* Packet);
BOOL    WriteQQPassToMap(PostFieldinfoList outlist,CString spytm);

CString GetHttpInfo(HttpCommInfo& Request, BOOL bGetALL = TRUE);

typedef struct _QQMapInfo_ 
{
	CStringArray uservaluelist;
	CStringArray passvaluelist;
}QQMapInfo,*LPQQMapInfo;

inline int Search4ByteFind(const char* buf, int len)
{
	int i = Search2CRLF.find(buf, len);		// "\r\n\r\n"
	if (i >= 0)
		return i+4;
	else
		return -1;

	/*
	char *end = strstr(buf,"\r\n\r\n");
	if (end)
	{
		int len = end - buf;
		if (len >= 0)
		{
			return len + 4;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
*/	
}

inline int Search2ByteFind(const char* buf, int len)
{
	int i = Search2CRLFUnix.find(buf, len);	// "\n\n"
	if (i >= 0)
		return i+2;
	else
		return -1;
}
#endif // !defined(AFX_PUBLICFUN_H__A754DC8B_B99C_4721_96EE_9E2B0653E8B3__INCLUDED_)
