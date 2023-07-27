// IrcSession.h: interface for the CIrcSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IRCSESSION_H__664D7682_AD17_45D5_96E3_3B5C46BE9594__INCLUDED_)
#define AFX_IRCSESSION_H__664D7682_AD17_45D5_96E3_3B5C46BE9594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIrcSession  : public SocketSession
{
public:
	CIrcSession(DATEHEADER& packh, int aT);
	virtual ~CIrcSession();
	virtual int InsertPacket(DATEHEADER& dataheader, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);

private:
	DWORD			m_servernseq		;	// ָ��һ������seq
	DWORD			m_clientnseq		;	// ָ��һ������seq
	int				m_sortflg			;	// ����������־
	DTcpPacketList	m_PacketList		;	// ���������
	
	CString m_ircpass;
	CString m_textfilename;

private:
	int LstToFile(char* buf, int bufsize);
	int WriteIrcTextfile(char* pData, int datlen);

};

#endif // !defined(AFX_IRCSESSION_H__664D7682_AD17_45D5_96E3_3B5C46BE9594__INCLUDED_)
