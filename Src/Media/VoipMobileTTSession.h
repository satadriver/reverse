#ifndef VOIP_TT_SESSION_H_H_H
#define VOIP_TT_SESSION_H_H_H
#include "voip.h"


class VoipMobileTTSession : public MediaSession
{
public:
	VoipMobileTTSession(DATEHEADER& packh, int aT, char * Packet);
	virtual ~VoipMobileTTSession();
	
	virtual int InsertPacket(DATEHEADER& packh, const char *pPacket);
	virtual int ListToFile();
	static BOOL IsVoipMobileTT(DATEHEADER& packh, const char *pPacket);
public:
	VoipPacketList	m_CallerPackList;
	VoipPacketList	m_CalleePackList;
	int				m_Packettype;

	VOIPFILEHEADER	m_VoipFileHdr;

	unsigned short	m_CallerPackIdx;
	unsigned int	m_CallerPackTime;
	unsigned int	m_CallerPackUnique;
	unsigned short	m_CalleePackIdx;
	unsigned int	m_CalleePackTime;
	unsigned int	m_CalleePackUnique;
	unsigned int	m_CallerIP;
	unsigned int	m_CalleeIP;
	unsigned short	m_CallerPORT;
	unsigned short	m_CalleePORT;
	unsigned int	m_CallerPackCnt;
	unsigned int	m_CalleePackCnt;
};
#endif