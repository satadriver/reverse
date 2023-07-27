//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Unip
//	版本:		1.0
//	创建日期:	2008-2-25
//	作者:		荣强
//	主要功能:	未知协议IP处理
//	模块描述:
//	主要函数:	
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