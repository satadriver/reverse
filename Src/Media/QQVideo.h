
#ifndef AFX_QQVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_
#define AFX_QQVIDEO_H__271604AF_0C90_4A76_8BE6_D7598E6ACB5F__INCLUDED_

/***************************  Flash视频处理  ******************************/

#if _MSC_VER > 1000
#pragma once
#endif


class QQSession : public MediaSession
{
public:
	QQSession(DATEHEADER& packh, int aT);
	virtual ~QQSession() { };
	virtual int InsertPacket(DATEHEADER& dataheader,const char *pPacket);
//	virtual BOOL CloseSession(time_t nowtm);
	virtual int ListToFile();
public:
	MediaPacketList m_packetlist;
// 	time_t		m_processtm;
	CHAR 		m_sipPacket[1600];
	OLDDATEHEADER  m_sipheader;
	int			m_Packettype;
	BOOL		m_b_haveID;		//已记下qq号
#ifdef _DEBUG
	DWORD		m_cntPacket;
#endif
};

int isQQVideo(DATEHEADER& dateHeader, BYTE* tp);

#endif