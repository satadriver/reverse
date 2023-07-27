//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	EPH
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	��Ҫ����:	E��ͨ����Ĵ�����
//	��Ҫ����:	isEPHPacket
//**************************************************************** 

#ifndef		INCLUDE_EPH_H_20070711
#define		INCLUDE_EPH_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

class EphSession : public SocketSession
{
public:
	EphSession(DATEHEADER& packh, int aT);
	virtual ~EphSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int				m_direction;
	DWORD			m_state;		// ״̬1(�Ѿ��õ���ID); 2(�Ѿ��õ����ǳ�); 0x10(����)
	CString			m_subip;		// ����ip
	CString			m_Nick;			// �ǳ�
protected:

	//*************************************************
	//	��������:		EphSession::WriteEphmsgFile
	//	����:			��ԭһ��������ö���Ļ״̬(������)
	//	�������:		packh �ǰ�ͷ
	//					pData ��һ����������
	//	�������:		
	//	����ֵ:			�л�ԭ�������ݷ���TRUE���򷵻�FALSE
	//	����:			
	//**************************************************
	BOOL ProcessEphPacket(DATEHEADER& packh, const BYTE* pData);

	//*************************************************
	//	��������:		OutIndexFile
	//	����:			���һ��DoShow�Ķ�λ��Ϣ
	//	�������:		state ����Ļ״̬
	//	�������:		
	//	����ֵ:			
	//	����:			
	//*************************************************
	void OutIndexFile(LPCTSTR state);

};

//*************************************************
//	��������:		isEPHPacket
//	����:			�ж�һ�����ǲ���E��ͨ�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����E��ͨ�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL isEPHPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen<0x10)
		return FALSE;
	if ((packh.m_type == UDPPacket) || (packh.m_type == ProxyUDP))
	{
//�е�½��
		if ((pPacket[0x0A] == 0) && (pPacket[0x14] == 0)
			&& (*(USHORT*)(pPacket + 0x0E) == 0x400)
			&& (*(USHORT*)(pPacket + 2) == 0))
		{
			return (memcmp(pPacket, "\x72\x20", 2) == 0);

		}
		else if ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& (*(USHORT*)(pPacket + 0x0E) == 0x500)
			&& (*(USHORT*)(pPacket + 2) == 0))
		{
			return (memcmp(pPacket, "\x72\x20", 2) == 0);
		}
	}

	return FALSE;
}


//////////////////////////////////////////////////////////////////////////


class ImsgEphSession : public SocketSession
{
public:
	ImsgEphSession(DATEHEADER& packh, int aT);
	virtual ~ImsgEphSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
 
public:
	int				m_direction;
	time_t			m_ProcessTime;
	int				m_PackCount;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;


	char*		m_pMsgBuf;			// �������ݻ���
	int			m_MsgBuflen;
	CString     m_Colloquist; //��ϵ��

protected:

	//*************************************************
	//	��������:		EphSession::ProcessEPHList
	//	����:			��ԭ�������б���İ�
	//	�������:		��ǰ����ʱ��
	//	�������:		
	//	����ֵ:			
	//	����:			
	//**************************************************
	int ProcessEPHList(time_t now);

	//*************************************************
	//	��������:		EphSession::WriteEphmsgFile
	//	����:			��ԭһ��������дʵ���ļ�
	//	�������:		Packet ��һ����
	//					msgbuf �Ǵ�Ż�ԭ����Ϣ�Ļ�����
	//					int �ǻ������Ĵ�С
	//	�������:		
	//	����ֵ:			���ػ�ԭ����Ϣ��С
	//	����:			
	//**************************************************
	int ProcessEphPacket(DTcpPacket* Packet, char* msgbuf, int len);

	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);

};

//*************************************************
//	��������:		isEPHImsgPacket
//	����:			�ж�һ�����ǲ���E��ͨ�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����E��ͨ�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL isEPHImsgPacket(DATEHEADER& packh, const char* pPacket)
{
	if(packh.m_datalen<0x10)
		return FALSE;
	if ((packh.m_type == UDPPacket) || (packh.m_type == ProxyUDP))
	{
//�е�½��
		if ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& memcmp(pPacket+0xe,"\x0\x85",2)==0)
			
		{
			return (memcmp(pPacket, "\x72\x00\x00", 3) == 0);

		}
		else if  ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
			&& memcmp(pPacket+0xe,"\x80\x85",2)==0)
		{
			return (memcmp(pPacket, "\x72\x00\x00", 3) == 0);
		}
// 		else if  ((pPacket[0x0A] == 0) && (pPacket[0x13] == 0)
// 			&& memcmp(pPacket+0xe,"\x0\x85",2)==0)
// 		{
// 			return (memcmp(pPacket, "\x72\x00", 2) == 0);
// 		}
//��MSG
	}

	return FALSE;
}


#endif		//INCLUDE_EPH_H_20070711
