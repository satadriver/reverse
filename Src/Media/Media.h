//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ��Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Media
//	�汾:		1.0
//	��������:	2008-2-26
//	����:		��ǿ
//	��Ҫ����:	��ƵЭ�黹ԭ
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#ifndef			_MEDIA_H_
#define			_MEDIA_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "DogLib\Cryptoimw.h"
 
// #ifdef _DEBUG
// #pragma message("���Ӿ�̬����Կ�: CryptoRBD.lib")
// #pragma comment (lib, "lib\\CryptoRBD.lib")
// #else
// #pragma message("���ӵ���̬���п�: CryptoRB.lib")
// #pragma comment (lib, "lib\\CryptoRB.lib")
// #endif




enum Videotype
{
	enumempty		=0,
	enummms			=1,
	enumrealplay	=2,
	enumflash		=3,
	enumeph			=4,
	enumuc			=5,
	enumqq			=6,
	enummsn		    =7,
	enumyahoo		=8,
	enumpopo		=9,
	enumsqq			=10,
	enumnetmeet		=11,
	enumvoip		=12,
	enumskype		=13,
	enumnet2phone	=14,
	enumbluesky		=15,
	enumbdc			=16,
	enumoyescam		=17,
	enumvp			=18,
	enumUUTong		=19
};

#define			MEDIAPRO		"dce4"

#include "..\\DceMission.h"

class MediaProtocol : public DceProcotol
{
public:
	MediaProtocol();
	virtual	~MediaProtocol();

public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual void ProcessBuffer(DATABUFFER& packetbuf, time_t& packtm);
};

extern DATEHEADER DataHeader;
extern char		g_Packet[MAXLEN];
extern CString	MediaTmpPath;
extern DceMission* g_MediaMissObj;
extern DWORD	g_MediaFileNum;

inline CString CreateMediaFilename(LPCTSTR suffix, LPCTSTR sip, LPCTSTR dip)
{
	ASSERT(suffix!=NULL);
	ASSERT(suffix[0] != 0);
	if (_access(MediaTmpPath, 0) != 0)
		CreateDir(MediaTmpPath);
	SYSTEMTIME now;
	GetLocalTime(&now);
	CString fname;
	fname.Format("%s%04d%02d%02d%02d%02d%02d_%s_%s_%u.media.%s", MediaTmpPath,
			now.wYear, now.wMonth, now.wDay, now.wHour, now.wMinute, now.wSecond,
			sip, dip, now.wMilliseconds+rand(), suffix);
	return fname;
}

#endif
