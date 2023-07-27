// ImapSession.h: interface for the ImapSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_)
#define AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

class ImapSession  : public SocketSession  
{
public:
	ImapSession(DATEHEADER& packh, int aT, BOOL isServer);
	virtual ~ImapSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);

public:
	DWORD			m_CSeqOffset	;
	DWORD			m_CAckOffset	;
	int				m_FLAG		;	// �ʼ���ԭ״̬:
									// ȡֵ��Χ: NOEML / EMLPROCING / EMLOVER
	int				m_sortflag	;	// 0:  ��ʾ����, ����ֵ��ʾ����
	BOOL			m_mailflag	;
	int				m_cntMail;		// �Ự�е��ʼ���
	int				m_emlfilecount;	// ��ԭ�����ʼ�����
	UINT			m_nseq		;	//ָ��һ������seq
	unsigned int	m_fileseq		;
	unsigned int	m_endseq		;	//crlf.crlf
	CString			m_emlfield		;
	TcpPacketList	m_PacketList	;	// ���ݰ�����
	CString			m_PASS	;
	char*			m_buffer;
	int				m_datlen;

private:
	// ������һ���ʼ�ͷ
	BOOL FindImapEmlHead();
	// ���������еİ�
	int Pop3LstToEml(char * buf, int bufsize);
	// ����ʵ���ļ���
	BOOL CreateFilename();
	// ����ʵ���ļ�
	int SaveEmlTmpFile(int flg, char* pData, int datlen);
	// ����м����
	void Clear();

	//////////////////////////////////////////////////////////////////////////
		// ����ʼ�(�ʼ�����, ���ĺ͸���)
	int StrippingMail(CString& filestr);

	// ��ȡ�ʼ�����
	int ExtractMailHeader(IMessagePtr& iMsg);

	// ��ȡ�ʼ����Ĳ������ı������ļ�
	int ExtractMailText(IMessagePtr& iMsg);

	// �ʼ������صĺ���(ʵ����Pop3MailFile.cpp��)

	// ���һ���ʼ�
	void DumpEmlFile();

};

#endif // !defined(AFX_IMAPSESSION_H__FD4768E9_5E9B_4AEC_91B6_400169237CE6__INCLUDED_)
