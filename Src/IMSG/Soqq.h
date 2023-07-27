//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	Soqq
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	Soqq��ʱ��Ϣ��ԭ
//	��Ҫ����:	IsSoQQPacket
//**************************************************************** 

#ifndef		INCLUDE_AFX_SOQQ_H_20070712
#define		INCLUDE_AFX_SOQQ_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

#define		SOQQSTAT_EMLTOID	1	// ���任ID
#define		SOQQSTAT_ONLINE		2	// ����
#define		SOQQSTAT_OFFLINE	3	// ����
#define		SOQQSTAT_SENDMSG	4	// ������Ϣ


//*************************************************
//	������:			SoqqSession
//	��̳�:       	SocketSession
//	����:			Soqq��ʱ��Ϣ�Ự��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************
class SoqqSession : public SocketSession
{
public:
	SoqqSession(DATEHEADER& packh, int aT);

	virtual ~SoqqSession();

	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);

public:
	void FormatSoqqPacket(DATEHEADER& packh, const char* pPacket);

	void OutputFile(time_t optm, LPCTSTR optxt, int direction);

	int			m_direction;
	CString		m_SoqqEml;
	CString		m_Colloquist;
	SimpleSeq	m_Seq;

};

//*************************************************
//	��������:		IsSoQQPacket
//	����:			���һ�����Ƿ���SoQQ�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����SoQQ�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsSoQQPacket(DATEHEADER& packh, const char* pPacket)
{
	if ((packh.m_datalen > 12)
		&& ((packh.m_type == TCPPacket) || (packh.m_type == ProxyTCP)))
	{
		DWORD datlen = 0;
		BYTE* p = (BYTE*)pPacket;
		for (int i = 0; i < 10; i++)
		{
			if ((*p==0x12)&&(p[2]==0)/*&&(p[3]==1)*/)
			{
				WORD len = Swap2Byte(*((WORD*)(p+6)));
				if (len < 8)
				{
					return FALSE;
				}
				datlen += len;
				if (datlen == packh.m_datalen)
				{
					return TRUE;
				}
				if (datlen > packh.m_datalen)
				{
					return FALSE;
				}
				p += len;
			}
			else
			{
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}


#endif	// INCLUDE_AFX_SOQQ_H_20070712