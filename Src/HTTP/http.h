//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	http
//	版本:		1.0
//	创建日期:	2008-2-26
//	作者:		荣强
//	主要功能:	
//	模块描述:
//	主要函数:	
//**************************************************************** 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

#define		CR						'\r'
#define		LF						'\n'
#define		CRLF					"\r\n"

#define		DATABUFFER2KSIZE		2048
#define		DATABUFFER4KSIZE		4096
#define		DATABUFFER8KSIZE		8192
#define		DATABUFFER16KSIZE		16384

#define		HEADBUFFERSIZE			16300
#define		INIHEADBUFFERSIZE		1514
#define		HTTPPRO					_T("dce2")


class HttpProtocol : public DceProcotol
{
public:
	HttpProtocol();
	HttpProtocol(int httpfilenum);

	virtual	~HttpProtocol();
public:
	//	LPCTSTR			m_ddefiletype;	// 处理dde文件的类型名称
	//	WORD			m_ObjectID;		// 协议对象ID
	//	WORD			m_ObjectVer;	// 协议对象版本
	//	DceProcotol*	m_pNext;		// 下一个协议对象
	//	DWORD			m_ProMark;		// 协议目录的掩码
	//	DWORD			m_flowmeter;	// 包处理的计数
	
	DWORD	m_GetPacketCount;		// 以单包方式处理的Get包总个数
	DWORD	m_HttpurlCount;			// 以单包方式处理的Get包中写URL文件的包个数
	DWORD	m_GetSessCount;			// 以单包方式处理的Get包中建立会话的包数
	DWORD	m_IdcHttpCount;			// 直接建立会话的Get包
	DWORD	m_PostPacketCount;		// 以单包方式处理的Post包总个数
	DWORD	m_HttpupCount;			// 以单包方式处理的Get包中写Httpup文件的包个数
	DWORD	m_PostSessCount;		// 以单包方式处理的Post包中建立会话的包数
	DWORD	m_HttpSessCount;		// 建立会话的后续的包数
	DWORD	m_HttpokCount;			// Http ok 的包数
	DWORD	m_HttpsPacketCount;		// Https的包数
	DWORD	m_RubbishPacketCount;	// 所有没有直接处理的包数
	DWORD	m_UltrasurfPacketCount;	// 无界8的包数
	DWORD	m_freegatePacketCount;	// 之门的包数
	DWORD	m_httpFileNum;

	char*	m_CHUNKBUFFER;
	char*	m_HttpFileBuffer;
	DceMission*	m_httpMissObj;
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr);
	virtual void RenameTmpDir();
	virtual BOOL WriteObjectState(time_t now, CString& strtxt);
};

typedef struct PACKETHEADER
{
	int		type;
	CString sip;
	CString dip;
	int		sport;
	int		dport;
	CString smac;
	CString dmac;
}PACKETHEADER;

typedef CMap<unsigned __int64,unsigned __int64,DWORD,DWORD> IPHostMap;

//extern CString	g_HttpTmpPath;
extern IPHostMap	g_IpHostMap;
extern CString		g_HttpRarPath;
extern DceMission*	g_httpMissObj;
extern LPCTSTR		g_DDEPOS;
extern char*		g_HttpFileBuffer;
extern char*		g_CHUNKBUFFER;
extern int			g_httpFrom;

// extern DWORD	g_GetPacketCount;		// 以单包方式处理的Get包总个数
// extern DWORD	g_HttpurlCount;			// 以单包方式处理的Get包中写URL文件的包个数
// extern DWORD	g_GetSessCount;			// 以单包方式处理的Get包中建立会话的包数
// extern DWORD	g_IdcHttpCount;			// 直接建立会话的Get包
// extern DWORD	g_PostPacketCount;		// 以单包方式处理的Post包总个数
// extern DWORD	g_HttpupCount;			// 以单包方式处理的Get包中写Httpup文件的包个数
// extern DWORD	g_PostSessCount;		// 以单包方式处理的Post包中建立会话的包数
// extern DWORD	g_HttpSessCount;		// 建立会话的后续的包数
// extern DWORD	g_HttpokCount;			// Http ok 的包数
// extern DWORD	g_HttpsPacketCount;		// Https的包数
// extern DWORD	g_RubbishPacketCount;	// 所有没有直接处理的包数
// extern DWORD	g_UltrasurfPacketCount;	// 无界8的包数
// extern DWORD	g_freegatePacketCount;	// 之门的包数

extern DWORD	g_httpFileNum;
extern Search	g_ExplorerType1;		// ("windows");
extern Search	g_ExplorerType2;		// ("linux");
extern BOOL		g_DumpPacketFlg;		// 

extern map<CString,time_t> g_uniqmid;
CString GetDomain(CString url);

inline int IsHttpsPacket(DATEHEADER& packh, const char* pPacket)
{
	// 处理https的包
	// https包格式:
	// 命令号(1B) 版本(2B) 片断长度(2B) ...(片断数据) (下一个片断)
	if (packh.m_datalen>5)
	{
		if (*pPacket!=22)
			return 0;		// 不需要的命令号
		BYTE onedata = (BYTE)(pPacket[5]);
		int count = 0;
		if (onedata==2)		// Server Hello
		{
			DWORD plen = 0;
			WORD ver = 0;
			ver = *((WORD*)(pPacket+1));
			while (plen<packh.m_datalen)
			{
				if (ver != *((WORD*)(pPacket+plen+1)))
					return 0;	// 版本不匹配
				plen += Swap2Byte(*((WORD*)(pPacket+plen+3)))+5;
				count++;
			}
			return count;
		}
	}
	return 0;
}

//LPCTSTR GetDataSrc(DATEHEADER& packh, int PacketFrom, CString &strISP);
