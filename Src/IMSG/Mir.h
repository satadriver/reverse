//***************************************************************
//	Copyright (C) 2007 - 山东兆物科技发展有限公司版权所有
//	文件名: 	Mir
//	版本:		1.0
//	创建日期:	2007-7-12
//	作者:		荣强
//	主要功能:	传奇登录的还原
//**************************************************************** 

#ifndef		INCLUDE_AFX_MIR_H_20070712
#define		INCLUDE_AFX_MIR_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

typedef struct _CHUANQI_PAC_
{
	DWORD		m_Type;
	int			m_PacketType;
	int			m_direction;
	CString		m_UserName;
	CString		m_PWD;
	CString		m_Info;
}CHUANQIPAC,*LPCHUANQIPAC;

//*************************************************
//	类名称:			MirSession
//	类继承:       	SocketSession
//	描述:			传奇协议的还原类
//	主要函数:		
//	使用事项:		
//**************************************************
class MirSession : public SocketSession  
{
public:
	MirSession(DATEHEADER& packh, int aT);

	virtual ~MirSession();

	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
public:
	SimpleSeq	m_seq;
	CString		m_password;

protected:
	void OutputFile(time_t optm, LPCHUANQIPAC opinfo);
};

#endif // INCLUDE_AFX_MIR_H_20070712
