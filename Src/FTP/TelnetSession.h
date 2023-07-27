// TelnetSession.h: interface for the TelnetSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TELNETSESSION_H__209565A7_C770_4CFF_8296_D41E4DEDD0A7__INCLUDED_)
#define AFX_TELNETSESSION_H__209565A7_C770_4CFF_8296_D41E4DEDD0A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TelnetSession : public SocketSession  
{
public:
	TelnetSession(DATEHEADER& packh, int aT);
	virtual ~TelnetSession();
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);

public:
	CString			m_textfilename;	
	time_t			m_playspytm;		// 播放文件的截获时间(会话创建时间)
	CString			m_playfilename	;
	CString			m_playindexfile	;
	DWORD			m_fileseq	;
	DWORD			m_Ssequence	;
	DWORD			m_servernseq		;	// 指下一个包的seq
	DWORD			m_clientnseq		;	// 指下一个包的seq
	int				m_sortflg			;	// 链表的排序标志
	DTcpPacketList	m_PacketList		;	// 请求包链表
	BOOL			m_validatetelnet; 
	BOOL			m_validatetelnetCTR; 
	int				m_numcmd;
	int				m_numctr;
	char*			m_playbuffer;
	int				m_playlen;
private:
	int LstToFile(char* buf, int bufsize);
	int WriteTelnetPlayPacket(DTcpPacket& packet);
	int WriteTelnetTextfile(char* pData, int datlen);
	int WriteTelnetPlayFile(OLDDATEHEADER* pPackh, char* pData, int datlen);
};


//有几个telnet命令
int parsetelnetcommand(UCHAR *databuf,int len, BOOL server);
int parsetelnetctr(UCHAR *databuf,int len, BOOL server);
void TranslateTelnetBuf(LPCTSTR srcbuf, int srclen, FILE* pf);

#endif // !defined(AFX_TELNETSESSION_H__209565A7_C770_4CFF_8296_D41E4DEDD0A7__INCLUDED_)





















