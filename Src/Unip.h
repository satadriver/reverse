//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Unip
//	�汾:		1.0
//	��������:	2008-2-25
//	����:		��ǿ
//	��Ҫ����:	δ֪Э��IP����
//	ģ������:
//	��Ҫ����:	
//**************************************************************** 

#ifndef		_UNIP_H_
#define		_UNIP_H_

#if _MSC_VER > 1000
#pragma once
#endif

#define		UNIPPRO		"dce8"

class UnipProtocol : public DceProcotol
{
public:
	UnipProtocol();
	virtual	~UnipProtocol();
public:
	
public:
	virtual BOOL InitObject(DceMission* pMiss);
	virtual void ProcessBuffer(DATABUFFER& nogdata, time_t& packtm);
	virtual BOOL WriteObjectState(time_t packtm, CString& strtxt);
protected:
	void ClearSocket();
};

extern DceMission*		g_pUnipMissObj;

#endif