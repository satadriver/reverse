//***************************************************************
//	Copyright (C) 2007 - ɽ������Ƽ���չ���޹�˾��Ȩ����
//	�ļ���: 	VP
//	�汾:		1.0
//	��������:	2007-7-12
//	����:		��ǿ
//	��Ҫ����:	VP����Ļ�ԭ
//	��Ҫ����:	IsVpPacket
//**************************************************************** 

#ifndef INCLUDE_AFX_VP_H_20070712
#define INCLUDE_AFX_VP_H_20070712

#if _MSC_VER > 1000
#pragma once
#endif

//*************************************************
//	������:			VPIMSession
//	��̳�:       	SocketSession
//	����:			VP����Ļ�ԭ��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************
class VPIMSession : public SocketSession
{
public:
	VPIMSession(DATEHEADER& packh, int aT);
	virtual ~VPIMSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// �Ự�����־
	BOOL		m_bIdc;				// �Ƿ�ƥ�������Դ
	CString     m_roomid;			// �����
	char*		m_pMsgBuf;			// �������ݻ���
	int			m_MsgBuflen;		
private:

	// ����VP������
	int FormatVpIM(DATEHEADER& packh, const char* pPacket);

	// �����û���¼�İ�
	int UserLogin(DATEHEADER& packh, const char* pPacket);

	// ���������Ϣ
	CString GetImsg(const char* pPacket);

	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);
};

//*************************************************
//	��������:		IsVpPacket
//	����:			���һ�����Ƿ���VP�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����VP�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsVpPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen > 12)
	{
		if (stricmp(pPacket+12, "ClientText") == 0)		// ������Ϣ
			return TRUE;
		if (stricmp(pPacket+12, "SystemMessage") == 0)	// ϵͳ��Ϣ (�������ֿͻ���)
			return TRUE;
		if (stricmp(pPacket+12, "VerifyUser") == 0)		// ��½ 
			return TRUE;
	}
	return FALSE;
}


#endif // !defined(AFX_VP_H__50708853_14F3_4CFD_93B9_9CE547C28E90__INCLUDED_)
