
#if !defined(AFX_SMTPSESSION_H__A30620CE_38E9_41C6_A4A9_E39599C8291D__INCLUDED_)
#define AFX_SMTPSESSION_H__A30620CE_38E9_41C6_A4A9_E39599C8291D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

#define		SmtpBufPacketCount		100

/*************************************************
	������:			SmtpSession
	��̳�:			SocketSession
	����:			��ԭSmtpЭ��
	��Ҫ����:		
	��Ҫ����:		
	ʹ������:		
*************************************************/
class SmtpSession : public SocketSession  
{
public:
	SmtpSession(DATEHEADER& packh, int aT);
	virtual ~SmtpSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	DWORD			m_CSeqOffset;
	DWORD			m_CAckOffset;
	DWORD			m_UserSeq	;
	DWORD			m_PassSeq	;
	int				m_FLAG		;	// �ʼ���ԭ״̬:
									// ȡֵ��Χ: NOEML / EMLPROCING / EMLOVER
	BOOL			m_mailflag	;
	int				m_sortflag	;	// 0: ��ʾ����	����ֵ��ʾ����
	UINT			m_nseq		;	// ָ��һ������seq
	DWORD			m_fileseq	;
	TcpPacketList	m_Packetlst	;	// ���ݰ�����
	char*			m_buffer	;
	int				m_datlen	;
//	CString			m_USER		;
	CString			m_PASS		;
	CString			m_emlfield	;
	CString			m_from		;
	CString			m_to		;

private:
	// �ʼ���ԭ��صĺ���(ʵ����SmtpSession.cpp)

	// ���������еİ�
	int SmtpLstToEml(char * buf, int bufsize);

	// ������һ���ʼ�ͷ
	BOOL SMTPNextSeqSession();

	// ����м����
	void Clear();

	// ����ʵ���ļ���
	BOOL CreateFilename();

	// ����ʵ���ļ�
	int SaveEmlTmpFile(int flg, char* pData, int datlen);

	// ��ȡ������û���������
	void GetSmtpPass(DATEHEADER& packh, const char* pPacket);

	// �ʼ�������صĺ���(ʵ����SmtpMailFile.cpp��)

	// ����ʼ�(�ʼ�����, ���ĺ͸���)
	int StrippingMail(CString& filestr);

	// ��ȡ�ʼ�����
	int ExtractMailHeader(IMessagePtr& iMsg);

	// ��ȡ�ʼ����Ĳ������ı������ļ�
	int ExtractMailText(IMessagePtr& iMsg);

	// �ʼ������صĺ���(ʵ����SmtpMailFile.cpp��)

	// ���һ���ʼ�
	void DumpEmlFile();
};


CString SplitEmlName(CString emlname,CStringArray* arrto,BOOL isfrom);

#endif