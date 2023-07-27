//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	imsg
//	�汾:		1.0
//	��������:	2008-2-26
//	����:		��ǿ
//	��Ҫ����:	IMSGЭ�黹ԭ
//	ģ������:
//	��Ҫ����:	
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
// �ṹ/��

typedef struct IMSGPARA
{
	time_t			m_packtm;			// ���ݰ���ʱ��
	int				m_direction;		// ���ݰ����� 1: ��ʾsip-->dip,
										// 2: ��ʾdip-->sip
	char*			m_packetbuf;		// �����ݻ�����
	int				m_packetbufsize;	// �����ݻ�������С
	int				m_packetlen;		// �����ݳ���
	char*			m_msgbuf;			// ��ԭ��Ϣ������
	int				m_msgbufsize;		// ��ԭ��Ϣ��������С
	int				m_msglen;			// ��ԭ��Ϣ�ĳ���

// 	CStringArray	m_hisstrarr;		// ���ʷ������
// 	CStringArray	m_webfigrarr;		// ������ݵ�����
	CStringArray	m_webidarr;		// ������ݵ�����

	BOOL			m_bIdcSvr;			// �Ƿ�дIDCServer
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
// ����
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
// ����
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

