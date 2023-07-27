// wuyiguagua.h: interface for the wuyiguagua class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_)
#define AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	������:			VPIMSession
//	��̳�:       	SocketSession
//	����:			VP����Ļ�ԭ��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************
class WuyiguaguaSession : public SocketSession
{
public:
	WuyiguaguaSession(DATEHEADER& packh, int aT);
	virtual ~WuyiguaguaSession();
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
//	��������:		IsWyggPacket
//	����:			���һ�����Ƿ���IsWyggPacket�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����VP�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsWyggPacket(DATEHEADER& packh, const char* pPacket)
{
	if (packh.m_datalen > 28)
	{
		if (memcmp(pPacket,"\x90\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",12)==0 &&
			memcmp(pPacket+15,"\x00\x04",0)==0)
		{
			return TRUE;//����
		}
		else if (memcmp(pPacket,"\x00\x00\x00\x00\x00\x00\x0a",7)==0&&
			memcmp(pPacket+15,"\x00\x0b",0)==0)
		{
			return TRUE;//����
		}
		if (stricmp(pPacket+12, "ClientText") == 0)		// ������Ϣ
			return TRUE;
		if (stricmp(pPacket+12, "SystemMessage") == 0)	// ϵͳ��Ϣ (�������ֿͻ���)
			return TRUE;
		if (stricmp(pPacket+12, "VerifyUser") == 0)		// ��½ 
			return TRUE;
	}
	return FALSE;
}

#endif // !defined(AFX_WUYIGUAGUA_H__EC8ECD3E_BE92_4D0B_B009_401173A8C436__INCLUDED_)
