// FtpSession.h: interface for the FtpSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FTPSESSION_H__B13620ED_D883_49AD_9300_DDB88C1C6639__INCLUDED_)
#define AFX_FTPSESSION_H__B13620ED_D883_49AD_9300_DDB88C1C6639__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class FtpSession : public SocketSession
{
public:
	FtpSession(DATEHEADER& packh, int aT, BOOL bServer);
	virtual ~FtpSession();
	virtual int InsertPacket(DATEHEADER& dataheader, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
	
public:
// 	UINT			m_Ssequence	;		//指下一个包的seq
	UINT			m_CSeqOffset	;
	UINT			m_CAckOffset	;
	BOOL			m_ftpflag		;
	CString			m_CurDir;
// 	CString			m_USER	;
	CString			m_PASS	;
	TcpPacketList	m_ClientPacketlst;		//请求包链表
	TcpPacketList	m_ServerPacketlst;		//请求包链表	
	int				m_retrnum;
	int				m_stornum;
	CString			m_lastupFile;		// 上次上传的文件名(用于与上次去重)
	CString			m_lastdnFile;		// 上次上传的文件名(用于与上次去重)

public:
	CString ProcessCMD(CString cmd);
	CString Process257(CString cmd);
	void AddFtpFile(int &num , CString &field ,const char * buf, CString &lastFile);
//	void WriteIndexFile(CString& text);


};

#endif // !defined(AFX_FTPSESSION_H__B13620ED_D883_49AD_9300_DDB88C1C6639__INCLUDED_)
