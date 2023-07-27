//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	wow
//	�汾:		1.0
//	��������:	2009-4-14
//	����:		��ǿ
//	��Ҫ����:	ħ��������Ϸ�ĵ�¼��ԭ
//	ģ������:	
//	��Ҫ����:	IsWoWPacket
//**************************************************************** 

#ifndef	INCLUDE_WOW_H_20090414
#define INCLUDE_WOW_H_20090414

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	������:			WowSession
//	��̳�:			SocketSession
//	����:			ħ��������Ϸ�Ĵ�����
//	��Ҫ����:		InsertPacket
//	ʹ������:		
//**************************************************
class WowSession : public SocketSession
{
public:
	WowSession(DATEHEADER& packh, int aT);
	virtual ~WowSession();

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

protected:
	void WriteWowIndexFile();

	// �ӵ�¼�İ��л�ȡ��¼ID (�ɹ�����TRUE, ʧ�ܷ���FALSE)
	BOOL GetLoginUserid(const char* pPacket, int len);

protected:
	DWORD			m_state;			// 1 ��ʾ����˵�¼�����
										// 2 ��ʾ����˵�¼״̬��
										// 4 ��ʾ�Ѿ�����˶����ļ�
};


//*************************************************
//	��������:		IsWoWPacket
//	����:			���һ�����Ƿ���ħ������İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����ħ������İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//*************************************************
inline int IsWoWPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_type ==TCPPacket) || (packh.m_type==ProxyTCP))
	{
		WORD* tp = (WORD*)(pPacket+2);
		if ((packh.m_datalen > 34) && (*tp == packh.m_datalen-4)
			&& (pPacket[33]+34 == packh.m_datalen)
			&& (memcmp(pPacket, "\x0\x8", 2) == 0)
			&& (memcmp(pPacket+4, "WoW\0", 4) == 0))
		{
			return 1;	// �ǵ�¼�İ�
		}
// 		if((packh.m_datalen==4) && (memcmp(pPacket, "\1\4\3\0", 4) == 0))
// 		{
// 			return TRUE;	// ��¼ʧ��ʱ���صİ�
// 		}
		if((packh.m_datalen==5) && (memcmp(pPacket, "\x10\0\0\0\0", 5) == 0))
		{
			return 2;	// ��¼�ɹ�ʱ�İ�
		}
	}
	return 0;
}

// ��׼�˿�3724
// ��¼�İ� Client --> Server(����������userid UTF-8����)
// 0030                     00 08 2a 00 57 6f 57 00 03 00        ..*.WoW...
// 0040   05 4e 24 36 38 78 00 6e 69 57 00 4e 43 68 7a e0  .N$68x.niW.NChz.
// 0050   01 00 00 c0 a8 00 02 0c[e6 88 91 e7 9a 84 e8 b4  ................
// 0060   a6 e5 8f b7]                                      ....

// 0030                     00 08 26 00 57 6f 57 00 03 00        ..&.WoW...
// 0040   05 4e 24 36 38 78 00 6e 69 57 00 4e 43 68 7a e0  .N$68x.niW.NChz.
// 0050   01 00 00 c0 a8 00 02 08[53 49 4e 41 54 54 4d 50] ........SINATTMP

// �ֽ�0-1�̶�Ϊ"00 08" ����Ϊ����
// �ֽ�2-3Ϊ�����ݵĳ���
// �ֽ�4-7�̶�Ϊ"WoW\0" ����Ϊ����
// �ֽ�33 userid�ĳ���
// �ֽ�33- userid(���������ڵĲ���)

// �����¼ʧ�ܾͷ���"\1\4\3\0" 4Byte Client <-- Server
// 0030                     01 04 03 00 00 00 00 00               ....

// �����¼�ɹ�����һ����("\x10\0\0\0\0") 5Byte Client --> Server
// 0030                     10 00 00 00 00                        .....

#endif