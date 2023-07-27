//***************************************************************
//	文件名: 	smtp
//	版本:		1.0
//	创建日期:	2008-2-25
//	主要功能:	Smtp协议还原
//	模块描述:
//	主要函数:	
//**************************************************************** 

#ifndef		_SMTP_H_
#define		_SMTP_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

// 定义最大可还原的邮件大小
#define		SMTP_MAXMAILSIZE		15*1024*1024
#define		SMTP_SIONBUFSIZE		8192

#define		SMTPPRO					_T("dce7")

#define		NOEML					0			// 当前没有邮件在处理
#define		EMLPROCING				0xA			// 当前有邮件在处理
#define		EMLOVER					0xF			// 当前刚刚处理完了一封邮件(且处理成功)

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
