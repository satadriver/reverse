//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	OurGame
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	��������Э��Ļ�ԭ
//	ģ������:	
//	��Ҫ����:	
//
//**************************************************************** 

#ifndef		INCLUDE_AFX_OURGAME_H_20070712
#define		INCLUDE_AFX_OURGAME_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	��������:		IsHfPacket
//	����:			���һ�����Ƿ���������Ϸ�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����������Ϸ�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsOurgamePacket(DATEHEADER& packh, const char* pPacket)
{
	// �ж��Ƿ���������Ϸ�ĵ�¼��
	if ((packh.m_datalen > 4)
		&& ((memcmp(pPacket, "\x63\x06\0\0", 4) == 0)
			|| (memcmp(pPacket, "\x7d\0\0\0", 4) == 0)
			|| (memcmp(pPacket, "\x65\0\0\0", 4) == 0)))
	{
		// �ͻ��˵���������¼�İ�
		return TRUE;
	}
	
	// �ж��Ƿ���������Ϸ����İ�
	if ((packh.m_sport==2002 || packh.m_dport==2002) &&
		((packh.m_datalen>=68 && memcmp(pPacket, "\x8\x3", 2)==0) ||
		(packh.m_datalen>=684 && memcmp(pPacket, "\x5\x3", 2)==0)))
	{
		return TRUE;
	}

	return FALSE;
}



//*************************************************
//	������:			OurGameSession
//	��̳�:       	SocketSession
//	����:			��������Э��ĻỰ��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************
class OurGameSession : public SocketSession
{
public:
	OurGameSession(DATEHEADER& packh, int aT);
	
	~OurGameSession();

	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

	virtual BOOL CloseSession(time_t nowtm);

public:
	time_t			m_ProcessTime;
	DWORD			m_processseq1;
	DWORD			m_processseq2;
	BOOL			m_bSortList;
	DTcpPacketList	m_PacketList;
	
	char*			m_MsgBuf;
	int				m_MsgBufLen;

	DWORD			m_loginack;			// ��¼�������ACK(��¼״̬����ACK>��¼�������ACK+1204)����Ϊ�ǵ�¼�ɹ�
	DWORD			m_stateack;			// ��¼״̬����ACK
	DWORD			m_state;			// 1 ��ʾ����˵�¼�����
										// 2 ��ʾ����˵�¼״̬��
										// 4 ��ʾ�Ѿ�����˶����ļ�
protected:

	// ���������еİ�
	int ProcessOurGameList(time_t now);

	// ����һ������
	int ProcessOurGamePacket(DTcpPacket& pack, int direction);

	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);

	// �����û���¼ (����ǵ�¼���ͷ���TRUE)
	BOOL ProcessLocin(DATEHEADER& packh, const char *pPacket);

	// ��ȡ�û��ĵ�¼ID
	BOOL GetUserid(const char* pPacket, int len);

	// �����¼��Ϣ�ļ�
	void WriteLoginFile();

	// У����ṹ
	BOOL CheckPacket(const char* pPacket, int len);

};

#endif