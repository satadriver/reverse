//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	HF
//	�汾:		1.0
//	��������:	2007-7-11
//	����:		��ǿ
//	��Ҫ����:	�Ʒ���Ϸ�Ĵ���
//	��Ҫ����:	IsHfPacket
//	ģ������:	
//**************************************************************** 

#ifndef	INCLUDE_HF_H_20070711
#define INCLUDE_HF_H_20070711

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	������:			HfSession
//	��̳�:			SocketSession
//	����:			�Ʒ���Ϸ�Ĵ�����
//	��Ҫ����:		InsertPacket
//	ʹ������:		
//**************************************************
class HfSession : public SocketSession
{
public:
	HfSession(DATEHEADER& packh, int aT);
	virtual ~HfSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	
public:
	void WriteHfIndexFile(time_t optm);
	void FormatHfPacket(DATEHEADER& packh, const char* pPacket);
		
	int			m_infonum;		// ������()
	int			m_acctype;		// �ʺ�����
	CString		m_password;
};


//*************************************************
//	��������:		IsHfPacket
//	����:			���һ�����Ƿ��ǺƷ��İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			����ǺƷ��İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsHfPacket(DATEHEADER& packh, const char* pPacket)
{
	if( (memcmp(pPacket, "\x28\x0\x0\x0", 4) == 0) ||
		(memcmp(pPacket, "\xb0\x01\x0\x0", 4) == 0) ||
		(memcmp(pPacket, "\x68\x04\x0\x0", 4) == 0))
	{
		return TRUE;
	}
	else
		return FALSE;
}


#endif // !defined(AFX_HF_H__EFD7986D_26CD_436F_999D_8D32BC451F86__INCLUDED_)
