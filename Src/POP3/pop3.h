//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	pop3
//	�汾:		1.0
//	��������:	2008-2-25
//	����:		��ǿ
//	��Ҫ����:	Pop3Э�黹ԭ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#ifndef			_POP3_H_
#define			_POP3_H_

// �������ɻ�ԭ���ʼ���С
#define		POP3_MAXMAILSIZE		15*1024*1024
#define		POP3_SIONBUFSIZE		8192

#define		POP3PRO					_T("dce3")

#define		NOEML					0			// ��ǰû���ʼ��ڴ���
#define		EMLPROCING				0xA			// ��ǰ���ʼ��ڴ���
#define		EMLOVER					0xF			// ��ǰ�ոմ�������һ���ʼ�(�Ҵ���ɹ�)

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

class Pop3Protocol : public DceProcotol
{
public:
	Pop3Protocol();
	virtual	~Pop3Protocol();
public:
	
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr);
	virtual BOOL WriteObjectState(time_t now, CString& strtxt);
};

extern DceMission*		g_pop3MissObj;
extern DWORD			g_Pop3FileNum;
extern char*			g_Pop3FILEBUFFER;
extern DWORD			g_RubbishPop3Count;

#endif
