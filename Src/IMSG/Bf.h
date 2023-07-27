//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Bf
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	��Ҫ����:	�߷���Ϸ�Ĵ���
//	ģ������:	
//	��Ҫ����:	IsBfPacket
//**************************************************************** 

#ifndef	INCLUDE_BF_H_20070711
#define INCLUDE_BF_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	������:			BfSession
//	��̳�:			SocketSession
//	����:			�߷���Ϸ�Ĵ�����
//	��Ҫ����:		InsertPacket
//	ʹ������:		
//**************************************************
class BfSession : public SocketSession
{
public:
	BfSession(DATEHEADER& packh, int aT);
	virtual ~BfSession();

	//*************************************************
	//	��������:		BfSession::InsertPacket
	//	����:			����һ���Ự��һ����
	//	�������:		packh �ǰ�ͷ�ṹ
	//					pPacket �ǰ�����
	//	�������:		
	//	����ֵ:			
	//	����:			
	//**************************************************
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);

public:
	void WriteBfIndexFile(time_t optm);
};


//*************************************************
//	��������:		IsBfPacket
//	����:			���һ�����Ƿ��Ǳ߷�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			����Ǳ߷�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//*************************************************
inline BOOL IsBfPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_datalen>50) && ((packh.m_type ==TCPPacket) || (packh.m_type==ProxyTCP)))
	{
		if ((memcmp(pPacket, "\x1\x0\x0\x0\x1\x0\x0\x0", 8) == 0)
			&& ((pPacket[14] > 0) && (pPacket[14] <= 24))
			&& ((pPacket[14+1 + pPacket[14]]) == 20))
		{
			return TRUE;
		}
	}
	return FALSE;
}

#endif