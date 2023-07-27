//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Mir
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	�����¼�Ļ�ԭ
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
//	������:			MirSession
//	��̳�:       	SocketSession
//	����:			����Э��Ļ�ԭ��
//	��Ҫ����:		
//	ʹ������:		
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
