//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	pop3
//	版本:		1.0
//	创建日期:	2008-2-25
//	作者:		荣强
//	主要功能:	Pop3协议还原
//	模块描述:
//	主要函数:	
//**************************************************************** 

#ifndef			_POP3_H_
#define			_POP3_H_

// 定义最大可还原的邮件大小
#define		POP3_MAXMAILSIZE		15*1024*1024
#define		POP3_SIONBUFSIZE		8192

#define		POP3PRO					_T("dce3")

#define		NOEML					0			// 当前没有邮件在处理
#define		EMLPROCING				0xA			// 当前有邮件在处理
#define		EMLOVER					0xF			// 当前刚刚处理完了一封邮件(且处理成功)

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
