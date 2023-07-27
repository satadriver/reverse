#ifndef ISAUDIOSESSION_H
#define ISAUDIOSESSION_H


#define IS_LOGIN 1
#define IS_MSG 2
#define MSG_LEN 512
#define SENDERNAME_LEN 45

#if _MSC_VER > 1000
#pragma once
#endif

#include "..\\src\\SocketSession.h"

class ISAudioSession : public SocketSession
{
public:
	ISAudioSession(DATEHEADER& packh, int aT);
	virtual ~ISAudioSession();
	static int IsISAudio(DATEHEADER& packh, const char* pPacket);
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);	
protected:
	void WriteISAudioIndexFile();
protected:
	int m_State;
	CString m_SenderID;
	CString m_RecverID;
	CString m_SenderName;
	CString m_RecverName;
	CString m_SenderAccount;
	CString m_Message;

	CString m_tranfilename;
	unsigned char	m_firstsip[4];
	BOOL m_iswrite;
};


#endif
