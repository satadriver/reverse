//***************************************************************
//	�ļ���: 	smtp
//	�汾:		1.0
//	��������:	2008-2-25
//	��Ҫ����:	SmtpЭ�黹ԭ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#ifndef		_SMTP_H_
#define		_SMTP_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

// �������ɻ�ԭ���ʼ���С
#define		SMTP_MAXMAILSIZE		15*1024*1024
#define		SMTP_SIONBUFSIZE		8192

#define		SMTPPRO					_T("dce7")

#define		NOEML					0			// ��ǰû���ʼ��ڴ���
#define		EMLPROCING				0xA			// ��ǰ���ʼ��ڴ���
#define		EMLOVER					0xF			// ��ǰ�ոմ�������һ���ʼ�(�Ҵ���ɹ�)

class SmtpProtocol : public DceProcotol
{
public:
	SmtpProtocol();
	virtual	~SmtpProtocol();
public:
	
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr);
	virtual BOOL WriteObjectState(time_t now, CString& strtxt);
};

//extern HANDLE		g_SmtpHeap;
extern DceMission*	g_smtpMissObj;
extern char*		g_SmtpFileBuffer;
extern DWORD		g_RubbishCount;
extern BOOL			g_RubPacketCatch;
extern DWORD		g_ReUseCount;
extern DWORD		g_smtpFileNum;

#endif
