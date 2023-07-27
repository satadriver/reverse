//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	http
//	�汾:		1.0
//	��������:	2008-2-26
//	����:		��ǿ
//	��Ҫ����:	
//	ģ������:
//	��Ҫ����:	
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
	//	LPCTSTR			m_ddefiletype;	// ����dde�ļ�����������
	//	WORD			m_ObjectID;		// Э�����ID
	//	WORD			m_ObjectVer;	// Э�����汾
	//	DceProcotol*	m_pNext;		// ��һ��Э�����
	//	DWORD			m_ProMark;		// Э��Ŀ¼������
	//	DWORD			m_flowmeter;	// ������ļ���
	
	DWORD	m_GetPacketCount;		// �Ե�����ʽ�����Get���ܸ���
	DWORD	m_HttpurlCount;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
	DWORD	m_GetSessCount;			// �Ե�����ʽ�����Get���н����Ự�İ���
	DWORD	m_IdcHttpCount;			// ֱ�ӽ����Ự��Get��
	DWORD	m_PostPacketCount;		// �Ե�����ʽ�����Post���ܸ���
	DWORD	m_HttpupCount;			// �Ե�����ʽ�����Get����дHttpup�ļ��İ�����
	DWORD	m_PostSessCount;		// �Ե�����ʽ�����Post���н����Ự�İ���
	DWORD	m_HttpSessCount;		// �����Ự�ĺ����İ���
	DWORD	m_HttpokCount;			// Http ok �İ���
	DWORD	m_HttpsPacketCount;		// Https�İ���
	DWORD	m_RubbishPacketCount;	// ����û��ֱ�Ӵ���İ���
	DWORD	m_UltrasurfPacketCount;	// �޽�8�İ���
	DWORD	m_freegatePacketCount;	// ֮�ŵİ���
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

// extern DWORD	g_GetPacketCount;		// �Ե�����ʽ�����Get���ܸ���
// extern DWORD	g_HttpurlCount;			// �Ե�����ʽ�����Get����дURL�ļ��İ�����
// extern DWORD	g_GetSessCount;			// �Ե�����ʽ�����Get���н����Ự�İ���
// extern DWORD	g_IdcHttpCount;			// ֱ�ӽ����Ự��Get��
// extern DWORD	g_PostPacketCount;		// �Ե�����ʽ�����Post���ܸ���
// extern DWORD	g_HttpupCount;			// �Ե�����ʽ�����Get����дHttpup�ļ��İ�����
// extern DWORD	g_PostSessCount;		// �Ե�����ʽ�����Post���н����Ự�İ���
// extern DWORD	g_HttpSessCount;		// �����Ự�ĺ����İ���
// extern DWORD	g_HttpokCount;			// Http ok �İ���
// extern DWORD	g_HttpsPacketCount;		// Https�İ���
// extern DWORD	g_RubbishPacketCount;	// ����û��ֱ�Ӵ���İ���
// extern DWORD	g_UltrasurfPacketCount;	// �޽�8�İ���
// extern DWORD	g_freegatePacketCount;	// ֮�ŵİ���

extern DWORD	g_httpFileNum;
extern Search	g_ExplorerType1;		// ("windows");
extern Search	g_ExplorerType2;		// ("linux");
extern BOOL		g_DumpPacketFlg;		// 

extern map<CString,time_t> g_uniqmid;
CString GetDomain(CString url);

inline int IsHttpsPacket(DATEHEADER& packh, const char* pPacket)
{
	// ����https�İ�
	// https����ʽ:
	// �����(1B) �汾(2B) Ƭ�ϳ���(2B) ...(Ƭ������) (��һ��Ƭ��)
	if (packh.m_datalen>5)
	{
		if (*pPacket!=22)
			return 0;		// ����Ҫ�������
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
					return 0;	// �汾��ƥ��
				plen += Swap2Byte(*((WORD*)(pPacket+plen+3)))+5;
				count++;
			}
			return count;
		}
	}
	return 0;
}

//LPCTSTR GetDataSrc(DATEHEADER& packh, int PacketFrom, CString &strISP);
