// TaoBao.h: interface for the TaoBao class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_)
#define AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//*************************************************
//	������:			TaoBaoSession
//	��̳�:       	SocketSession
//	����:			TaoBao����Ļ�ԭ��
//	��Ҫ����:		
//	ʹ������:		
//**************************************************

class TaoBaoSession : public SocketSession
{
public:
	TaoBaoSession(DATEHEADER& packh, int aT);
	virtual ~TaoBaoSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// �Ự�����־
	BOOL		m_bIdc;				// �Ƿ�ƥ�������Դ
	CString     m_roomid;			// �����
	char*		m_pMsgBuf;			// �������ݻ���
	int			m_MsgBuflen;		
private:
	int FormatLoginOrOut(DATEHEADER& packh, const char* pPacket);
	int FormatMsg(DATEHEADER& packh, const char* pPacket,int Direction);//����1�����������2����������

	// ����VP������
//	int FormatVpIM(DATEHEADER& packh, const char* pPacket);

	// �����û���¼�İ�
//	int UserLogin(DATEHEADER& packh, const char* pPacket);

	// ���������Ϣ
//	CString GetImsg(const char* pPacket);

	// дʵ���ļ�
	void WriteRealFile(char* pData, int datalen);
};

//*************************************************
//	��������:		IsTaoBaoPacket
//	����:			���һ�����Ƿ���VP�İ�
//	�������:		packh �ǰ�ͷ�ṹ
//					pPacket �ǰ�����
//	�������:		
//	����ֵ:			�����VP�İ��ͷ���TRUE, ���򷵻�FALSE
//	����:			
//**************************************************
inline BOOL IsTaoBaoPacket(DATEHEADER& packh, const char* pPacket)
{
// 	if (memcmp(pPacket,"\x8f\x01\x01\x00",4) == 0 &&
// 		packh.m_datalen > 0x50 &&
// 		(memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||	//����
// 		memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||	//����
// 		memcmp(pPacket+0x48,"\x01\x2b",2) == 0 ||	//������������İ������ 01 2b
// 		memcmp(pPacket+0x48,"\x01\x30",2) == 0))	//������������ͻ��˵İ������ 01 30 
	if ((memcmp(pPacket,"\x8f\x01\x01\x00",4) == 0 ||
		memcmp(pPacket,"\x88\x06\x00\x00\x00\x00\x00",7) == 0 )&&
		packh.m_datalen > 0x50 &&
		(memcmp(pPacket+24,"\x03\x06\x00\x01",4) == 0||
		memcmp(pPacket+0x48,"\x01\xf5",2) == 0 ||	//����
		memcmp(pPacket+0x48,"\x01\x04",2) == 0 ||	//����
		memcmp(pPacket+0x48,"\x01\x2b",2) == 0 ||	//������������İ������ 01 2b
		memcmp(pPacket+0x48,"\x01\x30",2) == 0))	//������������ͻ��˵İ������ 01 30 

	{
		return TRUE;
	}
	return FALSE;
}

#endif // !defined(AFX_TAOBAO_H__1C8A3E5E_66DD_4F4F_9946_E112AA7AB937__INCLUDED_)
