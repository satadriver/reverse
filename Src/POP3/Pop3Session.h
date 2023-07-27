// Pop3Session.h: interface for the Pop3Session class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_)
#define AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <comdef.h>
#import "..\\..\\lib\\msado15.dll" rename ("EOF","FileEOF") no_namespace 
#import "..\\..\\lib\\cdoex.dll" rename("Folder","CDOFolder") no_namespace

class Pop3Session : public SocketSession  
{
public:
	Pop3Session(DATEHEADER& packh, int aT, BOOL isServer);
	virtual ~Pop3Session();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	DWORD			m_CSeqOffset	;
	DWORD			m_CAckOffset	;
	DWORD			m_UserSeq;
	DWORD			m_PassSeq;
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
	CString			m_from;
	CString			m_to;
private:
	// �ʼ���ԭ��صĺ���(ʵ����Pop3Session.cpp)

	// ���������еİ�
	int Pop3LstToEml(char * buf, int bufsize);

	// ������һ���ʼ�ͷ
	BOOL Pop3NextSeqSession();
	
	// ��ø��ʼ��Ľ������к�
	static UINT GetemlEndSeq(const char *buf, UINT seq);

	// ����м����
	void Clear();

	// ����ʵ���ļ���
	BOOL CreateFilename();

	// ����ʵ���ļ�
	int SaveEmlTmpFile(int flg, char* pData, int datlen);

	// �ʼ�������صĺ���(ʵ����Pop3MailFile.cpp��)

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

CString SplitEmlName(CString emlname, CString username, BOOL isfrom);


#endif // !defined(AFX_POP3SESSION_H__19DD66C6_C5D5_43D8_B39C_B1B673BFF52B__INCLUDED_)
