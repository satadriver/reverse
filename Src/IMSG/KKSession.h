#ifndef KKSESSION_H
#define KKSESSION_H


#define KK_LOGIN 1
#define KK_MSG 2
#define KK_QUIT 3

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\\src\\SocketSession.h"

class KKSession : public SocketSession
{
public:
	KKSession(DATEHEADER& packh, int aT);
	virtual ~KKSession();
	static int IsKKpacket(DATEHEADER& packh, const char* pPacket);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
protected:
	void WriteKKIndexFile();
protected:
	int m_State;
	CString m_SenderID;
	CString m_RecverID;
	CString m_SenderName;
	CString m_RecverName;
	CString m_SenderAccount;
	CString m_RecverAccount;
	CString m_SenderPsw;
	CString m_Message;
	
	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};


#endif
