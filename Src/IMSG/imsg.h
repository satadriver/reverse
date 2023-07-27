//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	imsg
//	版本:		1.0
//	创建日期:	2008-2-26
//	作者:		荣强
//	主要功能:	IMSG协议还原
//	模块描述:
//	主要函数:	
//**************************************************************** 

#include "Winsock2.h"
#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

#define			MSGBUFFERLEN		256*1024
#define			MSNINBUFLEN			128*1024
#define			MSNOUTBUFLEN		120*1024
#define			MSGBUFLEN			8192
#define			IMSGPRO				"dce6"

//////////////////////////////////////////////////////////////////////////
// 结构/类

typedef struct IMSGPARA
{
	time_t			m_packtm;			// 数据包的时间
	int				m_direction;		// 数据包方向 1: 表示sip-->dip,
										// 2: 表示dip-->sip
	char*			m_packetbuf;		// 包数据缓冲区
	int				m_packetbufsize;	// 包数据缓冲区大小
	int				m_packetlen;		// 包数据长度
	char*			m_msgbuf;			// 还原信息缓冲区
	int				m_msgbufsize;		// 还原信息缓冲区大小
	int				m_msglen;			// 还原信息的长度

// 	CStringArray	m_hisstrarr;		// 活动历史的内容
// 	CStringArray	m_webfigrarr;		// 虚拟身份的内容
	CStringArray	m_webidarr;		// 虚拟身份的内容

	BOOL			m_bIdcSvr;			// 是否写IDCServer
}IMSGPARA;

typedef	struct SimpleSeq
{
	WORD	size;
	WORD	pos;
	DWORD	seq[15];
	SimpleSeq(){ memset(this, 0, sizeof(SimpleSeq)); }
}SimpleSeq;

inline BOOL SearchSeq(SimpleSeq& simpSeq, DWORD seq)
{
	if (simpSeq.size < 15)
	{
		for (WORD i=0; i<simpSeq.size; i++)
		{
			if (simpSeq.seq[i] == seq)
				return TRUE;
		}
		simpSeq.seq[simpSeq.size++] = seq;
		return FALSE;
	}
	else
	{
		for (WORD i=0; i<15; i++)
		{
			if (simpSeq.seq[i] == seq)
				return TRUE;
		}
		simpSeq.seq[simpSeq.pos++] = seq;
		if (simpSeq.pos>=15)
			simpSeq.pos = 0;
	}
	return FALSE;
}

class ImsgProtocol : public DceProcotol
{
public:
	ImsgProtocol();
	virtual	~ImsgProtocol();

public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr);
	virtual void RenameTmpDir();
	virtual BOOL WriteObjectState(time_t packtm, CString& strtxt);
};

///////////////////////////////////////////////////////////////////////////
// 变量
extern DceMission*		g_pImsgMissObj;
extern LPCTSTR			g_pImsgSubDir[];
extern char*			g_pMsgBuffer;
extern int				g_Msglen;

extern DWORD			g_OicqSion;
extern DWORD			g_MsnSion;
extern DWORD			g_YmsgSion;
// IMSG
extern DWORD			g_ImsgSion;
// GAME
extern DWORD			g_GameSion;

extern DWORD			g_qqTry[6];
//////////////////////////////////////////////////////////////////////////
// 函数
inline CString CreateFilename(LPCTSTR sip, LPCTSTR dip, LPCTSTR suffix)
{
	CString fname;
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString tmppath = g_pImsgMissObj->GetTmpPath();
	fname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_1_%u.%s",
		tmppath, now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute,
		now.wSecond, sip, dip, rand(), suffix);
	return fname;
}

