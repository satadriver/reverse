//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	ftp
//	版本:		1.0
//	创建日期:	2008-2-25
//	作者:		荣强
//	主要功能:	Ftp协议还原
//	模块描述:
//	主要函数:	
//**************************************************************** 

#ifndef			_FTP_H_
#define			_FTP_H_

#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"

#define				PROCESSBUFFERLEN		40960
#define				TLENET_SIONBUFSIZE		32*8192
#define				FTPPRO					"dce5"

class FtpProtocol : public DceProcotol
{
public:
	FtpProtocol();
	virtual	~FtpProtocol();

public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual BOOL ProcessPacket(DATEHEADER& packh, const char* pPacket, LPCTSTR spyaddr);
	virtual BOOL WriteObjectState(time_t packtm, CString& strtxt);
};


extern DWORD		g_FtpSion;
extern DWORD		g_TelnetSion;
extern DWORD		g_VncSion;

extern DWORD		g_TerminalSion;
extern DWORD		g_RadminSion;

extern char*		g_FtpFILEBUFFER;
extern char*		g_PROCESSBUFFER;

extern DceMission*	g_pFtpMissObj;

extern DWORD		g_FtpFileNum;
extern DWORD		g_TelnetFileNum;
extern DWORD		g_VncFileNum;
extern DWORD		g_EmuleFileNum;

#endif
