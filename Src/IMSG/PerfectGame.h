 // PerfectGame.h: interface for the Fetion class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PERFECTGAME_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_)
#define AFX_PERFECTGAME_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\\ReadDDeFile.h"
#include "..\\SocketSession.h"
#include "..\\DceMission.h"


class PerfectGameSession : public SocketSession
{
public:
	PerfectGameSession(DATEHEADER& packh, int aT);
	virtual ~PerfectGameSession();
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual BOOL CloseSession(time_t nowtm);
public:
	int			m_direction;		// �Ự�����־
	BOOL		m_bIdc;				// �Ƿ�ƥ�������Դ
	CString     m_roomid;			// �����
	char*		m_pMsgBuf;			// �������ݻ���
	int			m_MsgBuflen;		
};

BOOL isPerfectGame(DATEHEADER& packh, const char* pPacket);

#endif // !defined(AFX_PERFECTGAME_H__3C2F3FDE_2EE1_4D83_97E2_0B67AE7D5F6E__INCLUDED_)
